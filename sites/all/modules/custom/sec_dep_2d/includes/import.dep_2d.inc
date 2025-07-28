<?php

function dep_2d_import_storages($array)
{
  $voc = taxonomy_vocabulary_machine_name_load('warehouses');

  if (is_array($array) && !empty($array['load'])) {

    $terms_new = [];
    foreach($array['load'] as $record) {
      if ($record['name'] && $record['id']) {
        // проверить наличие записи в справочнике или создать её
        $term = dash_term_get_by_1c_id('warehouses', $record['id']);

        // обновить значения полей
        $term_wr = entity_metadata_wrapper('taxonomy_term', $term);
        $term_wr->name = trim($record['name']);
        $term_wr->field_status = 1;
        $term_wr->save();

        $terms_new[] = $term_wr->getIdentifier();
      }
    }

    // если полное обновление справочника, то запомнить отсутствующие записи и сделать их неактивными для клиента
    if ($array['reload']) {
      $dbr = db_select('taxonomy_term_data', 't');
      $dbr->condition('t.vid', $voc->vid);
      $dbr->addField('t', 'tid');
      $terms_old = $dbr->execute()->fetchAllAssoc('tid');

      foreach ($terms_old as $tid => $term_old) {
        if (!in_array($tid, $terms_new)) {
          $term_wr = entity_metadata_wrapper('taxonomy_term', $tid);
          $term_wr->field_status = 0;
          $term_wr->save();
        }
      }
    }

    watchdog('import', 'Справочник ' . $voc->name . ' импортирован.', [], WATCHDOG_INFO);
    return true;
  } else {
    watchdog('import', 'Импорт справочника ' . $voc->name . ' не выполнен.', [], WATCHDOG_ERROR);
    return false;
  }
}

function dep_2d_import_remains($file_arr)
{
  if (is_array($file_arr) && !empty($file_arr['load']) && !empty($file_arr['start_date']) && !empty($file_arr['end_date'])) {
    $import_time = REQUEST_TIME;

    // подготовка массива к импорту
    $import_arr = [];
    foreach ($file_arr['load'] as $load_item) {
      $warehouse  = dash_term_get_by_1c_id('warehouses', $load_item['storage_id']);
      $nom        = dash_term_get_by_1c_id('nomenklatura_td', $load_item['nom_id']);
      $date_arr   = explode('.', $load_item['date']);
      $date_prod  = gmmktime(0, 0, 0, $date_arr[1], $date_arr[0], $date_arr[2]);
      $date_expire = '';
      if ($load_item['expiration_date']) {
        $date_arr = explode('.', $load_item['expiration_date']);
        $date_expire = gmmktime(0, 0, 0, $date_arr[1], $date_arr[0], $date_arr[2]);
      }

      if ($date_prod && $warehouse && $nom) {
        $import_arr[] = [
          'date' => $date_prod,
          'date_expire' => $date_expire,
          'warehouse' => $warehouse->tid,
          'nom' => $nom->tid,
          'qty' => $load_item['quantity'] ?? 0,
          'price' => $load_item['price'] ?? 0,
        ];
      }
    }

    if ($import_arr) {
      // удалить старые остатки за период
      $date_arr   = explode('.', $file_arr['start_date']);
      $date_start = gmmktime(0, 0, 0, $date_arr[1], $date_arr[0], $date_arr[2]);
      $date_arr   = explode('.', $file_arr['end_date']);
      $date_end   = gmmktime(0, 0, 0, $date_arr[1], $date_arr[0]+1, $date_arr[2]); // включительно
      dash_remains_delete_by_period($date_start, $date_end);

      // создание Плана на месяц
      foreach ($import_arr as $plan_item) {
        $record = entity_create('remains', [
          'type' => 'remains',
          'created' => $import_time,
        ]);
        entity_save('remains', $record);

        $remains_wr = entity_metadata_wrapper('remains', $record);
        $remains_wr->field_date = $plan_item['date'];
        $remains_wr->field_date_expiration = $plan_item['date_expire'] ?: null;
        $remains_wr->field_warehouse_td = $plan_item['warehouse'];
        $remains_wr->field_nomenklatura_td = $plan_item['nom'];
        $remains_wr->field_quantity_td = $plan_item['qty'];
        $remains_wr->field_price = $plan_item['price'];
        $remains_wr->save();
      }

      watchdog('import', 'Остатки ТД импортированы.', [], WATCHDOG_INFO);
      return $import_arr;
    }
  } else {
    watchdog('import', 'Импорт остатков ТД не выполнен.', [], WATCHDOG_ERROR);
    return false;
  }
}
