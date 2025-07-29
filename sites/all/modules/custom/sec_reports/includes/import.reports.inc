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
      reports_debt_delete_records($month_start, $company->tid);

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
 * @return void
 */
function reports_debt_delete_records($month_start, $company_id)
{
  if ($month_start) {
    $next_month_start = gmmktime(0, 0, 0, date('n', $month_start)+1, 1, date('Y', $month_start));

    $efquery = new EntityFieldQuery();
    $efquery
      ->entityCondition('entity_type', 'debt')
      ->entityCondition('bundle', 'debt')
      ->fieldCondition('field_d_date', 'value', $month_start, '>=')
      ->fieldCondition('field_d_date', 'value', $next_month_start, '<')
      ->fieldCondition('field_d_company', 'tid', $company_id)
      ->fieldCondition('field_d_is_manual', 'value', 0);

    $result = $efquery->execute();
    if ($result) {
      entity_delete_multiple('debt', array_keys($result['debt']));
    }
  }
}
