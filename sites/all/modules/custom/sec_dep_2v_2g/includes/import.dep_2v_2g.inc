<?php

function dep_2v_2g_import_regions($array)
{
  if (is_array($array) && !empty($array['load'])) {

    foreach ($array['load'] as $region) {
      if ($region['id'] && $region['name']) {

        $term = dash_region_get_by_1c_id($region['id']);
        $parent_term = empty($region['parent_id']) ? 0 : dash_region_get_by_1c_id($region['parent_id']);

        $term_wr = entity_metadata_wrapper('taxonomy_term', $term);
        $term_wr->field_1c_id = $region['id'];
        $term_wr->parent = [$parent_term->tid ?? 0];
        $term_wr->name = $region['name'];
        $term_wr->save();
      }
    }

    // drupal_set_message('Регионы импортированы.');
    watchdog('import', 'Регионы импортированы.', [], WATCHDOG_INFO);
    return true;
  } else {
    // drupal_set_message('Импорт регионов не выполнен.', 'warning');
    watchdog('import', 'Импорт регионов не выполнен.', [], WATCHDOG_ERROR);
    return false;
  }
}

function dep_2v_2g_import_clients_td($array)
{
  if (is_array($array) && !empty($array['load'])) {
    $vid = taxonomy_vocabulary_machine_name_load('clients')->vid;

    $new_records = [];
    foreach ($array['load'] as $client) {
      if ($client['id'] && $client['name']) {

        if ($term = dash_client_get_by_1c_id(DASH_INN_TD, $client['id'])) {
          $client_wr = entity_metadata_wrapper('taxonomy_term', $term);
          $client_wr->name = $client['name'];
          $client_wr->field_status = 1;
          if ($region_tid = dash_region_get_by_1c_id($client['region_id'])) {
            $client_wr->field_region = $region_tid;
          } else {
            watchdog('import', 'Не найден регион ' . $client['region_id'] . ' при импорте клиентов ТД', [], WATCHDOG_ERROR);
          }
          $client_wr->save();

          $new_records[] = $client_wr->getIdentifier();
        }
      }
    }

    // если полное обновление справочника, то запомнить отсутствующие записи и сделать их неактивными
    if ($array['reload']) {
      $dbr = db_select('taxonomy_term_data', 't');
      $dbr->condition('t.vid', $vid);
      $dbr->addField('t', 'tid');
      $terms_old = $dbr->execute()->fetchAllAssoc('tid');

      foreach ($terms_old as $tid => $term_old) {
        if (!in_array($tid, $new_records)) {
          $term_wr = entity_metadata_wrapper('taxonomy_term', $tid);
          $term_wr->field_status = 0;
          $term_wr->save();
        }
      }
    }

    // drupal_set_message('Клиенты ТД импортированы.');
    watchdog('import', 'Клиенты ТД импортированы.', [], WATCHDOG_INFO);
    return true;
  } else {
    // drupal_set_message('Импорт клиентов ТД не выполнен.', 'warning');
    watchdog('import', 'Импорт клиентов ТД не выполнен.', [], WATCHDOG_ERROR);
    return false;
  }
}

function dep_2v_2g_import_nomenklatura($array)
{
  if (is_array($array) && !empty($array['load'])) {
    $new_records = [];
    foreach($array['load'] as $nom) {
      if ($nom['id'] && $nom['name'] && $nom['art']) {
        // проверить наличие записи Номенклатуры по артикулу (тк не у всех прописан ID)
        $term = dash_nomenklatura_get_by_1c_artikul(DASH_INN_TD, $nom['art']);
        // если нет по артикулу, искать по ID или создать
        if (!$term) {
          $term = dash_term_get_by_1c_id('nomenklatura', $nom['id']);
        }

        if ($term) {
          // обновить значения полей
          $term_wr = entity_metadata_wrapper('taxonomy_term', $term);
          $term_wr->name = $nom['name'];
          $term_wr->field_1c_id = trim($nom['id']);
          $term_wr->field_nom_artikul = trim($nom['art']);
          if (isset($nom['ei']))  $term_wr->field_nom_unit = trim($nom['ei']);
          if (isset($nom['nds'])) $term_wr->field_nom_nds = trim($nom['nds']);
          $term_wr->field_status = 1;
          $term_wr->save();

          $new_records[] = $term_wr->getIdentifier();
        }
      }
    }

    // если полное обновление справочника, то запомнить отсутствующие записи и сделать их неактивными для клиента
    if ($array['reload']) {
      $vid = taxonomy_vocabulary_machine_name_load('nomenklatura')->vid;
      $dbr = db_select('taxonomy_term_data', 't');
      $dbr->condition('t.vid', $vid);
      $dbr->addField('t', 'tid');
      $noms_old = $dbr->execute()->fetchAllAssoc('tid');

      foreach ($noms_old as $n_tid => $nom_old) {
        if (!in_array($n_tid, $new_records)) {
          $term_wr = entity_metadata_wrapper('taxonomy_term', $n_tid);
          $term_wr->field_status = 0;
          $term_wr->save();
        }
      }
    }

    // drupal_set_message('Номенклатура ТД импортирована.');
    watchdog('import', 'Номенклатура ТД импортирована.', [], WATCHDOG_INFO);
    return true;
  } else {
    // drupal_set_message('Импорт номенклатуры ТД не выполнен.', 'warning');
    watchdog('import', 'Импорт номенклатуры ТД не выполнен.', [], WATCHDOG_ERROR);
    return false;
  }
}



