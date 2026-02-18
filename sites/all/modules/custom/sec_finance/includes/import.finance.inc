<?php

/**
 * Удалить банковские выписки компании на дату
 *
 * @param $timestamp - unix
 * @param $company - текстовое наименование компании
 * @return void
 */
function finance_bank_docs_delete_by_date($timestamp, $company)
{
  $timestamp_from = gmmktime(0, 0, 0, date('n', $timestamp), date('j', $timestamp), date('Y', $timestamp));
  $timestamp_to = $timestamp_from + 86400 - 1;

  $dbr = db_select('eck_bank_doc', 'bc');
  $dbr->condition('bc.type', 'bank_doc');
  $dbr->innerJoin('field_data_field_bd_datetime', 'fdt', 'fdt.entity_id = bc.id');
  $dbr->condition('fdt.field_bd_datetime_value', [$timestamp_from, $timestamp_to], 'BETWEEN');
  $dbr->innerJoin('field_data_field_bd_company', 'fc', 'fc.entity_id = bc.id');
  $dbr->condition('fc.field_bd_company_value', $company);
  $dbr->orderBy('fdt.field_bd_datetime_value');
  $dbr->addField('bc', 'id');
  if ($ids = $dbr->execute()->fetchCol()) {
    entity_delete_multiple('bank_doc', $ids);
  }
}

function finance_bank_docs_import($array)
{
  // старт транзакции БД, откатить изменения при ошибке
  $transaction = db_transaction();

  try {
    if (!empty($array['date'])) {
      $date_ts = helper_datetime_to_timestamp($array['date']);

      // удалить старые документы на дату
      finance_bank_docs_delete_by_date($date_ts, $array['company']);

      // создать записи с остатком в кассе на начало и конец дня
      $params = [
        'company' => $array['company'],
        'date_ts' => $date_ts,
        'type' => FINANCE_DOC_TYPE_CASH,
        'purpose' => 'balance',
        'income' => $array['cash'][0],
        'outcome' => $array['cash'][1],
      ];
      dash_bank_doc_create($params);

      // создать записи с остатком в банке на начало и конец дня
      $params = [
        'company' => $array['company'],
        'date_ts' => $date_ts,
        'type' => FINANCE_DOC_TYPE_BANK,
        'purpose' => 'balance',
        'income' => $array['bank'][0],
        'outcome' => $array['bank'][1],
      ];
      dash_bank_doc_create($params);

      // создать записи с остатком депозитов на начало и конец дня
      if (isset($array['bankd'])) {
        $params = [
          'company' => $array['company'],
          'date_ts' => $date_ts,
          'type' => FINANCE_DOC_TYPE_BANK_DEPOSIT,
          'purpose' => 'balance',
          'income' => $array['bankd'][0],
          'outcome' => $array['bankd'][1],
        ];
        dash_bank_doc_create($params);
      }

      foreach ($array['load'] as $doc) {
        $datetime_ts = helper_datetime_to_timestamp($doc['datetime']);

        $params = [
          'company' => $array['company'],
          'date_ts' => $datetime_ts,
          'type' => $doc['type'],
          'purpose' => $doc['purpose'],
          'income' => $doc['income'],
          'outcome' => $doc['outcome'],
          'agent' => $doc['agent'],
        ];
        dash_bank_doc_create($params);
      }

      watchdog('import', 'Банковские выписки за @date импортированы.', ['@date' => $array['date']], WATCHDOG_INFO);
      return true;
    }
  }
  catch (Exception $e) {
    $transaction->rollback();
    watchdog('import', 'Ошибка при импорте банковских выписок. История выполнения: <pre>"@trace"</pre>', ['@trace' => var_dump($e->getTrace())], WATCHDOG_ERROR);
  }

  return false;
}

