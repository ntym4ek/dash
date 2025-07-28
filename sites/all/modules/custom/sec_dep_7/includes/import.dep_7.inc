<?php

function dep_7_import_cultures($array)
{
  $voc = taxonomy_vocabulary_machine_name_load('cultures');

  if (is_array($array) && !empty($array['load'])) {
    $new_records = [];
    foreach($array['load'] as $culture) {
      if ($culture['name'] && $culture['id']) {
        // проверить наличие записи Номенклатуры
        $term = dash_term_get_by_1c_id('cultures', $culture['id']);

        // обновить значения полей
        $term_wr = entity_metadata_wrapper('taxonomy_term', $term);
        $term_wr->name = trim($culture['name']);
        $term_wr->field_1c_id = trim($culture['id']);
        $term_wr->field_status = 1;
        $term_wr->save();

        $new_records[] = $term_wr->getIdentifier();
      }
    }

    // если полное обновление справочника, то запомнить отсутствующие записи и сделать их неактивными для клиента
    if ($array['reload']) {
      $dbr = db_select('taxonomy_term_data', 't');
      $dbr->condition('t.vid', $voc->vid);
      $dbr->addField('t', 'tid');
      $cultures_old = $dbr->execute()->fetchAllAssoc('tid');

      foreach ($cultures_old as $tid => $culture_old) {
        if (!in_array($tid, $new_records)) {
          $term_wr = entity_metadata_wrapper('taxonomy_term', $tid);
          $term_wr->field_status = 0;
          $term_wr->save();
        }
      }
    }

    // drupal_set_message('Справочник Культур импортирован.');
    watchdog('import', 'Справочник ' . $voc->name . ' импортирован.', [], WATCHDOG_INFO);
    return true;
  } else {
    // drupal_set_message('Импорт справочника Культур не выполнен.', 'warning');
    watchdog('import', 'Импорт справочника ' . $voc->name . ' не выполнен.', [], WATCHDOG_ERROR);
    return false;
  }
}
function dep_7_import_registrations($array)
{
  if (is_array($array) && !empty($array['load'])) {

    // массив локальных ID Культур
    foreach ($array['load'] as $reg_item) {
      $cultures = [];
      if ($reg_item['cultures']) {
        foreach (explode(',', $reg_item['cultures']) as $culture_item) {
          $culture = dash_term_get_by_1c_id('cultures', trim($culture_item));
          if ($culture) {
            $cultures[] = $culture->tid;
          } else {
            watchdog('import', 'Ошибка при импорте Регистраций. Не найдена культура ' . $culture_item, [], WATCHDOG_WARNING);
          }
        }
      }

      // массив локальных ID Номенклатуры
      $noms = [];
      if ($reg_item['art']) {
        foreach (explode(',', $reg_item['art']) as $nom_artikul) {
          // todo переписать под id номенклатуры вместо артикула
          $nom = dash_nomenklatura_get_by_1c_artikul(DASH_INN_TD, trim($nom_artikul));
          if ($nom) {
            $noms[] = $nom->tid;
          }
        }
      }
      if (!$noms) {
        watchdog('import', 'Ошибка при импорте Регистраций. Не задана ни одна номенклатура ' . $reg_item['art'], [], WATCHDOG_WARNING);
        continue;
      }

      $base_reg = dash_registration_get_by_1c_id($reg_item['base_id']);

      $date_from = $date_to = null;
      if (trim($reg_item['date_from'])) {
        $date_from_arr = explode('.', $reg_item['date_from']);
        $date_from = gmmktime(0, 0, 0, $date_from_arr[1], $date_from_arr[0], $date_from_arr[2]);
      }
      if (trim($reg_item["date_to"])) {
        $date_to_arr = explode('.', $reg_item['date_to']);
        $date_to = gmmktime(0, 0, 0, $date_to_arr[1], $date_to_arr[0], $date_to_arr[2]);
      }

      $reg = dash_registration_get_by_1c_id($reg_item['id']);

      if ($reg) {
        $reg_wr = entity_metadata_wrapper('registration', $reg);
        $reg_wr->field_r_cert_number = $reg_item['cert_num'];
        $reg_wr->field_r_number = $reg_item['reg_num'];
        $reg_wr->field_date_range = ['value' => $date_from, 'value2' => $date_to];
        $reg_wr->field_nomenklaturas_td = $noms;
        $reg_wr->field_cultures = $cultures;
        $reg_wr->field_1c_id = $reg_item['id'];
        $reg_wr->field_r_base_cert = $base_reg ? $base_reg->id : null;
        $reg_wr->field_status = $reg_item['status'];
        $reg_wr->field_r_type = $reg_item['type'];
        $reg_wr->save();
      }
    }

    watchdog('import', 'Список Регистраций импортирован.', [], WATCHDOG_INFO);
    // drupal_set_message('План отгрузки ТД импортирован.');
    return true;
  } else {
    watchdog('import', 'Импорт списка Регистраций не выполнен.', [], WATCHDOG_ERROR);
    return false;
  }
}
