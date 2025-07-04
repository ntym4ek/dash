<?php

function dep_2v_2g_import_regions($array)
{
  if (is_array($array) && !empty($array['load'])) {

    foreach ($array['load'] as $region) {
      if ($region['id'] && $region['name']) {

        $term = dash_get_region_by_1c_id($region['id']);
        $parent_term = empty($region['parent_id']) ? 0 : dash_get_region_by_1c_id($region['parent_id']);

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
    $vid = taxonomy_vocabulary_machine_name_load('clients_td')->vid;

    $new_records = [];
    foreach ($array['load'] as $client) {
      if ($client['id'] && $client['name']) {

        if ($term = dash_get_client_by_1c_id('td', $client['id'])) {
          $client_wr = entity_metadata_wrapper('taxonomy_term', $term);
          $client_wr->name = $client['name'];
          $client_wr->field_status = 1;
          if ($region_tid = dash_get_region_by_1c_id($client['region_id'])) {
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

function dep_2v_2g_import_nomenklatura_td($array)
{
  if (is_array($array) && !empty($array['load'])) {
    $new_records = [];
    foreach($array['load'] as $nom) {
      if ($nom['id'] && $nom['name'] && $nom['art']) {
        // проверить наличие записи Номенклатуры по артикулу (тк не у всех прописан ID)
        $term = dash_get_nomenklatura_by_1c_artikul('td', $nom['art']);
        // если нет по артикулу, искать по ID или создать
        if (!$term) {
          $term = dash_get_term_by_1c_id('nomenklatura_td', $nom['id']);
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
      $vid = taxonomy_vocabulary_machine_name_load('nomenklatura_td')->vid;
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
    $record_type = $array['fact'];
    $plan_import_time = REQUEST_TIME;

    // подготовка Плана на месяц
    $plan = [];
    foreach ($array['load'] as $load_item) {
      $client = dash_get_client_by_1c_id('td', $load_item['clients_id']);
      if (isset($client->tid)) {
        // todo перейти на поиск по nom_id
        $nom = dash_get_nomenklatura_by_1c_artikul('td', $load_item['artikul_1c']);
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
      dep_2v_2g_shipplan_td_month_delete($month_start, $record_type);

      // создание Плана на месяц
      foreach ($plan as $plan_item) {
        $plan = entity_create('plan', [
          'type' => 'plan_ship',
          'created' => $plan_import_time,
        ]);
        entity_save('plan', $plan);

        $plan_wr = entity_metadata_wrapper('plan', $plan);
        $plan_wr->field_record_type_td = $plan_item['record_type'];
        $plan_wr->field_date = $plan_item['date'];
        $plan_wr->field_client_td = $plan_item['client'];
        $plan_wr->field_nomenklatura_td = $plan_item['nom'];
        $plan_wr->field_quantity_td = $plan_item['qty'];
        $plan_wr->field_amount = $plan_item['amount'];
        $plan_wr->field_net_cost = $plan_item['net_cost'];
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
    $record_type = $array['fact'];
    $plan_import_time = time();

    // подготовка Плана на месяц
    $plan = [];
    foreach ($array['load'] as $plan_item) {
//      $client = import_get_client_by_id('td', $plan_item['clients_id']);
      $date_arr = explode('.', $plan_item['date']);
      $date_start = gmmktime(0, 0, 0, $date_arr[1], $date_arr[0], $date_arr[2]);

      $plan[] = [
        'record_type' => $record_type,
        'date' => $date_start,
        'client' => null, // план выручки с 17.02.2023 выгружается без привязки к клиенту
        'amount' => $plan_item['amount'],
      ];
    }

    if ($plan) {
      // удалить старый план на месяц
      dep_2v_2g_payplan_td_month_delete($month_start, $record_type);

      // создание Плана на месяц
      foreach ($plan as $date => $plan_item) {
        $plan = entity_create('plan', [
          'type' => 'plan_revenue',
          'created' => $plan_import_time,
        ]);
        entity_save('plan', $plan);

        $plan_wr = entity_metadata_wrapper('plan', $plan);
        $plan_wr->field_record_type_td = $plan_item['record_type'];
        $plan_wr->field_date = $plan_item['date'];
//        $plan_wr->field_client_td = $plan_item['client']; // план выручки с 17.02.2023 выгружается без привязки к клиенту
        $plan_wr->field_amount = $plan_item['amount'];
        $plan_wr->save();
      }

      watchdog('import', 'План выручки ТД импортирован.', [], WATCHDOG_INFO);
      // drupal_set_message('План выручки ТД импортирован.');
      return $plan;
    }
  } else {
    watchdog('import', 'Импорт плана выручки ТД не выполнен.', [], WATCHDOG_ERROR);
    return false;
  }
}

/**
 * Удалить записи за период, кроме введённых вручную
 * @param $month_start
 * @param $record_type
 * @return void
 */
function dep_2v_2g_shipplan_td_month_delete($month_start, $record_type)
{
  if ($month_start || $record_type == 2) {
    $next_month_start = gmmktime(0, 0, 0, date('n', $month_start)+1, 1, date('Y', $month_start));

    $efquery = new EntityFieldQuery();
    $efquery
      ->entityCondition('entity_type', 'plan')
      ->entityCondition('bundle', 'plan_ship')
      ->fieldCondition('field_record_type_td', 'value', $record_type)
      ->fieldCondition('field_is_manual', 'value', 1, '!=');

    if ($record_type != 2) {
      $efquery
        ->fieldCondition('field_date', 'value', $month_start, '>=')
        ->fieldCondition('field_date', 'value', $next_month_start, '<');
    }

    $result = $efquery->execute();
    if ($result) {
      entity_delete_multiple('plan', array_keys($result['plan']));
    }
  }
}

function dep_2v_2g_payplan_td_month_delete($month_start, $record_type)
{
  if ($month_start) {
    $next_month_start = gmmktime(0, 0, 0, date('n', $month_start)+1, 1, date('Y', $month_start));

    $efquery = new EntityFieldQuery();
    $efquery
      ->entityCondition('entity_type', 'plan')
      ->entityCondition('bundle', 'plan_revenue')
      ->fieldCondition('field_date', 'value', $month_start, '>=')
      ->fieldCondition('field_date', 'value', $next_month_start, '<')
      ->fieldCondition('field_record_type_td', 'value', $record_type);

    $result = $efquery->execute();
    if ($result) {
      entity_delete_multiple('plan', array_keys($result['plan']));
    }
  }
}