function dep_2v_2g_import_ship_plan_td($array)
{
  if (is_array($array) && !empty($array['load'])) {
    if ($array['fact'] == 0) $record_type = DASH_TYPE_PLAN;
    if ($array['fact'] == 1) $record_type = DASH_TYPE_FACT;

    $plan_import_time = REQUEST_TIME;

    // подготовка Плана на месяц
    $plan = [];
    foreach ($array['load'] as $load_item) {
      $client = dash_client_get_by_1c_id(DASH_INN_TD, $load_item['clients_id']);
      if (isset($client->tid)) {
        // todo перейти на поиск по nom_id
        $nom = dash_nomenklatura_get_by_1c_artikul(DASH_INN_TD, $load_item['artikul_1c']);
        if (isset($nom->tid)) {
          $date_arr = explode('.', $load_item['date']);
          $date_start = gmmktime(0, 0, 0, $date_arr[1], $date_arr[0], $date_arr[2]);

          $plan[] = [
            'record_type' => $record_type,
            'date' => $date_start,
            'client' => $client->tid,
            'nom' => $nom->tid,
            'qty' => $load_item['quantity'] ?? 0,
            'amount' => $load_item['amount'] ?? 0,
            'net_cost' => $load_item['net_amount'] ?? 0,
          ];
        } else {
          watchdog('import', 'Ошибка при импорте плана отгрузки ТД. Не найдена номенклатура (' . $load_item['artikul_1c'] . ')', [], WATCHDOG_WARNING);
        }
      } else {
        watchdog('import', 'Ошибка при импорте плана отгрузки ТД. Не найден клиент (' . $load_item['clients_id'] . ')', [], WATCHDOG_WARNING);
      }
    }

    if ($plan) {
      // удалить старый план на месяц
      // для Неоплаченных контрактов месяц не задан, тк это срез на момент импорта
      $month_start = null;
      if (isset($array['month'])) {
        $month_arr = explode('.', $array['month']);
        $month_start = gmmktime(0, 0, 0, $month_arr[1], 1, $month_arr[2]);
      }
      $company = dash_term_get_by_1c_id('companies', DASH_INN_TD);
      dash_ship_delete_by_month(['month_start' => $month_start, 'type' => $record_type, 'company_id' => $company->tid, 'manual' => 0]);

      // создание Плана на месяц
      foreach ($plan as $plan_item) {
        $plan = entity_create('ship', [
          'type' => 'ship',
          'created' => $plan_import_time,
        ]);
        entity_save('ship', $plan);

        $plan_wr = entity_metadata_wrapper('ship', $plan);
        $plan_wr->field_s_type = $plan_item['record_type'];
        $plan_wr->field_s_date = $plan_item['date'];
        $plan_wr->field_s_client = $plan_item['client'];
        $plan_wr->field_s_nomenklatura = $plan_item['nom'];
        $plan_wr->field_s_quantity = $plan_item['qty'];
        $plan_wr->field_s_amount = $plan_item['amount'];
        $plan_wr->field_s_net_cost = $plan_item['net_cost'];
        $plan_wr->field_s_is_manual = 0;
        $plan_wr->save();
      }

      watchdog('import', 'План отгрузки ТД импортирован.', [], WATCHDOG_INFO);
//      drupal_set_message('План отгрузки ТД импортирован.');
      return $plan;
    }
  } else {
    watchdog('import', 'Импорт плана отгрузки ТД не выполнен.', [], WATCHDOG_ERROR);
    return false;
  }
}

