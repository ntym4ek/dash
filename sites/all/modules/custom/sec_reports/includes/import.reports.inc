<?php

function reports_outcome_categories_import($array)
{
  if (is_array($array) && !empty($array['load'])) {
    $vid = taxonomy_vocabulary_machine_name_load('outcome_categories')->vid;

    $new_records = [];
    foreach ($array['load'] as $category) {
      if ($category['id'] && $category['name']) {

        if ($term = dash_term_get_by_1c_id('outcome_categories', $category['id'])) {
          $category_wr = entity_metadata_wrapper('taxonomy_term', $term);
          $category_wr->name = $category['name'];
          $category_wr->field_status = 1;
          $category_wr->save();

          $new_records[] = $category_wr->getIdentifier();
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
    watchdog('import', 'Статьи расходов импортированы.', [], WATCHDOG_INFO);
    return true;
  } else {
    // drupal_set_message('Импорт клиентов ТД не выполнен.', 'warning');
    watchdog('import', 'Импорт статей расходов не выполнен.', [], WATCHDOG_ERROR);
    return false;
  }
}

/**
 * Импорт данных в таблицу Расходов денежных средств
 * @param $array
 */
function reports_cash_outcome_import($array)
{
  if (is_array($array) && !empty($array['load'])) {
    $month_start = helper_datetime_to_timestamp($array['month']);
    $record_type = $array['type'];
    // для компаний инн является 1c_id
    $company = dash_term_get_by_1c_id('companies', $array['company_inn']);

    // подготовка Записей на месяц
    $data = [];
    foreach ($array['load'] as $item) {
      $date_start = helper_datetime_to_timestamp($item['date']);
      $category = dash_term_get_by_1c_id('outcome_categories', $item['category_id']);
      $data[] = [
        'type' => $record_type,
        'date' => $date_start,
        'category_id' => $category->tid,
        'amount' => $item['amount'],
      ];
    }

    if ($data) {
      // удалить старые записи на месяц
      reports_cash_outcome_delete_records($month_start, $company->tid, $record_type);

      // создание записей за месяц
      foreach ($data as $item) {
        dash_cash_outcome_create(
          [
            'created' => REQUEST_TIME,
            'company_id' => $company->tid,
            'month_ts' => $item['date'],
            'type' => $item['type'],
            'category_id' => $item['category_id'],
            'amount' => $item['amount'],
            'manual' => 0,
          ]
        );
      }

      watchdog('import', 'Записи расхода ДС импортированы.', [], WATCHDOG_INFO);
      return $data;
    }
  }
  watchdog('import', 'Импорт записей расходов ДС не выполнен.', [], WATCHDOG_ERROR);
  return false;
}

/**
 * Импорт данных в таблицу Прихода денежных средств
 * @param $array
 */
function reports_cash_income_import($array)
{
  if (is_array($array) && !empty($array['load']) && !empty($array['inn'])) {
    $month_start = helper_datetime_to_timestamp($array['month']);
    $company = dash_term_get_by_1c_id('companies', $array['inn']);

    // подготовка массива на заданный месяц
    $cash_income = [];
    foreach ($array['load'] as $arr_item) {
      $client = dash_client_get_by_1c_id($array['inn'], $arr_item['client_id']);
      $direction = dash_direction_get_by_1c_id($arr_item['direction_id']);
      $cash_income[] = [
        'date' => helper_datetime_to_timestamp($arr_item['date']),
        'client_id' => $client->tid ?? 0,
        'direction_id' => $direction->tid ?? 0,
        'amount' => $arr_item['amount'],
      ];
    }

    if ($cash_income && in_array($array['type'], [DASH_TYPE_PLAN, DASH_TYPE_FACT, DASH_TYPE_DEAL])) {

      // удалить старый план на месяц
      dash_cash_income_delete_by_month(['month_start' => $month_start, 'type' => $array['type'], 'company_id' => $company->tid, 'manual' => 0]);

      // создание записей
      foreach ($cash_income as $item) {
        dash_cash_income_create([
          'created' => REQUEST_TIME,
          'company_id' => $company->tid,
          'type' => $array['type'],
          'month_ts' => $item['date'],
          'client_id' => $item['client_id'],
          'direction_id' => $item['direction_id'],
          'amount' => $item['amount'],
          'manual' => 0,
        ]);
      }

      watchdog('import', 'Приход ДС импортирован.', [], WATCHDOG_INFO);
      // drupal_set_message('План выручки ТД импортирован.');
      return $cash_income;
    }
  }

  watchdog('import', 'Импорт прихода ДС не выполнен.', [], WATCHDOG_ERROR);
  return false;
}

/**
 * Импорт данных в таблицу Кредитов и задолженностей
 * @param $array
 */
function reports_debt_import($array)
{
  if (is_array($array) && !empty($array['load'])) {
    $month_start = helper_datetime_to_timestamp($array['month']);
    // для компаний инн является 1c_id
    $company = dash_term_get_by_1c_id('companies', $array['company_inn']);

    // подготовка Записей на месяц
    $data = [];
    foreach ($array['load'] as $item) {
      $data[] = [
        'category' => $item['category'],
        'title' => $item['title'],
        'type' => DASH_TYPE_FACT,
        'kind' => $item['type'],
        'amount' => $item['amount'],
        'amount2' => $item['amount2'],
      ];
    }

    if ($data) {
      // удалить старые записи на месяц
      reports_debt_delete_records($month_start, $company->tid, $array['source']);

      // создание записей за месяц
      foreach ($data as $item) {
        dash_debt_create([
          'created' => REQUEST_TIME,
          'company_id' => $company->tid,
          'month_ts' => $month_start,
          'type' => $item['type'],
          'kind' => $item['kind'],
          'category' => $item['category'],
          'name' => $item['title'],
          'amount' => $item['amount'],
          'amount2' => $item['amount2'],
          'source' => $array['source'],
          'manual' => 0,
        ]);
      }

      watchdog('import', 'Записи Кредитов и задолженностей импортированы.', [], WATCHDOG_INFO);
      return $data;
    }
  }
  watchdog('import', 'Импорт записей Кредитов и задолженностей не выполнен.', [], WATCHDOG_ERROR);
  return false;
}

/**
 * Импорт данных в таблицу Остатков готовой продукции и сырья
 * @param $array
 */
function reports_remains_brief_import($array)
{
  if (is_array($array) && !empty($array['load'])) {
    $month_start = helper_datetime_to_timestamp($array['month']);
    // для компаний инн является 1c_id
    $company = dash_term_get_by_1c_id('companies', $array['company_inn']);

    // подготовка Записей на месяц
    $data = [];
    foreach ($array['load'] as $item) {
      $data[] = [
        'category' => $item['category'],
        'amount' => $item['amount'],
      ];
    }

    if ($data) {
      // удалить старые записи на месяц
      reports_remains_brief_delete_records($month_start, $company->tid);

      // создание записей за месяц
      foreach ($data as $item) {
        dash_remains_brief_create([
          'created' => REQUEST_TIME,
          'company_id' => $company->tid,
          'month_ts' => $month_start,
          'category' => $item['category'],
          'amount' => $item['amount'],
        ]);
      }

      watchdog('import', 'Записи Остатков готовой продукции и сырья импортированы.', [], WATCHDOG_INFO);
      return $data;
    }
  }
  watchdog('import', 'Импорт записей Остатков готовой продукции и сырья не выполнен.', [], WATCHDOG_ERROR);
  return false;
}

/**
 * Импорт данных в таблицу Производство.Клиенты
 * @param $array
 */
function reports_production_clients_import($array)
{
  if (is_array($array) && !empty($array['load'])) {
    $month_start = helper_datetime_to_timestamp($array['month']);
    if ($month_start && in_array($array['type'], [DASH_TYPE_PLAN, DASH_TYPE_FACT])) {

      // подготовка Записей на месяц
      $data = [];
      foreach ($array['load'] as $arr_item) {
        $client = dash_client_get_by_1c_id(DASH_INN_AH, $arr_item['client_id']);
        $data[] = [
          'date_ts' => helper_datetime_to_timestamp($arr_item['date']),
          'client_id' => $client->tid,
          'volume' => $arr_item['volume'],
        ];
      }

      if ($data) {
        // удалить старые записи на месяц
        reports_production_clients_delete_records($month_start, $array['type']);

        // создание записей за месяц
        foreach ($data as $item) {
          dash_production_clients_create([
            'created' => REQUEST_TIME,
            'client_id' => $item['client_id'],
            'type' => $array['type'],
            'date_ts' => $item['date_ts'],
            'volume' => $item['volume'],
            'manual' => 0,
          ]);
        }

        watchdog('import', 'Записи Объёмов производства импортированы.', [], WATCHDOG_INFO);
        return true;
      }
    }
  }

  watchdog('import', 'Импорт записей Объёмов производства не выполнен.', [], WATCHDOG_ERROR);
  return false;
}

function reports_ship_import($array)
{
  if (is_array($array) && !empty($array['load'])) {
    $month_start = helper_datetime_to_timestamp($array['month']);
    // для компаний инн является 1c_id
    $company = dash_term_get_by_1c_id('companies', $array['company_inn']);
    $company_inn = trim($array['company_inn']);

    $table = '';
    if (empty($array['source']) || $array['source'] == 'ship') {
      $table = 'ship';
    } elseif ($array['source'] == 'accounts') {
      $table = 'accounts';
    }

    // подготовка Записей
    $ship = [];
    foreach ($array['load'] as $load_item) {
      $ship[] = [
        'amount' => $load_item['amount'] ?? 0,
      ];

      if ($table == 'ship') {
        $client = dash_client_get_by_1c_id($company_inn, $load_item['clients_id']);
        $nom = dash_nomenclature_get_by_1c_id($company_inn, $load_item['nom_id']);
        $date_start = helper_datetime_to_timestamp($load_item['date']);

        $ship[] = [
          'date' => $date_start,
          'client_id' => $client->tid,
          'nom_id' => $nom->tid,
          'quantity' => $load_item['quantity'] ?? 0,
          'net_cost' => $load_item['net_cost'] ?? 0,
        ];
      }
      if ($table == 'accounts') {
        $direction = dash_direction_get_by_1c_id($load_item['direction_id']);
        $ship[] = [
          'date' => $month_start,
          'direction_id' => $direction->tid,
        ];
      }
    }


    if ($ship && isset($array['type']) && $company) {
      // удалить старый план на месяц
      // для Неоплаченных контрактов месяц не задан, тк это срез на момент импорта
      $params = ['type' => $array['type'], 'company_id' => $company->tid, 'manual' => 0, 'table' => $table];
      if (isset($array['month'])) {
        $params['month_start'] = helper_datetime_to_timestamp($array['month']);
      }
      dash_ship_delete_by_month($params);

      // создание Плана на месяц
      foreach ($ship as $ship_item) {
        dash_ship_create([
          'table' => $table,
          'type' => $array['type'],
          'company_id' => $company->tid,
          'date_ts' => $ship_item['date'],
          'amount' => $ship_item['amount'],
          'quantity' => $ship_item['quantity'] ?? 0,
          'net_cost' => $ship_item['net_cost'] ?? 0,
          'nom_id' => $ship_item['nom_id'] ?? 0,
          'client_id' => $ship_item['client_id'] ?? 0,
          'direction_id' => $ship_item['direction_id'] ?? 0,
        ]);
      }

      watchdog('import', 'План отгрузки ТД импортирован.', [], WATCHDOG_INFO);
      return $ship;
    }
  }

  watchdog('import', 'Импорт плана отгрузки ТД не выполнен.', [], WATCHDOG_ERROR);
  return false;
}


/**
 * Удалить записи определённого типа за месяц.
 * Удаляем только данные из 1с, введённые вручную не трогаем
 *
 * @param $month_start - unix timestamp
 * @param $record_type - 1 - план, 2 - факт
 * @param $company_id - tid компании
 * @return void
 */
function reports_cash_outcome_delete_records($month_start, $company_id, $record_type)
{
  if ($month_start) {
    $next_month_start = gmmktime(0, 0, 0, date('n', $month_start)+1, 1, date('Y', $month_start));

    $efquery = new EntityFieldQuery();
    $efquery
      ->entityCondition('entity_type', 'cash_outcome')
      ->entityCondition('bundle', 'cash_outcome')
      ->fieldCondition('field_co_date', 'value', $month_start, '>=')
      ->fieldCondition('field_co_date', 'value', $next_month_start, '<')
      ->fieldCondition('field_co_type', 'value', $record_type)
      ->fieldCondition('field_co_company', 'tid', $company_id)
      ->fieldCondition('field_co_is_manual', 'value', 0);

    $result = $efquery->execute();
    if ($result) {
      entity_delete_multiple('cash_outcome', array_keys($result['cash_outcome']));
    }
  }
}

/**
 * Удалить записи за месяц из таблицы Кредитов и задолженностей
 *
 * @param $month_start - unix timestamp
 * @param $company_id - tid компании
 * @param $source - источник данных
 * @return void
 */
function reports_debt_delete_records($month_start, $company_id, $source)
{
  if ($month_start) {
    $next_month_start = gmmktime(0, 0, 0, date('n', $month_start)+1, 1, date('Y', $month_start));

    $efquery = new EntityFieldQuery();
    $efquery
      ->entityCondition('entity_type', 'debt')
      ->entityCondition('bundle', 'debt')
      ->fieldCondition('field_d_company', 'tid', $company_id)
      ->fieldCondition('field_d_date', 'value', $month_start, '>=')
      ->fieldCondition('field_d_date', 'value', $next_month_start, '<')
      ->fieldCondition('field_d_source', 'value', $source);

    $result = $efquery->execute();
    if ($result) {
      entity_delete_multiple('debt', array_keys($result['debt']));
    }
  }
}

/**
 * Удалить записи за месяц из таблицы Остатков готовой продукции и сырья
 *
 * @param $month_start - unix timestamp
 * @param $company_id - tid компании
 * @return void
 */
function reports_remains_brief_delete_records($month_start, $company_id)
{
  if ($month_start) {
    $next_month_start = gmmktime(0, 0, 0, date('n', $month_start)+1, 1, date('Y', $month_start));

    $efquery = new EntityFieldQuery();
    $efquery
      ->entityCondition('entity_type', 'remains')
      ->entityCondition('bundle', 'remains_brief')
      ->fieldCondition('field_rm_date', 'value', $month_start, '>=')
      ->fieldCondition('field_rm_date', 'value', $next_month_start, '<')
      ->fieldCondition('field_rm_company', 'tid', $company_id);

    $result = $efquery->execute();
    if ($result) {
      entity_delete_multiple('remains', array_keys($result['remains']));
    }
  }
}

/**
 * Удалить записи за месяц из таблицы Производство/Клиенты.
 * Ручной ввод не удаляется.
 *
 * @param $month_start - unix timestamp
 * @param $type - план/факт
 * @return void
 */
function reports_production_clients_delete_records($month_start, $type)
{
  if ($month_start) {
    $next_month_start = gmmktime(0, 0, 0, date('n', $month_start)+1, 1, date('Y', $month_start));

    $efquery = new EntityFieldQuery();
    $efquery
      ->entityCondition('entity_type', 'production')
      ->entityCondition('bundle', 'clients')
      ->fieldCondition('field_prod_date', 'value', $month_start, '>=')
      ->fieldCondition('field_prod_date', 'value', $next_month_start, '<')
      ->fieldCondition('field_prod_type', 'value', $type)
      ->fieldCondition('field_prod_is_manual', 'value', 0);

    $result = $efquery->execute();
    if ($result) {
      entity_delete_multiple('production', array_keys($result['production']));
    }
  }
}

