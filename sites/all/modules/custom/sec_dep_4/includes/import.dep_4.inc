<?php
/**
 * Удалить записи Истории валют по дате
 */
function dep_4_delete_curr_history_by_date($date)
{
  $dbr = db_select('eck_currency_rate', 'ecr');
  $dbr->innerJoin('field_data_field_cr_date', 'fcd', 'fcd.entity_id = ecr.id');
  $dbr->condition('fcd.field_cr_date_value', $date);
  $dbr->addField('ecr', 'id');
  if ($ids = $dbr->execute()->fetchCol()) {
    entity_delete_multiple('currency_rate', $ids);
  }
}

/**
 * Удалить записи Истории валют по дате
 */
function dep_4_delete_analog_price_by_1c_ID($id_1c)
{
  $dbr = db_select('eck_analog_price', 'eap');
  $dbr->innerJoin('field_data_field_ap_doc_1c_id', 'fid', 'fid.entity_id = eap.id');
  $dbr->condition('fid.field_ap_doc_1c_id_value', $id_1c);
  $dbr->addField('eap', 'id');
  if ($ids = $dbr->execute()->fetchCol()) {
    entity_delete_multiple('analog_price', $ids);
  }
}

function dep_4_import_analog_prices($array)
{
  // старт транзакции БД, откатить изменения при ошибке
  $transaction = db_transaction();

  try {
    if (is_array($array)) {
      if (!empty($array['currencies'])) {
        foreach ($array['currencies'] as $item) {
          $date_arr = explode('.', $item['date']);
          $date_unix = gmmktime(0, 0, 0, $date_arr[1], $date_arr[0], $date_arr[2]);

          // если есть на эту дату, удалить
          dep_4_delete_curr_history_by_date($date_unix);

          // добавить новые записи
          $vid = taxonomy_vocabulary_machine_name_load('currencies')->vid;
          foreach ($item['load'] as $curr_item) {
            $currencies = taxonomy_get_term_by_name($curr_item['name'], 'currencies');
            $currency = reset($currencies);
            if (!$currency) {
              $currency = entity_create('taxonomy_term', ['vid' => $vid, 'name' => $curr_item['name'], 'field_iso_4217' => ['und' => [0 => ['value' => $curr_item['code']]]]]);
              entity_save('taxonomy_term', $currency);
            }
            $curr_rate = entity_create('currency_rate', ['type' => 'currency_rate']);
            $curr_rate_wr = entity_metadata_wrapper('currency_rate', $curr_rate);
            $curr_rate_wr->field_cr_date = $date_unix;
            $curr_rate_wr->field_cr_currency = $currency->tid;
            $curr_rate_wr->field_cr_rate = $curr_item['rate'];
            $curr_rate_wr->save();
          }
        }
      }

      if (!empty($array['delete'])) {
        // удалить записи Цен на аналоги по ID документа
        foreach ($array['delete'] as $del_item) {
          dep_4_delete_analog_price_by_1c_ID($del_item['doc_id']);
        }
      }

      if (!empty($array['load'])) {
        // сгруппировать записи по ID документа
        $ap_by_doc = [];
        foreach ($array['load'] as $item) {
          if ($item['doc_id']) {
            $ap_by_doc[$item['doc_id']][$item['nom_id']] = $item;
          } else {
            watchdog('import', 'При импорте Цен на аналоги не задан 1с ID документа.', [], WATCHDOG_WARNING);
          }
        }

        // удалить старые и внести новые записи Цен на аналоги
        foreach ($ap_by_doc as $doc_id => $ap_items) {
          dep_4_delete_analog_price_by_1c_ID($doc_id);
          foreach ($ap_items as $ap_item) {
            if ($ap_item['nom_id']) {
              if ($nom = dash_term_get_by_1c_id('nomenklatura_td', $ap_item['nom_id'])) {
                $date_arr = explode('.', $ap_item['date']);
                $date_unix = gmmktime(0, 0, 0, $date_arr[1], $date_arr[0], $date_arr[2]);

                $ap = entity_create('analog_price', ['type' => 'analog_price']);
                $ap_wr = entity_metadata_wrapper('analog_price', $ap);

                $ap_wr->field_ap_nom_td = $nom->tid;
                $ap_wr->field_ap_date = $date_unix;
                $ap_wr->field_ap_doc_1c_id = $ap_item['doc_id'];
                $ap_wr->field_ap_price = $ap_item['price'];
                $ap_wr->field_ap_delivery_china = $ap_item['delivery_china'];
                $ap_wr->field_ap_customs_tax = $ap_item['customs_tax'];
                $ap_wr->field_ap_delivery_russia = $ap_item['delivery_russia'];
                $ap_wr->field_ap_manufacturer = $ap_item['manufacturer'];
                $ap_wr->save();
              }
            } else {
              watchdog('import', 'При импорте Цен на аналоги не задана номенклатура.', [], WATCHDOG_WARNING);
            }
          }
        }

        watchdog('import', 'Список Цен на аналоги импортирован.', [], WATCHDOG_INFO);
        return true;
      }
    }
  }
  catch (Exception $e) {
    $transaction->rollback();
    watchdog('import', 'Ошибка выполнения кода при импорте цен на аналоги. История выполнения: <pre>"@trace"</pre>', ['@trace' => var_dump($e->getTrace())], WATCHDOG_ERROR);
  }

  return false;
}