function dep_2v_2g_import_pay_plan_td($array)
{
  if (is_array($array) && !empty($array['load'])) {
    $month_arr = explode('.', $array['month']);
    $month_start = gmmktime(0, 0, 0, $month_arr[1], 1, $month_arr[2]);
    if ($array['fact'] == 0) $record_type = DASH_TYPE_PLAN;
    if ($array['fact'] == 1) $record_type = DASH_TYPE_FACT;
    $plan_import_time = time();

    // подготовка массива на заданный месяц
    $cash_income = [];
    foreach ($array['load'] as $arr_item) {
      $date_arr = explode('.', $arr_item['date']);
      $date_start = gmmktime(0, 0, 0, $date_arr[1], $date_arr[0], $date_arr[2]);

      $cash_income[] = [
        'record_type' => $record_type,
        'date' => $date_start,
        'amount' => $arr_item['amount'],
      ];
    }

    if ($cash_income) {
      // удалить старый план на месяц
      $company = dash_term_get_by_1c_id('companies', DASH_INN_TD);
      dash_cash_income_delete_by_month(['month_start' => $month_start, 'type' => $record_type, 'company_id' => $company->tid, 'manual' => 0]);

      // создание Плана на месяц
      foreach ($cash_income as $date => $item) {
        $cash_income = entity_create('cash_income', [
          'type' => 'cash_income',
          'created' => $plan_import_time,
        ]);
        entity_save('cash_income', $cash_income);

        $plan_wr = entity_metadata_wrapper('cash_income', $cash_income);
        $plan_wr->field_ci_company = $company->tid;
        $plan_wr->field_ci_type = $item['record_type'];
        $plan_wr->field_ci_date = $item['date'];
        $plan_wr->field_ci_amount = $item['amount'];
        $plan_wr->field_ci_is_manual = 0;
        $plan_wr->save();
      }

      watchdog('import', 'План выручки ТД импортирован.', [], WATCHDOG_INFO);
      // drupal_set_message('План выручки ТД импортирован.');
      return $cash_income;
    }
  } else {
    watchdog('import', 'Импорт плана выручки ТД не выполнен.', [], WATCHDOG_ERROR);
    return false;
  }
}

/**
 *  Удалить данные Отгрузке продукции (сущность ship/ship)
 *  с учётом заданных параметров.
 *
 * @param $conditions
 *   - company_id* - tid компании kccc
 *   - month_start - массив ['from' =>, 'to' => не включительно]           * - обязательно
 *   - type - тип записи (план, факт, неотгруженные контракты)
 *   - manual - данные из 1с или ручной ввод (если не задано, то только данные из 1с)
 *
 *  Для DASH_TYPE_DEAL дата не имеет значения, поэтому их удаляем все, без периода.
 * @return array|mixed|null
 */
function dash_ship_delete_by_month($conditions)
{
  if ($conditions['company_id'] && $conditions['type'] && $conditions['manual']
    && ($conditions['month_start'] || $conditions['type'] == DASH_TYPE_DEAL)) {
    $next_month_start = gmmktime(0, 0, 0, date('n', $conditions['month_start'])+1, 1, date('Y', $conditions['month_start']));

    $efquery = new EntityFieldQuery();
    $efquery
      ->entityCondition('entity_type', 'ship')
      ->entityCondition('bundle', 'ship')
      ->fieldCondition('field_s_company', 'value', $conditions['company_id'])
      ->fieldCondition('field_s_type', 'value', $conditions['type'])
      ->fieldCondition('field_s_is_manual', 'value', $conditions['manual']);

    if ($conditions['type'] != DASH_TYPE_DEAL) {
      $efquery
        ->fieldCondition('field_s_date', 'value', $conditions['month_start'], '>=')
        ->fieldCondition('field_s_date', 'value', $next_month_start, '<');
    }

    $result = $efquery->execute();
    if ($result) {
      entity_delete_multiple('ship', array_keys($result['ship']));
    }
  }
}

