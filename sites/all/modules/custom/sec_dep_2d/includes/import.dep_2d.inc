<?php

function dep_2d_import_storages($array)
{
  $voc = taxonomy_vocabulary_machine_name_load('warehouses');

  if (is_array($array) && !empty($array['load'])) {

    $new_records = [];
    foreach($array['load'] as $record) {
      if ($record['name'] && $record['id']) {
        // проверить наличие записи в справочнике или создать её
        $term = dash_term_get_by_1c_id('warehouses', $record['id']);

        // обновить значения полей
        $term_wr = entity_metadata_wrapper('taxonomy_term', $term);
        $term_wr->name = trim($record['name']);
        $term_wr->field_status = 1;
        $term_wr->save();

        $new_records[] = $term_wr->getIdentifier();
      }
    }

    // если полное обновление справочника, то запомнить отсутствующие записи и сделать их неактивными для клиента
    if ($array['reload']) {
      db_update('field_data_field_status')
        ->fields(['field_status_value' => 0])
        ->condition('entity_type', 'taxonomy_term')
        ->condition('bundle', 'warehouses')
        ->condition('entity_id', $new_records, 'NOT IN')
        ->execute();
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
      $nom_depr   = dash_nomenclature_depr_get_by_1c_id($load_item['nom_id']);                                    // todo удалить после заполнения поля field_rm_nomenclature
      $nom        = dash_nomenclature_get_by_1c_id(DASH_INN_TD, $load_item['nom_id']);
      $date_prod  = helper_datetime_to_timestamp($load_item['date']);
      $date_expire = '';
      if ($load_item['expiration_date']) {
        $date_expire = helper_datetime_to_timestamp($load_item['expiration_date']);
      }

      if ($date_prod && $warehouse && $nom_depr) {
        $import_arr[] = [
          'date' => $date_prod,
          'date_expire' => $date_expire,
          'warehouse' => $warehouse->tid,
          'nom_depr' => $nom_depr->tid,
          'nom' => $nom->tid,
          'qty' => $load_item['quantity'] ?? 0,
          'price' => $load_item['price'] ?? 0,
        ];
      }
    }

    if ($import_arr) {
      // удалить старые остатки за период
      $date_start = helper_datetime_to_timestamp($file_arr['start_date']);
      $date_arr   = explode('.', $file_arr['end_date']);
      $date_end   = gmmktime(0, 0, 0, $date_arr[1], $date_arr[0]+1, $date_arr[2]); // данные приходят включительно $file_arr['end_date']
      dash_remains_delete_by_period($date_start, $date_end);

      // создание Плана на месяц
      foreach ($import_arr as $plan_item) {
        $record = entity_create('remains', [
          'type' => 'remains',
          'created' => $import_time,
        ]);
        entity_save('remains', $record);

        $remains_wr = entity_metadata_wrapper('remains', $record);
        $remains_wr->field_rm_date = $plan_item['date'];
        $remains_wr->field_rm_date_exp = $plan_item['date_expire'] ?: null;
        $remains_wr->field_rm_warehouse = $plan_item['warehouse'];
        $remains_wr->field_rm_nomenklatura = $plan_item['nom_depr'];
        $remains_wr->field_rm_nomenclature = $plan_item['nom'];
        $remains_wr->field_rm_quantity = $plan_item['qty'];
        $remains_wr->field_rm_price = $plan_item['price'];
        $remains_wr->save();
      }

      watchdog('import', 'Остатки ТД импортированы.', [], WATCHDOG_INFO);
      return $import_arr;
    }
  }

  watchdog('import', 'Импорт остатков ТД не выполнен.', [], WATCHDOG_ERROR);
  return false;
}
