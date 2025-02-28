<?php

/**
 * Удалить банковские выписки на дату
 *
 * @param $timestamp - unix
 * @return void
 */
function finance_delete_bank_docs($timestamp, $company)
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

function finance_import_bank_docs($array)
{
  // старт транзакции БД, откатить изменения при ошибке
  $transaction = db_transaction();

  try {
    if (!empty($array['date'])) {
      $date_ts = helper_datetime_to_timestamp($array['date']);

      // удалить старые документы на дату
      finance_delete_bank_docs($date_ts, $array['company']);

      // создать запись с остатком в кассе на начало дня
      $bank_doc = entity_create('bank_doc', ['type' => 'bank_doc']);
      $bank_doc_wr = entity_metadata_wrapper('bank_doc', $bank_doc);
      $bank_doc_wr->field_bd_company = $array['company'];
      $bank_doc_wr->field_bd_datetime = $date_ts;
      $bank_doc_wr->field_bd_type = FINANCE_DOC_TYPE_CASH;
      $bank_doc_wr->field_bd_income = $array['cash'];
      $bank_doc_wr->field_bd_purpose = 'balance';
      $bank_doc_wr->save();

      // создать запись с остатком в банке на начало дня
      $bank_doc = entity_create('bank_doc', ['type' => 'bank_doc']);
      $bank_doc_wr = entity_metadata_wrapper('bank_doc', $bank_doc);
      $bank_doc_wr->field_bd_company = $array['company'];
      $bank_doc_wr->field_bd_datetime = $date_ts;
      $bank_doc_wr->field_bd_type = FINANCE_DOC_TYPE_BANK;
      $bank_doc_wr->field_bd_income = $array['bank'];
      $bank_doc_wr->field_bd_purpose = 'balance';
      $bank_doc_wr->save();

      // создать запись с остатком депозитов на начало дня
      if (isset($array['bankd'])) {
        $bank_doc = entity_create('bank_doc', ['type' => 'bank_doc']);
        $bank_doc_wr = entity_metadata_wrapper('bank_doc', $bank_doc);
        $bank_doc_wr->field_bd_company = $array['company'];
        $bank_doc_wr->field_bd_datetime = $date_ts;
        $bank_doc_wr->field_bd_type = FINANCE_DOC_TYPE_BANK_DEPOSIT;
        $bank_doc_wr->field_bd_income = $array['bankd'];
        $bank_doc_wr->field_bd_purpose = 'balance';
        $bank_doc_wr->save();
      }

      foreach ($array['load'] as $doc) {
        $datetime_ts = helper_datetime_to_timestamp($doc['datetime']);

        $bank_doc = entity_create('bank_doc', ['type' => 'bank_doc']);
        $bank_doc_wr = entity_metadata_wrapper('bank_doc', $bank_doc);

        $bank_doc_wr->field_bd_company = $array['company'];
        $bank_doc_wr->field_bd_datetime = $datetime_ts;
        $bank_doc_wr->field_bd_type = $doc['type'];
        $bank_doc_wr->field_bd_income = $doc['income'];
        $bank_doc_wr->field_bd_outcome = $doc['outcome'];
        $bank_doc_wr->field_bd_purpose = $doc['purpose'];
        $bank_doc_wr->field_bd_agent = $doc['agent'];
        $bank_doc_wr->save();
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
