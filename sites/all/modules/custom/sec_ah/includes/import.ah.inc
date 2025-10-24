<?php


function ah_import_nomenklatura($array)
{
  if (is_array($array) && !empty($array['load'])) {
    $vid = taxonomy_vocabulary_machine_name_load('nomenklatura')->vid;

    $new_records = [];
    foreach($array['load'] as $nom) {
      if ($nom['name'] && $nom['id'] && $nom['art'] && $nom['dav_id']) {
        // найти пользователя
        $client = dash_client_get_by_1c_id(DASH_INN_AH, $nom['dav_id']);
        $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);
        if (isset($client->tid)) {
          // проверить наличие записи Номенклатуры по артикулу (тк не у всех прописан ID)
          $term = dash_nomenklatura_get_by_1c_artikul(DASH_INN_AH, $nom['art']);
          // если нет по артикулу, искать по ID или создать
          if (!$term) {
            $term = dash_nomenclature_get_by_1c_id(DASH_INN_AH, $nom['id']);
          }

          if ($term) {
            // обновить значения полей
            $term_wr = entity_metadata_wrapper('taxonomy_term', $term);
            $term_wr->name = $nom['name'];
            $term_wr->field_1c_id = trim($nom['id']);
            $term_wr->field_nom_artikul = trim($nom['art']);
            if (isset($nom['ei'])) $term_wr->field_nom_unit = $nom['ei'];
            $term_wr->field_nom_type = $nom['vid'];
            $term_wr->field_nom_client = $client->tid;
            $term_wr->field_kccc_company = $company->tid;
            $term_wr->field_status = 1;
            $term_wr->save();

            $new_records[] = $term_wr->getIdentifier();
          }
        } else {
          // drupal_set_message('Не найден давалец с ID: ' . $nom['dav_id'], 'error');
          watchdog('import', 'Импорт номенклатуры не выполнен. Не найден давалец с ID: ' . $nom['dav_id'], [], WATCHDOG_ERROR);
          return false;
        }
      }
    }

    // если полное обновление справочника, то запомнить отсутствующие записи и сделать их неактивными для клиента
    if ($array['reload']) {
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

    // drupal_set_message('Номенклатура импортирована.');
    watchdog('import', 'Номенклатура импортирована.', [], WATCHDOG_INFO);
    return true;
  } else {
    // drupal_set_message('Импорт номенклатуры не выполнен.', 'warning');
    watchdog('import', 'Импорт номенклатуры не выполнен.', [], WATCHDOG_ERROR);
    return false;
  }
}

/**
 * -- функции импорта данных
 */
function ah_import_production_plan($array)
{
  if (is_array($array) && !empty($array['load'])) {
    $month_arr = explode('.', $array['month']);
    $month_start = gmmktime(0, 0, 0, $month_arr[1], 1, $month_arr[2]);
    $is_fact = !empty($array['fact']) ? 1 : 0;
    $plan_import_time = time();

    // подготовка Плана на месяц
    $plan = [];
    foreach ($array['load'] as $plan_item) {
      // todo перейти на поиск по nom_id
      $nom = dash_nomenklatura_get_by_1c_artikul(DASH_INN_AH, $plan_item['artikul_1c']);
      if (isset($nom->tid)) {
        $date_arr = explode('.', $plan_item['date']);
        $date_start = gmmktime(0, 0, 0, $date_arr[1], $date_arr[0], $date_arr[2]);

        $plan[] = [
          'date' => $date_start,
          'nom' => $nom->tid,
          'output' => $plan_item['output'],
          'is_fact' => $is_fact
        ];
      } else {
        watchdog('import', 'Ошибка при импорте плана. Не найдена номенклатура ' . $plan_item['artikul_1c'], [], WATCHDOG_WARNING);
      }
    }

    if ($plan) {
      // удалить старый план на месяц
      ah_plan_delete_by_month($month_start, $is_fact);

      // создание Плана на месяц
      foreach ($plan as $date => $plan_item) {
        $plan = entity_create('plan', [
          'type' => 'plan',
          'created' => $plan_import_time,
        ]);
        entity_save('plan', $plan);

        $plan_wr = entity_metadata_wrapper('plan', $plan);
        $plan_wr->field_date = $plan_item['date'];
        $plan_wr->field_nomenklatura = $plan_item['nom'];
        $plan_wr->field_output = $plan_item['output'];
        $plan_wr->field_is_fact = $plan_item['is_fact'];
        $plan_wr->field_is_manual = 0;
        $plan_wr->save();
      }

      watchdog('import', 'План производства АХ импортирован.', [], WATCHDOG_INFO);
      // drupal_set_message('План импортирован.');
      return $plan;
    }
  } else {
    watchdog('import', 'Импорт плана производства АХ не выполнен.', [], WATCHDOG_ERROR);
    return false;
  }
}



/**
 * -- вспомогательные функции
 */

/**
 *  Удалить записи за период, кроме введённых вручную
 * @param $month_start
 * @param $is_fact
 * @return void
 */
function ah_plan_delete_by_month($month_start, $is_fact)
{
  if ($month_start) {
    $next_month_start = gmmktime(0, 0, 0, date('n', $month_start)+1, 1, date('Y', $month_start));

    $efquery = new EntityFieldQuery();
    $efquery
      ->entityCondition('entity_type', 'plan')
      ->entityCondition('bundle', 'plan')
      ->fieldCondition('field_date', 'value', $month_start, '>=')
      ->fieldCondition('field_date', 'value', $next_month_start, '<')
      ->fieldCondition('field_is_fact', 'value', $is_fact)
      ->fieldCondition('field_is_manual', 'value', 1, '!=');

    $result = $efquery->execute();
    if ($result) {
      entity_delete_multiple('plan', array_keys($result['plan']));
    }
  }
}
