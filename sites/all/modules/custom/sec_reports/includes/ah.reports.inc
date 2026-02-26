<?php

function reports_ah_period_to_form($form, &$form_state)
{
  // дефолтное значение: предыдущий месяц
  $month_default    = date('n') - 1;
  $year_default     = date('Y');

  $month          = empty($form_state["values"]["month"])   ? $month_default : $form_state["values"]['month'];
  $year           = empty($form_state["values"]['year'])    ? $year_default : $form_state["values"]['year'];

  // окончание отчётного периода (не включительно)
  $period_to = gmmktime(0, 0, 0, $month + 1, 1, $year);

  // сформировать текстовое представление периода
  $season = dash_season_bounds_by_date_get(1, $period_to);
  $period_text = helper_month_label_ru(date('n', $season['from']));
  if (date('n', $season['from']) != date('n', $season['to']-1)) {
    $period_text .= '-' . helper_month_label_ru(date('n', $season['to']-1));
  }
  $period_text .= ' ' . date('Y', $season['from']) . ' года';
  $period_text = drupal_strtolower($period_text);

  $form = [
    '#prefix' => '<div id="period-to-form-wrapper">',
    '#suffix' => '</div>',
    'filters' => [
      '#type' => 'container',
      '#suffix' => '<div class="text-notes">Отчёт составлен за период: ' . $period_text . ' (если не указано иное)</div>',
      '#attributes' => ['class' => ['filters']],
    ],
  ];

  $month_opts = [];
  for($i = 1; $i <= 12; $i++) {
    $month_opts[$i] = helper_month_label_ru($i);
  }
  $form['filters']['month'] = [
    '#title' => 'Месяц',
    '#type' => 'select',
    '#options' => $month_opts,
    '#default_value' => [$month],
    '#ajax' => [
      'callback' => 'reports_ah_period_to_form_callback',
      'wrapper' => 'period-to-form-wrapper',
    ],
    '#prefix' => '<div class="row"><div class="col-xs-12 col-md-4"><div class="filter"><div class="month-select-wrapper">',
  ];
  $year_opts = [];
  for($i = -1; $i <= 1; $i++) {
    $yr = date('Y') + $i;
    $year_opts[$yr] = $yr;
  }
  $form['filters']['year'] = [
    '#title' => 'Год',
    '#type' => 'select',
    '#options' => $year_opts,
    '#default_value' => [$year],
    '#ajax' => [
      'callback' => 'reports_ah_period_to_form_callback',
      'wrapper' => 'period-to-form-wrapper',
    ],
    '#suffix' => '</div></div></div></div>',
  ];

  $form['content'] = [
    '#markup' => reports_ah_page_content($period_to)
  ];

  return $form;
}
function reports_ah_period_to_form_callback($form, &$form_state)
{
  return $form;
}

function reports_ah_page_content($period_to)
{
  $season = dash_season_bounds_by_date_get(1, $period_to);

  // слайд 1
  $output  =  '<div class="section-title">' .
                '<h3>Динамика производства продукции по направлениям (в ' . DASH_PROD_SUFFIX . ')</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12 col-lg-8">';
  $output .=     '<div class="ajax-load" data-func="ah_block_011" data-delay="0"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .=   '<div class="col-xs-12 col-lg-4">';
  $output .=     '<div class="ajax-load" data-func="ah_block_012" data-delay="0"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 2
  $output .=  '<div class="section-title">' .
                '<h3>План/факт производства продукции и оказания услуг (в ' . DASH_PROD_SUFFIX . ')</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12">';
  $output .=     '<div class="ajax-load" data-func="ah_block_021" data-delay="100"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .=   '<div class="col-xs-12">';
  $output .=     '<div class="ajax-load" data-func="ah_block_022" data-delay="120"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 3
  $url = url('/data/manual-production-by-directions/edit', ['query' => ['blocks' => ['ah_block_030'], 'inn' => DASH_INN_AH]]);
  $output .=  '<div class="section-title">' .
                '<h3>План-факт производства продукции и оказания услуг (в шт.)</h3>' .
                '<a class="edit-link autodialog" data-dialog-width="550" href="' . $url . '">Редактировать данные</a>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12 col-lg-8">';
  $output .=     '<h5 class="chart-title">За ' . helper_month_label_ru(date('n', $season['to'])-1) . '</h5>';
  $output .=     '<div class="ajax-load" data-func="ah_block_031" data-delay="300"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .=   '<div class="col-xs-12 col-lg-4">';
  $output .=     '<div class="ajax-load" data-func="ah_block_032" data-delay="310"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 4
  $url = url('/data/cash-income/edit', ['query' => ['blocks' => ['ah_block_040'], 'inn' => DASH_INN_AH, 'type' => DASH_TYPE_PLAN]]);
  $output .=  '<div class="section-title">' .
                '<h3>План-факт поступления денежных средств (в ' . DASH_REV_SUFFIX . ')</h3>' .
                '<a class="edit-link autodialog" data-dialog-width="550" href="' . $url . '">Редактировать данные</a>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .=  '<div class="row">';
  $output .=    '<div class="col-xs-12">';
  $output .=      '<div class="ajax-load" data-func="ah_block_040" data-delay="400"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=     '</div>';
  $output .=  '</div>';

  // слайд 6
  $output .=  '<div class="section-title">' .
                '<h3>Динамика поступления денежных средств (в ' . DASH_REV_SUFFIX . ' с НДС)</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12 col-lg-8">';
  $output .=     '<div class="ajax-load" data-func="ah_block_061" data-delay="500"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .=   '<div class="col-xs-12 col-lg-4">';
  $output .=     '<div class="ajax-load" data-func="ah_block_062" data-delay="550"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 5
  $output .=  '<div class="section-title">' .
                '<h3>План-факт. Бюджет движения денежных средств (в тыс. руб. с НДС)</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12">';
  $output .=     '<h5 class="chart-title">На ' . helper_month_label_ru(date('n', $season['to'])-1) . '</h5>';
  $output .=     '<div class="ajax-load" data-func="ah_block_5" data-delay="500"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 7
  $url = url('/data/ship/ship/edit', ['query' => ['blocks' => ['ah_block_070'], 'inn' => DASH_INN_AH, 'type' => DASH_TYPE_PLAN]]);
  $output .=  '<div class="section-title">' .
                '<h3>План-факт отгрузки продукции (в ' . DASH_REV_SUFFIX . ')</h3>' .
                '<a class="edit-link autodialog" data-dialog-width="550" href="' . $url . '">Редактировать данные</a>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="ajax-load" data-func="ah_block_070" data-delay="200"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';


  // слайд 8
  $output .=  '<div class="section-title">' .
                '<h3>Динамика отгрузки продукции и оказания услуг (в ' . DASH_REV_SUFFIX . ' с НДС)</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12 col-lg-8">';
  $output .=     '<div class="ajax-load" data-func="ah_block_081" data-delay="700"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .=   '<div class="col-xs-12 col-lg-4">';
  $output .=     '<div class="ajax-load" data-func="ah_block_082" data-delay="750"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 10
  $output .=  '<div class="section-title">' .
                '<h3>Динамика дебиторской задолженности покупателей (в ' . DASH_REV_SUFFIX . ' с НДС)</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12 col-lg-8">';
  $output .=     '<div class="ajax-load" data-func="ah_block_101" data-delay="800"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .=   '<div class="col-xs-12 col-lg-4">';
  $output .=     '<div class="ajax-load" data-func="ah_block_102" data-delay="850"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 12
  $output .=  '<div class="section-title">' .
                '<h3>Динамика кредиторской задолженности покупателей (в ' . DASH_REV_SUFFIX . ' с НДС)</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12 col-lg-8">';
  $output .=     '<div class="ajax-load" data-func="ah_block_121" data-delay="1000"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .=   '<div class="col-xs-12 col-lg-4">';
  $output .=     '<div class="ajax-load" data-func="ah_block_122" data-delay="1050"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 9
  $output .=  '<div class="section-title">' .
                '<h3>Кредиты и займы (в ' . DASH_REV_SUFFIX . ')</h3>' .
               '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12 col-lg-8">';
  $output .=     '<div class="ajax-load" data-func="ah_block_091" data-delay="1500"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .=   '<div class="col-xs-12 col-lg-4">';
  $output .=     '<div class="ajax-load" data-func="ah_block_092" data-delay="1500"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 20
  $url = url('/data/manual-bdr/edit', ['query' => ['blocks' => ['ah_block_200'], 'inn' => DASH_INN_AH]]);
  $output .=  '<div class="section-title">' .
                '<h3>План-факт. Бюджет доходов и расходов (в ' . DASH_REV_SUFFIX . ' без НДС)</h3>' .
                '<a class="edit-link autodialog" data-dialog-width="550" href="' . $url . '">Редактировать данные</a>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12">';
  $output .=     '<h5 class="chart-title">За ' . helper_month_label_ru(date('n', $season['from'])) . (date('n', $season['to']) != 3 ?  '-' . helper_month_label_ru(date('n', $season['to'])-2) : '') . '</h5>';
  $output .=     '<div class="ajax-load" data-func="ah_block_200" data-delay="1700"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';


  return '<div class="charts" data-module="reports" data-period-from="' . $season['from'] . '" data-period-to="' . $season['to'] . '" data-period-end="' . $season['end'] . '">' . $output . '</div>';
}

function reports_ah_block_011()
{
  // период с начала отчётного года по начало заданного месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  // факт производства продукции по группам
  return reports_ah_production_fact_table($period);
}

function reports_ah_block_012()
{
  // период с начала года по начало заданного месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  // факт производства продукции по группам
  $data_arr = reports_ah_production_fact_line($period);

  return drupal_render($data_arr);
}

function reports_ah_block_021()
{
  // период с начала года по начало заданного месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  // факт производства продукции по группам
  $html = reports_ah_production_plan_fact_table($period);

  return $html;
}

function reports_ah_block_022()
{
  // период с начала года по начало заданного месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  // факт производства продукции по группам
  $data_arr = reports_ah_production_plan_fact_column($period);

  return drupal_render($data_arr);
}

function reports_ah_block_031()
{
  // за предыдущий заданному месяц
  $period = [
    'from' => !empty($_GET['period_to']) ? gmmktime(0, 0, 0, date('n', $_GET['period_to'])-1, 1, date('Y', $_GET['period_to'])) : gmmktime(0, 0, 0, date('n')-1, 1, date('Y')),
    'to' =>   !empty($_GET['period_to']) ? gmmktime(0, 0, 0, date('n', $_GET['period_to']), 1, date('Y', $_GET['period_to'])) : gmmktime(0, 0, 0, date('n'), 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  return reports_ah_manual_production_by_directions_table_html($period);
}
function reports_ah_block_032()
{
  // за предыдущий заданному месяц
  $period = [
    'from' => !empty($_GET['period_to']) ? gmmktime(0, 0, 0, date('n', $_GET['period_to'])-1, 1, date('Y', $_GET['period_to'])) : gmmktime(0, 0, 0, date('n')-1, 1, date('Y')),
    'to' =>   !empty($_GET['period_to']) ? gmmktime(0, 0, 0, date('n', $_GET['period_to']), 1, date('Y', $_GET['period_to'])) : gmmktime(0, 0, 0, date('n'), 1, date('Y')),
    'end' =>  gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  // факт производства продукции по группам
  $data_arr = reports_ah_manual_production_by_directions_column($period);

  return drupal_render($data_arr);
}

function reports_ah_block_040()
{
  // период с начала года по начало заданного месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  // план-факт выручки помесячно линейный
  $data_arr = reports_ah_line_revenue_plan_fact($period);

  return drupal_render($data_arr);
}


function reports_ah_block_061()
{
  // период с начала года по начало заданного месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  // факт производства продукции по группам
  return reports_ah_cash_income_fact_table_html($period);
}
function reports_ah_block_062()
{
  // период с начала года по начало заданного месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  // факт производства продукции по группам
  $data_arr = reports_ah_cash_income_fact_column($period);

  return drupal_render($data_arr);
}
function reports_ah_block_5()
{
  // период с начала года по начало текущего месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  return reports_ah_bdds_table_output($period);
}
function reports_ah_block_070()
{
  // период с начала года по начало заданного месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  // план-факт отгрузки продукции
  $data_arr = reports_ah_line_ship_plan_fact($period);

  return drupal_render($data_arr);
}
function reports_ah_block_081()
{
  // период с начала года по начало заданного месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  // факт производства продукции по группам
  return reports_ah_ship_accounts_fact_table_html($period);
}
function reports_ah_block_082()
{
  // период с начала года по начало заданного месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  // факт производства продукции по группам
  $data_arr = reports_ah_ship_accounts_fact_column($period);

  return drupal_render($data_arr);
}

function reports_ah_block_091()
{
  // период с начала года по начало текущего месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  return reports_debt_credits_loans_table_output($period, DASH_INN_AH);
}
function reports_ah_block_092()
{
  // период с начала года по начало заданного месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  // план-факт отгрузки продукции
  $data_arr = reports_ah_debt_credits_loans_column($period);

  return drupal_render($data_arr);
}

function reports_ah_block_101()
{
  // период с начала года по начало текущего месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  return reports_debt_debitors_table_output($period, DASH_INN_AH);
}
function reports_ah_block_102()
{
  // период с начала года по начало заданного месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  // факт дебиторской задолженности
  $data_arr = reports_ah_debt_debitors_loans_column($period);

  return drupal_render($data_arr);
}

function reports_ah_block_121()
{
  // период с начала года по начало текущего месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  return reports_debt_creditors_table_output($period, DASH_INN_AH);
}
function reports_ah_block_122()
{
  // период с начала года по начало заданного месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  // факт кредиторской задолженности
  $data_arr = reports_ah_debt_creditors_loans_column($period);

  return drupal_render($data_arr);
}


function reports_ah_block_200()
{
  // период с начала года по начало текущего месяца
  $period = dash_season_bounds_by_date_get(1, $_GET['period_to']);

  return reports_ah_manual_bdr_table_output($period);
}

/**
 * ----- Подготовка графиков --------------------------------------------------------------------------------------------
 */
// слайд 1
function reports_ah_production_fact_table($period)
{
  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  // собрать данные периодов с DASH_YEAR_HISTORY_START года
  $data = [];
  $total = [];
  foreach (dash_years_shift_list_get(null, date('n', $period['from'])) as $shift) {
    $period_shifted = dash_period_shifted_get($period, $shift);

    $month_start = gmmktime(0, 0, 0, date('n', $period_shifted['to'])-1, 1, date('Y', $period_shifted['to']));
    $next_month_start = gmmktime(0, 0, 0, date('n', $period_shifted['to']), 1, date('Y', $period_shifted['to']));

    $fact = dash_production_clients_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'period' => $period_shifted]);
    foreach ($fact as $record) {
      // получить группу клиентов
      $client = entity_metadata_wrapper('taxonomy_term', $record->client);
      $group = $client->field_client_group->value() ?: 'Прочее';

      if (!isset($data[$group]['month'][$month_start])) $data[$group]['month'][$month_start] = 0;
      if (!isset($total['month'][$month_start])) $total['month'][$month_start] = 0;

      if ($month_start <= $record->date && $record->date < $next_month_start) {
        $data[$group]['month'][$month_start] += $record->volume;
        $total['month'][$month_start] += $record->volume;
      }

      if (date('n', $month_start) != 1) {
        if (!isset($data[$group]['period'][$period_shifted['from']])) $data[$group]['period'][$period_shifted['from']] = 0;
        if (!isset($total['period'][$period_shifted['from']])) $total['period'][$period_shifted['from']] = 0;

        $data[$group]['period'][$period_shifted['from']] += $record->volume;
        $total['period'][$period_shifted['from']] += $record->volume;
      }
    }
  }

  // переместить Прочее на последнее место
  if (isset($data['Прочее'])) {
    $row = $data['Прочее'];
    unset($data['Прочее']);
    $data['Прочее'] = $row;
  }
  if ($total) $data['Итого'] = $total;

    // формируем таблицу
  $header = $rows = [];
  if ($data) {
    $header = [
      ['data' => 'Партнёры', 'align' => 'center', 'class' => ['td-name']],
    ];
    foreach ($data['Итого']['month'] as $period_start => $volume) {
      $header[] = ['data' => date('m', $period_start) . '<br>' . date('Y', $period_start), 'align' => 'center'];
    }
    if (date('n', $month_start) != 1) {
      foreach ($data['Итого']['period'] as $period_start => $volume) {
        $header[] = ['data' => '01-' . str_pad(date('n', $period['to']) - 1 ?: 12, '2', '0', STR_PAD_LEFT) . '<br>' . date('Y', $period_start), 'align' => 'center'];
      }
    }

    foreach ($data as $key => $group) {
      $row = [$key];
      // проходим по колонкам Итого, так как в самих Группах могут быть пропущены месяцы (нет данных)
      foreach ($data['Итого']['month'] as $key_m => $aux) {
        $row[] = ['data' => helper_number_format(($group['month'][$key_m] ?? 0) / DASH_PROD_DIVIDER ?? 0, 2, ' ', false), 'align' => 'right'];
      }
      if (date('n', $month_start) != 1) {
        // проходим по колонкам Итого, так как в самих Группах может не быть периодов (нет данных по ним)
        foreach ($data['Итого']['period'] as $key_p => $aux) {
          $row[] = ['data' => helper_number_format(($group['period'][$key_p] ?? 0) / DASH_PROD_DIVIDER ?? 0, 2, ' ', false), 'align' => 'right'];
        }
      }
      if ($key == 'Итого') {
        $rows[] = ['data' => $row,'class' => ['tfoot']];
      } else $rows[] = $row;
    }
  }

  $output = theme('table', [
    'header' => $header,
    'rows' => $rows,
    'attributes' => ['class' => ['table', 'table-intersect', 'table--xs']],
    'sticky' => false,
    'empty' => 'нет данных',
  ]);

  return '<div class="production-client-fact-table scrollable">' . $output . '</div>';
}

function reports_ah_production_fact_line($period)
{
  $chart = dash_no_data_chart_get();

  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  // собрать данные периодов с DASH_YEAR_HISTORY_START года
  $total = [];
  foreach (dash_years_shift_list_get(null, date('n', $period['from'])) as $shift) {
    $period_shifted = dash_period_shifted_get($period, $shift);

    $month_start = gmmktime(0, 0, 0, date('n', $period_shifted['to'])-1, 1, date('Y', $period_shifted['to']));
    $next_month_start = gmmktime(0, 0, 0, date('n', $period_shifted['to']), 1, date('Y', $period_shifted['to']));

    $fact = dash_production_clients_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'period' => $period_shifted]);
    foreach ($fact as $record) {
      if (!isset($total[$period_shifted['from']]['month'])) $total[$period_shifted['from']]['month'] = 0;
      if (!isset($total[$period_shifted['from']]['period'])) $total[$period_shifted['from']]['period'] = 0;

      if ($month_start <= $record->date && $record->date < $next_month_start) {
        $total[$period_shifted['from']]['month'] += $record->volume;
      }
      $total[$period_shifted['from']]['period'] += $record->volume;
    }
  }

  if ($total) {
    $chart = array(
      '#type' => 'chart',
      '#chart_type' => 'line',
      '#colors' => [DASH_COLOR_GREEN, DASH_COLOR_BLUE],
      '#font' => 'Montserrat',
      '#height' => '350',
      '#legend_position' => 'top',
      '#raw_options' => [
        'titleTextStyle' => ['bold' => 'true', 'fontSize' => '16'],
        'crosshair' => ['trigger' => 'both', 'orientation' => 'vertical'],
        'annotations' => [
          'datum' => ['stem' => ['color' => 'transparent']],
          'textStyle' => ['fontSize' => 14, 'bold' => 'true'],
        ],
        'chartArea' => [
          'width' => '90%',
          'left' => 70,
        ],
        'tooltip' => ['isHtml' => true],
      ],
      'xaxis' => [
        '#type' => 'chart_xaxis',
      ],
      'yaxis' => [
        '#type' => 'chart_yaxis',
        '#min' => 0,
      ],
      'month' => [
        '#type' => 'chart_data',
        '#title' => 'За месяц',
        '#raw_options' => [
          'pointSize' => 5,
          'annotations' => ['stem' => ['color' => 'transparent', 'length' => -30]],
        ],
        '#data' => [],
      ],
      'period' => [
        '#type' => 'chart_data',
        '#title' => 'За период',
        '#raw_options' => [
          'pointSize' => 5,
          'annotations' => ['stem' => ['color' => 'transparent', 'length' => 30]],
        ],
        '#data' => [],
      ],
    );

    foreach ($total as $period_start => $item) {
      $chart['xaxis']['#labels'][] = date('Y', $period_start);

      $month_volume = $item['month'] / DASH_PROD_DIVIDER;
      $period_volume = $item['period'] / DASH_PROD_DIVIDER;

      $chart['month']['#data'][] = $month_volume;
      $chart['period']['#data'][] = $period_volume;
    }
  }

  return $chart;
}

// слайд 2
function reports_ah_production_plan_fact_table($period)
{
  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  $data = [];
  $total = [];

  $month_start = gmmktime(0, 0, 0, date('n', $period['to'])-1, 1, date('Y', $period['to']));
  $next_month_start = gmmktime(0, 0, 0, date('n', $period['to']), 1, date('Y', $period['to']));

  $plan = dash_production_clients_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_PLAN, 'period' => $period]);
  $fact = dash_production_clients_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'period' => $period]);

  foreach ($plan as $record) {
    $client = entity_metadata_wrapper('taxonomy_term', $record->client);
    $client_name = $client->label();

    if ($month_start <= $record->date && $record->date < $next_month_start) {
      $data[$client_name]['month']['plan'] = ($data[$client_name]['month']['plan'] ?? 0) + $record->volume;
      $total['month']['plan'] = ($total['month']['plan'] ?? 0) + $record->volume;
    }

    // в январе данные за период не выводим (равны январю)
    if (date('n', $month_start) != 1) {
      $data[$client_name]['period']['plan'] = ($data[$client_name]['period']['plan'] ?? 0) + $record->volume;
      $total['period']['plan'] = ($total['period']['plan'] ?? 0) + $record->volume;
    }
  }

  foreach ($fact as $record) {
    $client = entity_metadata_wrapper('taxonomy_term', $record->client);
    $client_name = $client->label();

    if (!isset($data[$client_name]['month']['fact'])) $data[$client_name]['month']['fact'] = 0;
    if (!isset($total['month']['fact'])) $total['month']['fact'] = 0;

    if ($month_start <= $record->date && $record->date < $next_month_start) {
      $data[$client_name]['month']['fact'] += $record->volume;
      $total['month']['fact'] += $record->volume;
    }

    if (date('n', $month_start) != 1) {
      if (!isset($data[$client_name]['period']['fact'])) $data[$client_name]['period']['fact'] = 0;
      if (!isset($total['period']['fact'])) $total['period']['fact'] = 0;

      $data[$client_name]['period']['fact'] += $record->volume;
      $total['period']['fact'] += $record->volume;
    }
  }

  if ($total) $data['Итого'] = $total;

    // формируем таблицу
  $header = $rows = [];

  if ($data) {
    $header = [
      ['data' => 'Партнёры', 'align' => 'center', 'class' => ['td-name']],
      ['data' => 'План<br>' . date('m', $month_start) . '.' . date('Y', $month_start), 'align' => 'center'],
      ['data' => 'Факт<br>' . date('m', $month_start) . '.' . date('Y', $month_start), 'align' => 'center'],
      ['data' => 'Вып. (%)<br>' . date('m', $month_start) . '.' . date('Y', $month_start), 'align' => 'center'],
      ['data' => 'Разница<br>' . date('m', $month_start) . '.' . date('Y', $month_start), 'align' => 'center'],
    ];

    if (date('n', $month_start) != 1) {
      $header[] = ['data' => 'План<br>' . '01-' . date('m', $month_start) . '.' . date('Y', $month_start), 'align' => 'center'];
      $header[] = ['data' => 'Факт<br>' . '01-' . date('m', $month_start) . '.' . date('Y', $month_start), 'align' => 'center'];
      $header[] = ['data' => 'Вып. (%)<br>' . '01-' . date('m', $month_start) . '.' . date('Y', $month_start), 'align' => 'center'];
      $header[] = ['data' => 'Разница<br>' . '01-' . date('m', $month_start) . '.' . date('Y', $month_start), 'align' => 'center'];
    }

    foreach ($data as $key => $client) {
      $row = [$key];
      $client_month_plan = $client['month']['plan'] ?? 0;
      $client_month_fact = $client['month']['fact'] ?? 0;
      $row[] = ['data' => helper_number_format($client_month_plan / DASH_PROD_DIVIDER, 2, ' ', false), 'align' => 'right'];
      $row[] = ['data' => helper_number_format($client_month_fact / DASH_PROD_DIVIDER, 2, ' ', false), 'align' => 'right'];
      $prc = $client_month_plan ? helper_number_format(($client_month_fact / $client_month_plan * 100), 2, ' ', false) : '-';
      $row[] = ['data' => $prc, 'align' => 'right'];
      $row[] = ['data' => helper_number_format(($client_month_fact - $client_month_plan) / DASH_PROD_DIVIDER, 2, ' ', false), 'align' => 'right'];

      if (date('n', $month_start) != 1) {
        $client_period_plan = $client['period']['plan'] ?? 0;
        $client_period_fact = $client['period']['fact'] ?? 0;

        $row[] = ['data' => helper_number_format($client_period_plan / DASH_PROD_DIVIDER, 2, ' ', false), 'align' => 'right'];
        $row[] = ['data' => helper_number_format($client_period_fact / DASH_PROD_DIVIDER, 2, ' ', false), 'align' => 'right'];
        $prc = $client_period_plan ? helper_number_format(($client_period_fact / $client_period_plan * 100), 2, ' ', false) : '-';
        $row[] = ['data' => $prc, 'align' => 'right'];
        $row[] = ['data' => helper_number_format(($client_period_fact - $client_period_plan) / DASH_PROD_DIVIDER, 2, ' ', false), 'align' => 'right'];
      }

      if ($key == 'Итого') {
        $rows[] = ['data' => $row,'class' => ['tfoot']];
      } else $rows[] = $row;
    }
  }

  $output = theme('table', [
    'header' => $header,
    'rows' => $rows,
    'attributes' => ['class' => ['table', 'table-intersect', 'table--xs']],
    'sticky' => false,
    'empty' => 'нет данных',
  ]);

  return '<div class="production-client-plan-fact-table scrollable">' . $output . '</div>';
}

function reports_ah_production_plan_fact_column($period)
{
  $chart = dash_no_data_chart_get();

  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  $total = [];

  $month_start = gmmktime(0, 0, 0, date('n', $period['to'])-1, 1, date('Y', $period['to']));
  $next_month_start = gmmktime(0, 0, 0, date('n', $period['to']), 1, date('Y', $period['to']));

  $plan = dash_production_clients_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_PLAN, 'period' => $period]);
  $fact = dash_production_clients_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'period' => $period]);

  foreach ($plan as $record) {
    $client = entity_metadata_wrapper('taxonomy_term', $record->client);
    $client_name = $client->label();

    if (!isset($data[$client_name]['month']['plan'])) $data[$client_name]['month']['plan'] = 0;
    if (!isset($data[$client_name]['period']['plan'])) $data[$client_name]['period']['plan'] = 0;
    if (!isset($total['month']['plan'])) $total['month']['plan'] = 0;
    if (!isset($total['period']['plan'])) $total['period']['plan'] = 0;

    if ($month_start <= $record->date && $record->date < $next_month_start) {
      $data[$client_name]['month']['plan'] += $record->volume;
      $total['month']['plan'] += $record->volume;
    }

    $data[$client_name]['period']['plan'] += $record->volume;

    // в январе данные за период не выводим (равны январю)
    if (date('n', $month_start) != 1) {
      $total['period']['plan'] += $record->volume;
    }
  }

  foreach ($fact as $record) {
    if ($month_start <= $record->date && $record->date < $next_month_start) {
      $total['month']['fact'] = ($total['month']['fact'] ?? 0) + $record->volume;
    }

    if (date('n', $month_start) != 1) {
      $total['period']['fact'] = ($total['period']['fact'] ?? 0) + $record->volume;
    }
  }

  if ($total) {
    $chart = array(
      '#type' => 'chart',
      '#chart_type' => 'column',
      '#colors' => ['#ccc', DASH_COLOR_BLUE],
      '#font' => 'Montserrat',
      '#height' => '350',
      '#legend_position' => 'top',
      '#raw_options' => [
        'titleTextStyle' => ['bold' => 'true', 'fontSize' => '16'],
        'crosshair' => ['trigger' => 'both', 'orientation' => 'vertical'],
        'annotations' => [
          'datum' => ['stem' => ['color' => 'transparent']],
          'textStyle' => ['fontSize' => 14, 'bold' => 'true'],
        ],
        'chartArea' => [
          'width' => '90%',
          'left' => 70,
        ],
        'tooltip' => ['isHtml' => true],
        'bar' => ['groupWidth' => '250'],
      ],
      'xaxis' => [
        '#type' => 'chart_xaxis',
      ],
      'yaxis' => [
        '#type' => 'chart_yaxis',
        '#min' => 0,
      ],
      'plan' => [
        '#type' => 'chart_data',
        '#title' => 'План',
        '#data' => [],
      ],
      'fact' => [
        '#type' => 'chart_data',
        '#title' => 'Факт',
        '#data' => [],
      ],
    );

    $chart['xaxis']['#labels'][] = helper_month_label_ru(date('n', $month_start));
    $total_p = $total['month']['plan'] / DASH_PROD_DIVIDER;
    $total_f = $total['month']['fact'] / DASH_PROD_DIVIDER;
    $chart['plan']['#data'][] = $total_p;
    $chart['fact']['#data'][] = $total_f;

    $chart['plan'][] = array(
      '#type' => 'chart_data_item',
      '#raw_options' => [
        'annotation' => $total_p ? number_format($total_p, 0, null, ' ') : null,
      ],
    );
    $chart['fact'][] = array(
      '#type' => 'chart_data_item',
      '#raw_options' => [
        'annotation' => $total_f ? number_format($total_f, 0, null, ' ') : null,
      ],
    );


    if (date('n', $month_start) != 1) {
      $chart['xaxis']['#labels'][] = helper_month_label_ru(date('n', $period['from'])) . '-' . helper_month_label_ru(date('n', $month_start));

      $total_p = $total['period']['plan'] / DASH_PROD_DIVIDER;
      $total_f = $total['period']['fact'] / DASH_PROD_DIVIDER;
      $chart['plan']['#data'][] = $total_p;
      $chart['fact']['#data'][] = $total_f;

      $chart['plan'][] = array(
        '#type' => 'chart_data_item',
        '#raw_options' => [
          'annotation' => $total_p ? number_format($total_p, 0, null, ' ') : null,
        ],
      );
      $chart['fact'][] = array(
        '#type' => 'chart_data_item',
        '#raw_options' => [
          'annotation' => $total_f ? number_format($total_f, 0, null, ' ') : null,
        ],
      );

    }
  }

  return $chart;
}

// слайд 3
function reports_ah_manual_production_by_directions_table_html($period)
{
  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  // собрать данные по Направлениям за месяц
  $data = [];
  $total = ['name' => 'Итого', 'plan_period' => 0, 'fact_period' => 0];[];
  if ($pbd = dash_manual_production_by_directions_get_by_conditions(['company_id' => $company->tid, 'period' => $period])) {
    foreach ($pbd as $record) {
      if (!isset($data[$record->direction_id])) $data[$record->direction_id] = ['name' => $record->direction_name, 'plan_period' => 0, 'fact_period' => 0];
      if ($record->type == DASH_TYPE_PLAN) {
        $data[$record->direction_id]['plan_period'] = $record->volume;
        $total['plan_period'] += $record->volume;
      }
      if ($record->type == DASH_TYPE_FACT) {
        $data[$record->direction_id]['fact_period'] = $record->volume;
        $total['fact_period'] += $record->volume;
      }
    }

    if ($total) $data[] = $total;
  }

  // формируем таблицу
  $header = [
    ['data' => 'Направление', 'align' => 'center', 'class' => ['td-name']],
    ['data' => 'План', 'align' => 'center'],
    ['data' => 'Факт', 'align' => 'center'],
    ['data' => '% вып.', 'align' => 'center'],
  ];

  $rows = [];
  foreach ($data as $item) {
    if ($item['plan_period'] || $item['fact_period']) {

      $row = [
        'data' => [
          $item['name'],
          ['data' => helper_number_format($item['plan_period'] ?? 0, 0, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($item['fact_period'] ?? 0, 0, ' ', false), 'align' => 'right'],
          ['data' => ($item['plan_period'] ? helper_number_format($item['fact_period'] / $item['plan_period'] * 100, 0, ' ', false) . ' %' : '-'), 'align' => 'right'],
        ],
      ];

      if ($item['name'] == 'Итого') {
        $row['class'] = ['tfoot'];
      }
      $rows[] = $row;
    }
  }

  $output = theme('table', [
    'header' => $header,
    'rows' => $rows,
    'attributes' => ['class' => ['table', 'table-intersect']],
    'sticky' => false,
    'empty' => 'нет данных',
  ]);

  return '<div class="production-by-directions-table scrollable">' . $output . '</div>';
}

function reports_ah_manual_production_by_directions_column($period)
{
  $chart = dash_no_data_chart_get();

  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  // собрать данные по Направлениям за месяц

  $total = ['plan_period' => 0, 'fact_period' => 0];
  if ($pbd = dash_manual_production_by_directions_get_by_conditions(['company_id' => $company->tid, 'period' => $period])) {
    foreach ($pbd as $record) {
      if ($record->type == DASH_TYPE_PLAN) {
        $total['plan_period'] += $record->volume;
      }
      if ($record->type == DASH_TYPE_FACT) {
        $total['fact_period'] += $record->volume;
      }
    }
  }

  // собрать все записи за период
  if ($total['plan_period'] || $total['fact_period']) {
    $chart = array(
      '#type' => 'chart',
      '#chart_type' => 'column',
      '#colors' => ['#ccc', DASH_COLOR_BLUE],
      '#font' => 'Montserrat',
      '#height' => '350',
      '#legend_position' => 'top',
      '#raw_options' => [
        'titleTextStyle' => ['bold' => 'true', 'fontSize' => '16'],
        'crosshair' => ['trigger' => 'both', 'orientation' => 'vertical'],
        'annotations' => [
          'datum' => ['stem' => ['color' => 'transparent']],
          'textStyle' => ['fontSize' => 14, 'bold' => 'true'],
        ],
        'chartArea' => [
          'width' => '90%',
          'left' => 70,
        ],
        'tooltip' => ['isHtml' => true],
      ],
      'xaxis' => [
        '#type' => 'chart_xaxis',
      ],
      'yaxis' => [
        '#type' => 'chart_yaxis',
        '#min' => 0,
      ],
      'plan' => [
        '#type' => 'chart_data',
        '#title' => 'План',
        '#data' => [],
      ],
      'fact' => [
        '#type' => 'chart_data',
        '#title' => 'Факт',
        '#data' => [],
      ],
    );

    $chart['xaxis']['#labels'][] = helper_month_label_ru(date('n', $period['from']));

    $plan = (float)$total['plan_period'];
    $fact = (float)$total['fact_period'];

    $chart['plan']['#data'][] = $plan;
    $chart['fact']['#data'][] = $fact;

    $tooltip = '<div style="padding: 8px 12px; min-width:140px;white-space: nowrap;">' .
      '<div><div style="display:inline-block; font-weight: 700;">' . helper_month_label_ru(date('n', $period['from'])) . '</div></div>' .
      '<div><div style="display:inline-block; width: 210px;">План:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($plan, 2, ',', ' ') . '</b></div></div>' .
      '<div><div style="display:inline-block; width: 210px;">Факт:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($fact, 2, ',', ' ') . '</b></div></div>' .
      '<div><div style="display:inline-block; width: 210px;">Выполнение:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . ($plan ? number_format($fact/$plan*100, 0, ',', ' ') : '-') . '</b>&nbsp;%</div></div>' .
      '</div>';

    $chart['fact'][] = array(
      '#type' => 'chart_data_item',
      '#title' => $tooltip,
      '#raw_options' => [
        'annotation' => $fact ? number_format($fact, 0, null, ' ') : null,
      ],
    );
    $chart['plan'][] = array(
      '#type' => 'chart_data_item',
      '#title' => $tooltip,
      '#raw_options' => [
        'annotation' => $plan ? number_format($plan, 0, null, ' ') : null,
      ],
    );

  }

  return $chart;
}

// слайд 4
function reports_ah_line_revenue_plan_fact($period)
{
  $chart = dash_no_data_chart_get();

  $plan_bounds = ['from' => $period['from'], 'to' => $period['end']];

  // собрать все записи за период
  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  $plan = dash_cash_income_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_PLAN, 'period' => $plan_bounds, 'manual' => 1]);
  $fact = dash_cash_income_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'period' => $period, 'manual' => 0]);

  $total_plan = $total_fact = 0;
  $last_plan = $last_fact = 0;
  if ($plan || $fact) {
    // сгруппировать помесячно
    $plan_fact_by_month = [];
    foreach (dash_months_list_get($period) as $month) {
      $plan_fact_by_month[$month['start']] = [
        'plan_amount' => 0,
        'fact_amount' => 0,
        'start' => $month['start'],
        'end' => $month['end'],
      ];

      foreach ($plan as $record) {
        if ($record->date >= $month['start'] && $record->date < $month['end']) {
          $plan_fact_by_month[$month['start']]['plan_amount'] += $record->amount;
        }
      }
      foreach ($fact as $record) {
        if ($record->date >= $month['start'] && $record->date < $month['end']) {
          $plan_fact_by_month[$month['start']]['fact_amount'] += $record->amount;
        }
      }
    }
    ksort($plan_fact_by_month);

    $chart = array(
      '#type' => 'chart',
      '#chart_type' => 'line',
      '#font' => 'Montserrat',
      '#height' => '350',
      '#legend_position' => 'top',
      '#raw_options' => [
        'titleTextStyle' => ['bold' => 'true', 'fontSize' => '16'],
        'crosshair' => ['trigger' => 'both', 'orientation' => 'vertical'],
        'annotations' => [
          'datum' => ['stem' => ['color' => 'transparent']],
          'textStyle' => ['fontSize' => 14, 'bold' => 'true'],
        ],
        'chartArea' => [
          'width' => '90%',
          'left' => 70,
        ],
        'tooltip' => ['isHtml' => true],
      ],
      'xaxis' => [
        '#type' => 'chart_xaxis',
      ],
      'yaxis' => [
        '#type' => 'chart_yaxis',
        '#min' => 0,
      ],
      'plan' => [
        '#type' => 'chart_data',
        '#title' => 'План',
        '#raw_options' => [
          'pointSize' => 5,
          'annotations' => ['stem' => ['color' => 'transparent', 'length' => -30]],
        ],
        '#data' => [],
      ],
      'fact' => [
        '#type' => 'chart_data',
        '#title' => 'Факт',
        '#raw_options' => [
          'pointSize' => 5,
          'annotations' => ['stem' => ['color' => 'transparent', 'length' => -30]],
        ],
        '#data' => [],
      ],
    );

    foreach ($plan_fact_by_month as $month) {
      $chart['xaxis']['#labels'][] = helper_month_label_ru(date('n', $month['start']));

      $plan = $month['plan_amount'] / DASH_REV_DIVIDER;
      $fact = $month['fact_amount'] / DASH_REV_DIVIDER;
      $total_plan += $plan;
      $total_fact += $fact;

      // если это текущий период,
      // то считаем и выводим накопительный факт только до месяца предстоящего текущему месяцу
      if ($month['start'] < gmmktime(0, 0, 0, date('n', $period['to']), 1, date('Y', $period['to']))) {
        // запоминаем последнее значение для определения цвета графика
        $last_fact = $total_fact;
        $last_plan = $total_plan;
      } else {
        $total_fact = null;
      }

      $chart['plan']['#data'][] = $total_plan;
      $chart['fact']['#data'][] = $total_fact;

      // запоминаем последнее значение для определения цвета графика
      if (isset($total_fact)) {
        $last_fact = $total_fact;
        $last_plan = $total_plan;
      }

      $tooltip = '<div style="padding: 8px 12px; min-width:140px;white-space: nowrap;">' .
        '<div><div style="display:inline-block; font-weight: 700;">' . helper_month_label_ru(date('n', $month['start'])) . '</div></div>' .
        '<div><div style="display:inline-block; width: 210px;">План:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($plan, 2, ',', ' ') . '</b>&nbsp;млн.</div></div>' .
        '<div><div style="display:inline-block; width: 210px;">Факт:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($fact, 2, ',', ' ') . '</b>&nbsp;млн.</div></div>' .
        '<div><div style="display:inline-block; width: 210px;">Выполнение:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . ($plan ? number_format($fact/$plan*100, 0, ',', ' ') : '-') . '</b>&nbsp;%</div></div>' .
        '<div>&nbsp;</div>' .
        '<div><div style="display:inline-block; font-weight: 700;">Накопительный итог</div></div>' .
        '<div><div style="display:inline-block; width: 210px;">План:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($total_plan, 2, ',', ' ') . '</b>&nbsp;млн.</div></div>' .
        '<div><div style="display:inline-block; width: 210px;">Факт:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($last_fact, 2, ',', ' ') . '</b>&nbsp;млн.</div></div>' .
        '<div><div style="display:inline-block; width: 210px;">Выполнение:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . ($total_plan ? number_format($last_fact/$total_plan*100, 0, ',', ' ') : '-') . '</b>&nbsp;%</div></div>' .
        '</div>';

      $chart['fact'][] = array(
        '#type' => 'chart_data_item',
        '#title' => $tooltip,
        '#raw_options' => [
          'annotation' => $total_fact ? number_format($total_fact, 0, null, ' ') : null,
        ],
      );
      $chart['plan'][] = array(
        '#type' => 'chart_data_item',
        '#title' => $tooltip,
        '#raw_options' => [
          'annotation' => $total_plan ? number_format($total_plan, 0, null, ' ') : null,
        ],
      );
    }
  }

  // определить цвет графика
  if ($last_fact > $last_plan) {
    $chart['#colors'] = ['#ccc', DASH_COLOR_GREEN];
    $chart['fact']['#raw_options']['annotations']['stem']['length'] = 20;
    $chart['plan']['#raw_options']['annotations']['stem']['length'] = -30;
  } else {
    $chart['#colors'] = ['#ccc', DASH_COLOR_DARK_RED];
    $chart['fact']['#raw_options']['annotations']['stem']['length'] = -30;
    $chart['plan']['#raw_options']['annotations']['stem']['length'] = 20;
  }

  $max = max($total_plan, $total_fact, $last_plan, $last_fact);
  $chart['yaxis']['#max'] = $max * 1.3;
  $chart['yaxis']['#min'] = -1 * $max/3.2;

  return $chart;
}

// слайд 6
function reports_ah_cash_income_fact_table_html($period)
{
  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  // собрать данные за 2 года и сгруппировать по направлениям
  $data = [];
  $total = [];
  foreach ([-1, 0] as $shift) {
    $period_shifted = dash_period_shifted_get($period, $shift);

    $month_start = gmmktime(0, 0, 0, date('n', $period_shifted['to'])-1, 1, date('Y', $period_shifted['to']));
    $next_month_start = gmmktime(0, 0, 0, date('n', $period_shifted['to']), 1, date('Y', $period_shifted['to']));

    $fact = dash_cash_income_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'period' => $period_shifted]);
    foreach ($fact as $record) {
      $direction_wr = entity_metadata_wrapper('taxonomy_term', $record->direction);
      $direction_name = $direction_wr->label() ?? 'Прочее';
      if (!isset($data[$direction_name]['month'][$month_start])) $data[$direction_name]['month'][$month_start] = 0;
      if (!isset($data[$direction_name]['period'][$period_shifted['from']])) $data[$direction_name]['period'][$period_shifted['from']] = 0;
      if (!isset($total['month'][$month_start])) $total['month'][$month_start] = 0;
      if (!isset($total['period'][$period_shifted['from']])) $total['period'][$period_shifted['from']] = 0;

      if ($month_start <= $record->date && $record->date < $next_month_start) {
        $data[$direction_name]['month'][$month_start] = ($data[$direction_name]['month'][$month_start] ?? 0) + $record->amount;
        $total['month'][$month_start] += $record->amount;
      }

      $data[$direction_name]['period'][$period_shifted['from']] += $record->amount;

      $total['period'][$period_shifted['from']] += $record->amount;
    }
  }
  // переместить Прочее на последнее место
  if (isset($data['Прочее'])) {
    $row = $data['Прочее'];
    unset($data['Прочее']);
    $data['Прочее'] = $row;
  }
  if ($total) $data['Итого'] = $total;

  // формируем таблицу
  $header = $rows = [];
  if ($data) {
    $header = [
      ['data' => 'Направления', 'align' => 'center', 'class' => ['td-name']],
    ];
    foreach ($data['Итого']['month'] as $period_start => $amount) {
      $header[] = ['data' => helper_month_label_ru(date('n', $period_start)) . ' ' . date('Y', $period_start), 'align' => 'center'];
    }
    if (date('n', $month_start) != 1) {
      foreach ($data['Итого']['period'] as $period_start => $amount) {
        $header[] = ['data' => helper_month_label_ru(date('n', $period['from'])) . '-' . helper_month_label_ru(date('n', $period['to'])-1 ?: 12) . ' ' . date('Y', $period_start), 'align' => 'center'];
      }
    }

    foreach ($data as $direction_name => $item_d) {
      $row = [$direction_name];
      // проходим по колонкам Итого, так как в самих Группах могут быть пропущены месяцы (нет данных)
      foreach ($data['Итого']['month'] as $key_m => $aux) {
        $row[] = ['data' => helper_number_format(($item_d['month'][$key_m] ?? 0) / DASH_REV_DIVIDER, 2, ' ', false), 'align' => 'right'];
      }
      if (date('n', $month_start) != 1) {
        // проходим по колонкам Итого, так как в самих Группах может не быть периодов (нет данных по ним)
        foreach ($data['Итого']['period'] as $key_p => $aux) {
          $row[] = ['data' => helper_number_format(($item_d['period'][$key_p] ?? 0) / DASH_REV_DIVIDER, 2, ' ', false), 'align' => 'right'];
        }
      }

      if ($direction_name == 'Итого') {
        $rows[] = ['data' => $row,'class' => ['tfoot']];
      } else $rows[] = $row;
    }
  }

  $output = theme('table', [
    'header' => $header,
    'rows' => $rows,
    'attributes' => ['class' => ['table', 'table-intersect', 'table--xs']],
    'sticky' => false,
    'empty' => 'нет данных',
  ]);

  return '<div class="cash-income-fact-table scrollable">' . $output . '</div>';
}
function reports_ah_cash_income_fact_column($period)
{
  $chart = dash_no_data_chart_get();

  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  // собрать данные за 2 года
  $total = [];
  foreach ([-1, 0] as $shift) {
    $period_shifted = dash_period_shifted_get($period, $shift);

    $month_start = gmmktime(0, 0, 0, date('n', $period_shifted['to'])-1, 1, date('Y', $period_shifted['to']));
    $next_month_start = gmmktime(0, 0, 0, date('n', $period_shifted['to']), 1, date('Y', $period_shifted['to']));

    $year = date('Y', $period_shifted['from']);

    $fact = dash_cash_income_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'period' => $period_shifted]);
    foreach ($fact as $record) {
      if (!isset($total[$year]['month'])) $total[$year]['month'] = 0;

      if ($month_start <= $record->date && $record->date < $next_month_start) {
        $total[$year]['month'] += $record->amount;
      }

      if (date('n', $month_start) != 1) {
        if (!isset($total[$year]['period'])) $total[$year]['period'] = 0;
        $total[$year]['period'] += $record->amount;
      }
    }
  }

  if ($total) {
    $chart = array(
      '#type' => 'chart',
      '#chart_type' => 'column',
      '#colors' => [DASH_COLOR_LIGHT_BLUE, DASH_COLOR_BLUE],
      '#font' => 'Montserrat',
      '#height' => '350',
      '#legend_position' => 'top',
      '#raw_options' => [
        'titleTextStyle' => ['bold' => 'true', 'fontSize' => '16'],
        'crosshair' => ['trigger' => 'both', 'orientation' => 'vertical'],
        'annotations' => [
          'datum' => ['stem' => ['color' => 'transparent']],
          'textStyle' => ['fontSize' => 14, 'bold' => 'true'],
        ],
        'chartArea' => [
          'width' => '90%',
          'left' => 70,
        ],
        'tooltip' => ['isHtml' => true],
      ],
      'xaxis' => [
        '#type' => 'chart_xaxis',
      ],
      'yaxis' => [
        '#type' => 'chart_yaxis',
        '#min' => 0,
      ],
      'start' => [
        '#type' => 'chart_data',
        '#title' => 'за ' . helper_month_label_ru(date('n', $month_start)),
        '#data' => [],
      ],
    );

    foreach ($total as $year => $record) {
      $chart['xaxis']['#labels'][] = (string)$year;

      $chart['start']['#data'][] = $record['month'] / DASH_REV_DIVIDER;

      $chart['start'][] = array(
        '#type' => 'chart_data_item',
        '#raw_options' => [
          'annotation' => $record['month'] ? number_format($record['month'] / DASH_REV_DIVIDER, 0, null, ' ') : null,
        ],
      );

      if (date('n', $month_start) != 1) {
        if (!isset($chart['current'])) $chart['current'] = [
          '#type' => 'chart_data',
          '#title' => 'за ' . (($period['from'] == $month_start) ? helper_month_label_ru(date('n', $period['from'])) : helper_month_label_ru(date('n', $period['from'])) . '-' . helper_month_label_ru(date('n', $month_start))),
          '#data' => [],
        ];

        $chart['current']['#data'][] = $record['period'] / DASH_REV_DIVIDER;

        $chart['current'][] = array(
          '#type' => 'chart_data_item',
          '#raw_options' => [
            'annotation' => $record['period'] ? number_format($record['period'] / DASH_REV_DIVIDER, 0, null, ' ') : null,
          ],
        );

      }
    }
  }

  return $chart;
}

// слайд 5
function reports_ah_bdds_table_output($period)
{
  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  $prev_month_start = gmmktime(0, 0, 0, date('n', $period['to']) - 1, 1, date('Y', $period['to']));

  $data = [
    'income' => [
      'total' => [
        'plan_month' => 0, 'fact_month' => 0,
        'plan_period' => 0, 'fact_period' => 0,
      ],
    ],
    'outcome' => [
      'total' => [
        'plan_month' => 0, 'fact_month' => 0,
        'plan_period' => 0, 'fact_period' => 0,
      ],
      'categories' => [],
    ],
  ];

  // собрать все записи Прихода ДС за период
  $plan = dash_cash_income_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_PLAN, 'period' => $period, 'manual' => 0]);
  $fact = dash_cash_income_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'period' => $period, 'manual' => 0]);

  // собрать за предыдущий полный месяц и за год
  if ($plan) foreach ($plan as $record) {
    $amount = $record->amount / 1000;
    $data['income']['total']['plan_period'] += $amount;
    if ($prev_month_start <= $record->date && $record->date < $period['to']) {
      $data['income']['total']['plan_month'] += $amount;
    }
  }
  if ($fact) foreach ($fact as $record) {
    $amount = $record->amount / 1000;
    $data['income']['total']['fact_period'] += $amount;
    if ($prev_month_start <= $record->date && $record->date < $period['to']) {
      $data['income']['total']['fact_month'] += $amount;
    }
  }

  // собрать все записи Расхода ДС Торгового Дома за период
  $plan = dash_cash_outcome_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_PLAN, 'period' => $period]);
  $fact = dash_cash_outcome_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'period' => $period]);

  // собрать за предыдущий полный месяц и за год
  if ($plan) foreach ($plan as $record) {
    $amount = $record->amount / 1000;
    $data['outcome']['total']['plan_period'] += $amount;
    if ($prev_month_start <= $record->date && $record->date < $period['to']) $data['outcome']['total']['plan_month'] += $amount;

    // todo выяснить id категорий для вывода и убрать true
    if (true || in_array($record->category, [198576, 198575, 198574, 198573])) {
      if (!isset($data['outcome']['categories'][$record->category])) $data['outcome']['categories'][$record->category] = ['plan_month' => 0, 'fact_month' => 0, 'plan_period' => 0, 'fact_period' => 0];
      $data['outcome']['categories'][$record->category]['plan_period'] += $amount;
      if ($prev_month_start <= $record->date && $record->date < $period['to']) $data['outcome']['categories'][$record->category]['plan_month'] += $amount;
    }
  }
  if ($fact) foreach ($fact as $record) {
    $amount = $record->amount / 1000;
    $data['outcome']['total']['fact_period'] += $amount;
    if ($prev_month_start <= $record->date && $record->date < $period['to']) $data['outcome']['total']['fact_month'] += $amount;

    // todo выяснить id категорий для вывода и убрать true
    if (true || in_array($record->category, [198576, 198575, 198574, 198573])) {
      if (!isset($data['outcome']['categories'][$record->category])) $data['outcome']['categories'][$record->category] = ['plan_month' => 0, 'fact_month' => 0, 'plan_period' => 0, 'fact_period' => 0];
      $data['outcome']['categories'][$record->category]['fact_period'] += $amount;
      if ($prev_month_start <= $record->date && $record->date < $period['to']) $data['outcome']['categories'][$record->category]['fact_month'] += $amount;
    }
  }

  // формируем таблицу
  $header = [
    ['data' => 'Наименование', 'align' => 'center', 'class' => ['td-name']],
    ['data' => 'План (мес.)', 'align' => 'center'],
    ['data' => 'Факт (мес.)', 'align' => 'center'],
    ['data' => '% вып.', 'align' => 'center'],
    ['data' => 'План (год)', 'align' => 'center'],
    ['data' => 'Факт (год)', 'align' => 'center'],
    ['data' => '% вып.', 'align' => 'center'],
  ];

  $rows[] = [
    'data' => [
      '1. Приход (итого)',
      ['data' => helper_number_format($data['income']['total']['plan_month'] ?? 0, 1, ' ', false), 'align' => 'right'],
      ['data' => helper_number_format($data['income']['total']['fact_month'] ?? 0, 1, ' ', false), 'align' => 'right'],
      ['data' => ($data['income']['total']['plan_month'] ? helper_number_format($data['income']['total']['fact_month']/$data['income']['total']['plan_month']*100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
      ['data' => helper_number_format($data['income']['total']['plan_period'] ?? 0, 1, ' ', false), 'align' => 'right'],
      ['data' => helper_number_format($data['income']['total']['fact_period'] ?? 0, 1, ' ', false), 'align' => 'right'],
      ['data' => ($data['income']['total']['plan_period'] ? helper_number_format($data['income']['total']['fact_period']/$data['income']['total']['plan_period']*100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
    ],
  ];

  $rows[] = [
    'data' => [
      '2. Расход (итого), в т.ч.:',
      ['data' => helper_number_format($data['outcome']['total']['plan_month'] ?? 0, 1, ' ', false), 'align' => 'right'],
      ['data' => helper_number_format($data['outcome']['total']['fact_month'] ?? 0, 1, ' ', false), 'align' => 'right'],
      ['data' => ($data['outcome']['total']['plan_month'] ? helper_number_format($data['outcome']['total']['fact_month']/$data['outcome']['total']['plan_month']*100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
      ['data' => helper_number_format($data['outcome']['total']['plan_period'] ?? 0, 1, ' ', false), 'align' => 'right'],
      ['data' => helper_number_format($data['outcome']['total']['fact_period'] ?? 0, 1, ' ', false), 'align' => 'right'],
      ['data' => ($data['outcome']['total']['plan_period'] ? helper_number_format($data['outcome']['total']['fact_period']/$data['outcome']['total']['plan_period']*100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
    ],
  ];

  foreach ($data['outcome']['categories'] as $id => $cat) {
    $term = taxonomy_term_load($id);
    $rows[] = [
      'data' => [
        '&nbsp;&nbsp;- ' . drupal_strtolower($term->name),
        ['data' => helper_number_format($cat['plan_month'] ?? 0, 1, ' ', false), 'align' => 'right'],
        ['data' => helper_number_format($cat['fact_month'] ?? 0, 1, ' ', false), 'align' => 'right'],
        ['data' => ($cat['plan_month'] ? helper_number_format($cat['fact_month']/$cat['plan_month']*100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
        ['data' => helper_number_format($cat['plan_period'] ?? 0, 1, ' ', false), 'align' => 'right'],
        ['data' => helper_number_format($cat['fact_period'] ?? 0, 1, ' ', false), 'align' => 'right'],
        ['data' => ($cat['plan_period'] ? helper_number_format($cat['fact_period']/$cat['plan_period']*100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
      ],
    ];
  }

  $output = theme('table', [
    'header' => $header,
    'rows' => $rows,
    'attributes' => ['class' => ['table', 'table-intersect']],
    'sticky' => false,
    'empty' => 'нет данных',
  ]);

  return '<div class="bdds-table scrollable">' . $output . '</div>';
}

// слайд 7
function reports_ah_line_ship_plan_fact($period)
{
  $chart = dash_no_data_chart_get();

  $plan_bounds = ['from' => $period['from'], 'to' => $period['end']];

  // собрать все записи за период
  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);
  $plan = dash_ship_ship_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_PLAN, 'period' => $plan_bounds, 'manual' => 1]);
  $fact = dash_ship_accounts_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'period' => $period]);

  $total_plan = $total_fact = 0;
  $last_plan = $last_fact = 0;
  if ($plan || $fact) {
    // сгруппировать помесячно
    $plan_fact_by_month = [];
    foreach (dash_months_list_get($period) as $month) {
      $plan_fact_by_month[$month['start']] = [
        'plan_amount' => 0,
        'fact_amount' => 0,
        'start' => $month['start'],
        'end' => $month['end'],
      ];

      foreach ($plan as $record) {
        if ($record->date >= $month['start'] && $record->date < $month['end']) {
          $plan_fact_by_month[$month['start']]['plan_amount'] += $record->amount;
        }
      }
      foreach ($fact as $record) {
        if ($record->date >= $month['start'] && $record->date < $month['end']) {
          $plan_fact_by_month[$month['start']]['fact_amount'] += $record->amount;
        }
      }
    }
    ksort($plan_fact_by_month);

    $chart = array(
      '#type' => 'chart',
      '#chart_type' => 'line',
      '#font' => 'Montserrat',
      '#height' => '350',
      '#legend_position' => 'top',
      '#raw_options' => [
        'titleTextStyle' => ['bold' => 'true', 'fontSize' => '16'],
        'crosshair' => ['trigger' => 'both', 'orientation' => 'vertical'],
        'annotations' => [
          'datum' => ['stem' => ['color' => 'transparent']],
          'textStyle' => ['fontSize' => 14, 'bold' => 'true'],
        ],
        'chartArea' => [
          'width' => '90%',
          'left' => 70,
        ],
        'tooltip' => ['isHtml' => true],
      ],
      'xaxis' => [
        '#type' => 'chart_xaxis',
      ],
      'yaxis' => [
        '#type' => 'chart_yaxis',
        '#min' => 0,
      ],
      'plan' => [
        '#type' => 'chart_data',
        '#title' => 'План',
        '#raw_options' => [
          'pointSize' => 5,
          'annotations' => ['stem' => ['color' => 'transparent', 'length' => -30]],
        ],
        '#data' => [],
      ],
      'fact' => [
        '#type' => 'chart_data',
        '#title' => 'Факт',
        '#raw_options' => [
          'pointSize' => 5,
          'annotations' => ['stem' => ['color' => 'transparent', 'length' => 30]],
        ],
        '#data' => [],
      ],
    );

    foreach ($plan_fact_by_month as $month) {
      $chart['xaxis']['#labels'][] = helper_month_label_ru(date('n', $month['start']));

      $plan = $month['plan_amount'] / DASH_REV_DIVIDER;
      $fact = $month['fact_amount'] / DASH_REV_DIVIDER;
      $total_plan += $plan;
      $total_fact += $fact;

      // если это текущий период,
      // то считаем и выводим накопительный факт только до месяца предстоящего текущему месяцу
      if ($month['start'] < gmmktime(0, 0, 0, date('n', $period['to']), 1, date('Y', $period['to']))) {
        // запоминаем последнее значение для определения цвета графика
        $last_fact = $total_fact;
        $last_plan = $total_plan;
      } else {
        $total_fact = null;
      }

      $chart['plan']['#data'][] = $total_plan;
      $chart['fact']['#data'][] = $total_fact;


      $tooltip =  '<div style="padding: 8px 12px; min-width:140px;white-space: nowrap;">' .
                    '<div><div style="display:inline-block; font-weight: 700;">' . helper_month_label_ru(date('n', $month['start'])) . '</div></div>' .
                    '<div><div style="display:inline-block; width: 210px;">План:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($plan, 2, ',', ' ') . '</b>&nbsp;млн.</div></div>' .
                    '<div><div style="display:inline-block; width: 210px;">Факт:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($fact, 2, ',', ' ') . '</b>&nbsp;млн.</div></div>' .
                    '<div><div style="display:inline-block; width: 210px;">Выполнение:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . ($plan ? number_format($fact/$plan*100, 0, ',', ' ') : '-') . '</b>&nbsp;%</div></div>' .
                    '<div>&nbsp;</div>' .
                    '<div><div style="display:inline-block; font-weight: 700;">Накопительный итог</div></div>' .
                    '<div><div style="display:inline-block; width: 210px;">План:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($total_plan, 2, ',', ' ') . '</b>&nbsp;млн.</div></div>' .
                    '<div><div style="display:inline-block; width: 210px;">Факт:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($last_fact, 2, ',', ' ') . '</b>&nbsp;млн.</div></div>' .
                    '<div><div style="display:inline-block; width: 210px;">Выполнение:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . ($total_plan ? number_format($last_fact/$total_plan*100, 0, ',', ' ') : '-') . '</b>&nbsp;%</div></div>' .
                  '</div>';

      $chart['fact'][] = array(
        '#type' => 'chart_data_item',
        '#title' => $tooltip,
        '#raw_options' => [
          'annotation' => $total_fact ? number_format($total_fact, 0, null, ' ') : null,
        ],
      );
      $chart['plan'][] = array(
        '#type' => 'chart_data_item',
        '#title' => $tooltip,
        '#raw_options' => [
          'annotation' => $total_plan ? number_format($total_plan, 0, null, ' ') : null,
        ],
      );
    }
  }

  // определить цвет графика
  if ($last_fact > $last_plan) {
    $chart['#colors'] = ['#ccc', DASH_COLOR_GREEN];
    $chart['fact']['#raw_options']['annotations']['stem']['length'] = 20;
    $chart['plan']['#raw_options']['annotations']['stem']['length'] = -30;
  } else {
    $chart['#colors'] = ['#ccc', DASH_COLOR_DARK_RED];
    $chart['fact']['#raw_options']['annotations']['stem']['length'] = -30;
    $chart['plan']['#raw_options']['annotations']['stem']['length'] = 20;
  }

  $max = max($total_plan, $total_fact, $last_plan, $last_fact);
  $chart['yaxis']['#max'] = $max * 1.3;
  $chart['yaxis']['#min'] = -1 * $max/2.8;

  return $chart;
}

// слайд 8
function reports_ah_ship_accounts_fact_table_html($period)
{
  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  // собрать данные за 3 года и сгруппировать по направлениям
  $data = [];
  $total = [];
  foreach ([-2, -1, 0] as $shift) {
    $period_shifted = dash_period_shifted_get($period, $shift);

    $month_start = gmmktime(0, 0, 0, date('n', $period_shifted['to'])-1, 1, date('Y', $period_shifted['to']));
    $next_month_start = gmmktime(0, 0, 0, date('n', $period_shifted['to']), 1, date('Y', $period_shifted['to']));

    $fact = dash_ship_accounts_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'period' => $period_shifted]);
    foreach ($fact as $record) {
      $direction_name = $record->direction_name ?? 'Прочее';
      if (!isset($data[$direction_name]['month'][$month_start])) $data[$direction_name]['month'][$month_start] = 0;
      if (!isset($total['month'][$month_start])) $total['month'][$month_start] = 0;

      if ($month_start <= $record->date && $record->date < $next_month_start) {
        $data[$direction_name]['month'][$month_start] = ($data[$direction_name]['month'][$month_start] ?? 0) + $record->amount;
        $total['month'][$month_start] += $record->amount;
      }

      if (date('n', $month_start) != 1) {
        if (!isset($data[$direction_name]['period'][$period_shifted['from']])) $data[$direction_name]['period'][$period_shifted['from']] = 0;
        if (!isset($total['period'][$period_shifted['from']])) $total['period'][$period_shifted['from']] = 0;

        $data[$direction_name]['period'][$period_shifted['from']] += $record->amount;
        $total['period'][$period_shifted['from']] += $record->amount;
      }
    }
  }
  // переместить Прочее на последнее место
  if (isset($data['Прочее'])) {
    $row = $data['Прочее'];
    unset($data['Прочее']);
    $data['Прочее'] = $row;
  }
  if ($total) $data['Итого'] = $total;

  // формируем таблицу
  $header = $rows = [];
  if ($data) {
    $header = [
      ['data' => 'Направления', 'align' => 'center', 'class' => ['td-name']],
    ];
    foreach ($data['Итого']['month'] as $period_start => $amount) {
      $header[] = ['data' => helper_month_label_ru(date('n', $period_start)) . '<br>' . date('Y', $period_start), 'align' => 'center'];
    }
    if (date('n', $month_start) != 1) {
      foreach ($data['Итого']['period'] as $period_start => $amount) {
        $header[] = ['data' => helper_month_label_ru(date('n', $period['from'])) . '-' . helper_month_label_ru(date('n', $period['to'])-1) . '<br>' . date('Y', $period_start), 'align' => 'center'];
      }
    }


    foreach ($data as $direction_name => $item_d) {
      $row = [$direction_name];
      // проходим по колонкам Итого, так как в самих Группах могут быть пропущены месяцы (нет данных)
      foreach ($data['Итого']['month'] as $key_m => $aux) {
        $row[] = ['data' => helper_number_format($item_d['month'][$key_m] / DASH_REV_DIVIDER, 2, ' ', false), 'align' => 'right'];
      }
      if (date('n', $month_start) != 1) {
        // проходим по колонкам Итого, так как в самих Группах может не быть периодов (нет данных по ним)
        foreach ($data['Итого']['period'] as $key_p => $aux) {
          $row[] = ['data' => helper_number_format($item_d['period'][$key_p] / DASH_REV_DIVIDER, 2, ' ', false), 'align' => 'right'];
        }
      }
      if ($direction_name == 'Итого') {
        $rows[] = ['data' => $row,'class' => ['tfoot']];
      } else $rows[] = $row;
    }
  }

  $output = theme('table', [
    'header' => $header,
    'rows' => $rows,
    'attributes' => ['class' => ['table', 'table-intersect', 'table--xs']],
    'sticky' => false,
    'empty' => 'нет данных',
  ]);

  return '<div class="cash-income-fact-table scrollable">' . $output . '</div>';
}
function reports_ah_ship_accounts_fact_column($period)
{
  $chart = dash_no_data_chart_get();

  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  // собрать данные за 3 года
  $total = [];
  foreach ([-2, -1, 0] as $shift) {
    $period_shifted = dash_period_shifted_get($period, $shift);

    $month_start = gmmktime(0, 0, 0, date('n', $period_shifted['to'])-1, 1, date('Y', $period_shifted['to']));
    $next_month_start = gmmktime(0, 0, 0, date('n', $period_shifted['to']), 1, date('Y', $period_shifted['to']));

    $year = date('Y', $period_shifted['from']);

    $fact = dash_ship_accounts_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'period' => $period_shifted]);
    foreach ($fact as $record) {
      if (!isset($total[$year]['month'])) $total[$year]['month'] = 0;

      if ($month_start <= $record->date && $record->date < $next_month_start) {
        $total[$year]['month'] += $record->amount;
      }

      if (date('n', $month_start) != 1) {
        if (!isset($total[$year]['period'])) $total[$year]['period'] = 0;

        $total[$year]['period'] += $record->amount;
      }
    }
  }

  if ($total) {
    $chart = array(
      '#type' => 'chart',
      '#chart_type' => 'column',
      '#colors' => [DASH_COLOR_LIGHT_BLUE, DASH_COLOR_BLUE],
      '#font' => 'Montserrat',
      '#height' => '350',
      '#legend_position' => 'top',
      '#raw_options' => [
        'titleTextStyle' => ['bold' => 'true', 'fontSize' => '16'],
        'crosshair' => ['trigger' => 'both', 'orientation' => 'vertical'],
        'annotations' => [
          'datum' => ['stem' => ['color' => 'transparent']],
          'textStyle' => ['fontSize' => 14, 'bold' => 'true'],
        ],
        'chartArea' => [
          'width' => '90%',
          'left' => 70,
        ],
        'tooltip' => ['isHtml' => true],
      ],
      'xaxis' => [
        '#type' => 'chart_xaxis',
      ],
      'yaxis' => [
        '#type' => 'chart_yaxis',
        '#min' => 0,
      ],
      'start' => [
        '#type' => 'chart_data',
        '#title' => 'за ' . helper_month_label_ru(date('n', $month_start)),
        '#data' => [],
      ],
    );

    foreach ($total as $year => $record) {
      $chart['xaxis']['#labels'][] = (string)$year;

      $chart['start']['#data'][] = $record['month'] / DASH_REV_DIVIDER;

      $chart['start'][] = array(
        '#type' => 'chart_data_item',
        '#raw_options' => [
          'annotation' => $record['month'] ? number_format($record['month'] / DASH_REV_DIVIDER, 0, null, ' ') : null,
        ],
      );

      if (date('n', $month_start) != 1) {
        if (!isset($chart['current'])) $chart['current'] = [
          '#type' => 'chart_data',
          '#title' => 'за ' . (($period['from'] == $month_start) ? helper_month_label_ru(date('n', $period['from'])) : helper_month_label_ru(date('n', $period['from'])) . '-' . helper_month_label_ru(date('n', $month_start))),
          '#data' => [],
        ];

        $chart['current']['#data'][] = $record['period'] / DASH_REV_DIVIDER;

        $chart['current'][] = array(
          '#type' => 'chart_data_item',
          '#raw_options' => [
            'annotation' => $record['period'] ? number_format($record['period'] / DASH_REV_DIVIDER, 0, null, ' ') : null,
          ],
        );

      }
    }
  }

  return $chart;
}

// слайд 10
function reports_ah_debt_debitors_table_output($period)
{
  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  // Данные за месяц передаются в этом же месяце, но отображать их нужно как данные на 1 число следующего месяца,
  // то есть данные за июль выводятся под заголовком 1 августа.
  $month_num = date('n', $period['to'])-1 > 0 ? date('n', $period['to'])-1 : 12;
  $month_year_num = $month_num == 12 ? date('Y', $period['to'])-1 : date('Y', $period['to']);
  $next_month_num = date('n', $period['to']);
  $next_month_year_num = date('Y', $period['to']);

  $prev_year_start = gmmktime(0, 0, 0, 1, 1, date('Y', $period['from'])-1);
  $year_start = $period['from'];

  $prev_year_month_start = gmmktime(0, 0, 0, $month_num, 1, $month_year_num-1);
  $prev_year_next_month_start = gmmktime(0, 0, 0, $next_month_num, 1, $next_month_year_num-1);

  $month_start = gmmktime(0, 0, 0, $month_num, 1, $month_year_num);
  $next_month_start = gmmktime(0, 0, 0, $next_month_num, 1, $next_month_year_num);

  $data = [
    'row1' => [],
    'kccc' => [],
    'row2' => [],
    'row3' => [],
  ];

  // собрать все записи Дебиторской задолженности (DASH_DEBT_KIND_DEBITOR)
  $debts = dash_debt_get_by_conditions([
    'company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'kinds' => [DASH_DEBT_KIND_DEBITOR],
    'period' => ['from' => $prev_year_start, 'to' => $period['to']]
  ]);
  foreach($debts as $debt) {
    if ($debt->category == 1 && strpos(drupal_strtolower($debt->name), 'покупателей') !== false) { $row_key = 'row1'; }
    elseif ($debt->category == 1 && strpos(drupal_strtolower($debt->name), 'юристов') !== false) { $row_key = 'row2'; }
    elseif ($debt->category == 1 && strpos(drupal_strtolower($debt->name), 'вся') !== false) { $row_key = 'row3'; }
    elseif ($debt->category == 2) { $row_key = 'kccc'; }

    $data[$row_key][$debt->name][$debt->date] = $debt->amount / DASH_REV_DIVIDER;
  }

  // формируем таблицу
  $header = [
    ['data' => 'Наименование', 'align' => 'center', 'class' => ['td-name']],
    ['data' => date('d.m.Y', $prev_year_start), 'align' => 'center'],
    ['data' => date('d.m.Y', $prev_year_next_month_start), 'align' => 'center'],
    ['data' => date('d.m.Y', $year_start), 'align' => 'center'],
    ['data' => date('d.m.Y', $next_month_start), 'align' => 'center'],
  ];

  foreach ($data as $key => $row) {
    foreach ($row as $name => $item) {
      $rows[] = [
        'data' => [
          ($key == 'kccc' ? '&nbsp;&nbsp;- ': '') . $name,
          ['data' => helper_number_format($item[$prev_year_start] ?? 0, 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($item[$prev_year_month_start] ?? 0, 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($item[$year_start] ?? 0, 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($item[$month_start] ?? 0, 1, ' ', false), 'align' => 'right'],
        ],
        'class' => [$key == 'row3' ? 'tfoot': ''],
      ];
    }
  }

  $output = theme('table', [
    'header' => $header,
    'rows' => $rows,
    'attributes' => ['class' => ['table', 'table-intersect']],
    'sticky' => false,
    'empty' => 'нет данных',
  ]);

  return '<div class="debt-debitors-table scrollable">' . $output . '</div>';
}
function reports_ah_debt_debitors_loans_column($period)
{
  $chart = dash_no_data_chart_get();

  // собрать все записи за период
  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  $next_month_start = $period['to'];
  $prev_year_next_month_start = gmmktime(0, 0, 0, date('n', $next_month_start), 1, date('Y', $next_month_start) - 1);

  $year_start = $period['from'];
  $prev_year_start = gmmktime(0, 0, 0, 1, 1, date('Y', $period['from'])-1);

  // В отчёте за месяц мы показываем данные на 1 число следующего месяца,
  // то есть период выборки данных нужно увеличить на один месяц вперед.
  $period_to = strtotime('next month', $next_month_start);

  $data = [];

  // собрать все записи Дебиторской задолженности (DASH_DEBT_KIND_DEBITOR)
  $debts  = dash_debt_get_by_conditions([
    'company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'kinds' => [DASH_DEBT_KIND_DEBITOR],
    'period' => ['from' => $prev_year_start, 'to' => $period_to]
  ]);
  foreach($debts as $debt) {
    if ($debt->category == 1 && strpos(drupal_strtolower($debt->name), 'вся') !== false) {
      $data[$debt->date] = $debt->amount / DASH_REV_DIVIDER;
    }
  }

  if ($data) {
    $chart = array(
      '#type' => 'chart',
      '#chart_type' => 'column',
      '#colors' => [DASH_COLOR_LIGHT_BLUE, DASH_COLOR_BLUE],
      '#font' => 'Montserrat',
      '#height' => '350',
      '#legend_position' => 'top',
      '#raw_options' => [
        'titleTextStyle' => ['bold' => 'true', 'fontSize' => '16'],
        'crosshair' => ['trigger' => 'both', 'orientation' => 'vertical'],
        'annotations' => [
          'datum' => ['stem' => ['color' => 'transparent']],
          'textStyle' => ['fontSize' => 14, 'bold' => 'true'],
        ],
        'chartArea' => [
          'width' => '90%',
          'left' => 70,
        ],
        'tooltip' => ['isHtml' => true],
      ],
      'xaxis' => [
        '#type' => 'chart_xaxis',
      ],
      'yaxis' => [
        '#type' => 'chart_yaxis',
        '#min' => 0,
      ],
      'start' => [
        '#type' => 'chart_data',
        '#title' => 'На 01.01',
        '#data' => [],
      ],
      'current' => [
        '#type' => 'chart_data',
        '#title' => 'На 01.' . date('m', $next_month_start),
        '#data' => [],
      ],
    );

    $chart['xaxis']['#labels'][] = date('Y', $prev_year_start);
    $chart['start']['#data'][] = $data[$prev_year_start];
    $chart['current']['#data'][] = $data[$prev_year_next_month_start];

    $chart['start'][] = array(
      '#type' => 'chart_data_item',
      '#raw_options' => [
        'annotation' => $data[$prev_year_start] ? number_format($data[$prev_year_start], 0, null, ' ') : null,
      ],
    );
    $chart['current'][] = array(
      '#type' => 'chart_data_item',
      '#raw_options' => [
        'annotation' => $data[$prev_year_next_month_start] ? number_format($data[$prev_year_next_month_start], 0, null, ' ') : null,
      ],
    );

    $chart['xaxis']['#labels'][] = date('Y', $year_start);
    $chart['start']['#data'][] = $data[$year_start];
    $chart['current']['#data'][] = $data[$next_month_start];

    $chart['start'][] = array(
      '#type' => 'chart_data_item',
      '#raw_options' => [
        'annotation' => $data[$year_start] ? number_format($data[$year_start], 0, null, ' ') : null,
      ],
    );
    $chart['current'][] = array(
      '#type' => 'chart_data_item',
      '#raw_options' => [
        'annotation' => $data[$next_month_start] ? number_format($data[$next_month_start], 0, null, ' ') : null,
      ],
    );

  }

  return $chart;
}

// слайд 12
function reports_ah_debt_creditors_loans_column($period)
{
  $chart = dash_no_data_chart_get();

  // собрать все записи за период
  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  $next_month_start = $period['to'];
  $prev_year_next_month_start = gmmktime(0, 0, 0, date('n', $next_month_start), 1, date('Y', $next_month_start) - 1);

  $year_start = $period['from'];
  $prev_year_start = gmmktime(0, 0, 0, 1, 1, date('Y', $period['from'])-1);

  // В отчёте за месяц мы показываем данные на 1 число следующего месяца,
  // то есть период выборки данных нужно увеличить на один месяц вперед.
  $period_to = strtotime('next month', $next_month_start);

  $data = [];

  // собрать все записи Кредиторской задолженности и Займам
  $credits  = dash_debt_get_by_conditions([
    'company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'kinds' => [DASH_DEBT_KIND_CREDITOR],
    'period' => ['from' => $prev_year_start, 'to' => $period_to]
  ]);
  foreach($credits as $credit) {
    if ($credit->category == 1 && strpos(drupal_strtolower($credit->name), 'вся') !== false) {
      $data[$credit->date] = $credit->amount / DASH_REV_DIVIDER;
    }
  }

  if ($data) {
    $chart = array(
      '#type' => 'chart',
      '#chart_type' => 'column',
      '#colors' => [DASH_COLOR_LIGHT_BLUE, DASH_COLOR_BLUE],
      '#font' => 'Montserrat',
      '#height' => '350',
      '#legend_position' => 'top',
      '#raw_options' => [
        'titleTextStyle' => ['bold' => 'true', 'fontSize' => '16'],
        'crosshair' => ['trigger' => 'both', 'orientation' => 'vertical'],
        'annotations' => [
          'datum' => ['stem' => ['color' => 'transparent']],
          'textStyle' => ['fontSize' => 14, 'bold' => 'true'],
        ],
        'chartArea' => [
          'width' => '90%',
          'left' => 70,
        ],
        'tooltip' => ['isHtml' => true],
      ],
      'xaxis' => [
        '#type' => 'chart_xaxis',
      ],
      'yaxis' => [
        '#type' => 'chart_yaxis',
        '#min' => 0,
      ],
      'start' => [
        '#type' => 'chart_data',
        '#title' => 'На 01.01',
        '#data' => [],
      ],
      'current' => [
        '#type' => 'chart_data',
        '#title' => 'На 01.' . date('m', $next_month_start),
        '#data' => [],
      ],
    );

    $chart['xaxis']['#labels'][] = date('Y', $prev_year_start);
    $chart['start']['#data'][] = $data[$prev_year_start];
    $chart['current']['#data'][] = $data[$prev_year_next_month_start];

    $chart['start'][] = array(
      '#type' => 'chart_data_item',
      '#raw_options' => [
        'annotation' => $data[$prev_year_start] ? number_format($data[$prev_year_start], 0, null, ' ') : null,
      ],
    );
    $chart['current'][] = array(
      '#type' => 'chart_data_item',
      '#raw_options' => [
        'annotation' => $data[$prev_year_next_month_start] ? number_format($data[$prev_year_next_month_start], 0, null, ' ') : null,
      ],
    );

    $chart['xaxis']['#labels'][] = date('Y', $year_start);
    $chart['start']['#data'][] = $data[$year_start];
    $chart['current']['#data'][] = $data[$next_month_start];

    $chart['start'][] = array(
      '#type' => 'chart_data_item',
      '#raw_options' => [
        'annotation' => $data[$year_start] ? number_format($data[$year_start], 0, null, ' ') : null,
      ],
    );
    $chart['current'][] = array(
      '#type' => 'chart_data_item',
      '#raw_options' => [
        'annotation' => $data[$next_month_start] ? number_format($data[$next_month_start], 0, null, ' ') : null,
      ],
    );

  }

  return $chart;
}


// слайд 9
function reports_ah_debt_credits_loans_column($period)
{
  $chart = dash_no_data_chart_get();

  // собрать все записи за период
  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  $next_month_start = $period['to'];
  $prev_year_next_month_start = gmmktime(0, 0, 0, date('n', $next_month_start), 1, date('Y', $next_month_start) - 1);

  $year_start = $period['from'];
  $prev_year_start = gmmktime(0, 0, 0, 1, 1, date('Y', $period['from'])-1);

  // В отчёте за месяц мы показываем данные на 1 число следующего месяца,
  // то есть период выборки данных нужно увеличить на один месяц вперед.
  $period_to = strtotime('next month', $next_month_start);

  $data = [];

  // собрать все записи по всем кредитам (DASH_DEBT_KIND_CREDIT)
  $debts  = dash_debt_get_by_conditions([
    'company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'kinds' => [DASH_DEBT_KIND_CREDIT, DASH_DEBT_KIND_LOAN],
    'period' => ['from' => $prev_year_start, 'to' => $period_to]
  ]);
  foreach($debts as $debt) {
    $data[$debt->date] = ($data[$debt->date] ?? 0) + $debt->amount / DASH_REV_DIVIDER;
  }

  if ($data) {
    $chart = array(
      '#type' => 'chart',
      '#chart_type' => 'column',
      '#colors' => [DASH_COLOR_LIGHT_BLUE, DASH_COLOR_BLUE],
      '#font' => 'Montserrat',
      '#height' => '350',
      '#legend_position' => 'top',
      '#raw_options' => [
        'titleTextStyle' => ['bold' => 'true', 'fontSize' => '16'],
        'crosshair' => ['trigger' => 'both', 'orientation' => 'vertical'],
        'annotations' => [
          'datum' => ['stem' => ['color' => 'transparent']],
          'textStyle' => ['fontSize' => 14, 'bold' => 'true'],
        ],
        'chartArea' => [
          'width' => '90%',
          'left' => 70,
        ],
        'tooltip' => ['isHtml' => true],
      ],
      'xaxis' => [
        '#type' => 'chart_xaxis',
      ],
      'yaxis' => [
        '#type' => 'chart_yaxis',
        '#min' => 0,
      ],
      'start' => [
        '#type' => 'chart_data',
        '#title' => 'На 01.01',
        '#data' => [],
      ],
      'current' => [
        '#type' => 'chart_data',
        '#title' => 'На 01.' . date('m', $next_month_start),
        '#data' => [],
      ],
    );

    $chart['xaxis']['#labels'][] = date('Y', $prev_year_start);
    $chart['start']['#data'][] = $data[$prev_year_start] ?? 0;
    $chart['current']['#data'][] = $data[$prev_year_next_month_start] ?? 0;

    $chart['start'][] = array(
      '#type' => 'chart_data_item',
      '#raw_options' => [
        'annotation' => isset($data[$prev_year_start]) ? number_format($data[$prev_year_start], 0, null, ' ') : null,
      ],
    );
    $chart['current'][] = array(
      '#type' => 'chart_data_item',
      '#raw_options' => [
        'annotation' => isset($data[$prev_year_next_month_start]) ? number_format($data[$prev_year_next_month_start], 0, null, ' ') : null,
      ],
    );

    $chart['xaxis']['#labels'][] = date('Y', $year_start);
    $chart['start']['#data'][] = $data[$year_start] ?? 0;
    $chart['current']['#data'][] = $data[$next_month_start] ?? 0;

    $chart['start'][] = array(
      '#type' => 'chart_data_item',
      '#raw_options' => [
        'annotation' => isset($data[$year_start]) ? number_format($data[$year_start], 0, null, ' ') : null,
      ],
    );
    $chart['current'][] = array(
      '#type' => 'chart_data_item',
      '#raw_options' => [
        'annotation' => isset($data[$next_month_start]) ? number_format($data[$next_month_start], 0, null, ' ') : null,
      ],
    );
  }

  return $chart;
}

// слайд 20
/**
 * Данные для этой таблицы должны заноситься накопительным итогом
 * (в отличие от таблицы для ТД, для которой Бобков заносит данные в итоговыми значениями на месяц).
 *
 *
 * Нужно учитывать, что данные за месяц вносятся в конце следующего месяца.
 * Например, в таблице "План-факт. Бюджет доходов и расходов" (block_13)
 * при выводе данных на начало октября нужно выводить по август.
 */
function reports_ah_manual_bdr_table_output($period)
{
  // Данные выводятся за месяц, предшествующий отчётному на -2,
  // например, если отчётный месяц Февраль, то данные выводятся за Декабрь.
  $month_num = date('n', $period['to']);
  $month_start = gmmktime(0, 0, 0, $month_num-2, 1, date('Y', $period['to']));

  // переопределить Отчётный период,
  // так как, например, для Февраля период, за который нужно выбирать данные - прошлый год
  $period_to_month_start = gmmktime(0, 0, 0, $month_num-1, 1, date('Y', $period['to']));
  $period = dash_season_bounds_by_date_get(1, $period_to_month_start);

  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  $data = [
    'income' => [
      'total' => [
        'plan_month' => 0, 'fact_month' => 0,
        'plan_period' => 0, 'fact_period' => 0,
      ],
    ],
    'outcome' => [
      'total' => [
        'plan_month' => 0, 'fact_month' => 0,
        'plan_period' => 0, 'fact_period' => 0,
      ],
      'categories' => [],
    ],
  ];

  // По родительскому термину Доходы определить список id терминов потомков
  $income_term_ids = dash_term_siblings_get(227895);

  // собрать доходы, входящие в подкатегории Доходы
  $plan_income = dash_manual_bdr_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_PLAN, 'period' => $period, 'category_ids' => $income_term_ids, 'manual' => 1]);
  $fact_income = dash_manual_bdr_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'period' => $period, 'category_ids' => $income_term_ids, 'manual' => 1]);

  // собрать за предыдущий полный месяц и за год
  if ($plan_income) foreach ($plan_income as $record) {
    $data['income']['total']['plan_period'] += $record->amount;
    if ($month_start <= $record->date && $record->date < $period['to']) $data['income']['total']['plan_month'] += $record->amount;

    if (!isset($data['income']['categories'][$record->category_id])) $data['income']['categories'][$record->category_id] = ['name' => $record->category_name, 'plan_month' => 0, 'fact_month' => 0, 'plan_period' => 0, 'fact_period' => 0];
    $data['income']['categories'][$record->category_id]['plan_period'] += $record->amount;
    if ($month_start <= $record->date && $record->date < $period['to']) $data['income']['categories'][$record->category_id]['plan_month'] += $record->amount;
  }
  if ($fact_income) foreach ($fact_income as $record) {
    $data['income']['total']['fact_period'] += $record->amount;
    if ($month_start <= $record->date && $record->date < $period['to']) $data['income']['total']['fact_month'] += $record->amount;

    if (!isset($data['income']['categories'][$record->category_id])) $data['income']['categories'][$record->category_id] = ['name' => $record->category_name, 'plan_month' => 0, 'fact_month' => 0, 'plan_period' => 0, 'fact_period' => 0];
    $data['income']['categories'][$record->category_id]['fact_period'] += $record->amount;
    if ($month_start <= $record->date && $record->date < $period['to']) $data['income']['categories'][$record->category_id]['fact_month'] += $record->amount;
  }

  // По родительскому термину Расходы определить список id терминов потомков
  $outcome_term_ids = dash_term_siblings_get(1041788);

  // собрать расходы, входящие в подкатегории Расходы
  $plan_outcome = dash_manual_bdr_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_PLAN, 'period' => $period, 'category_ids' => $outcome_term_ids, 'manual' => 1]);
  $fact_outcome = dash_manual_bdr_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'period' => $period, 'category_ids' => $outcome_term_ids, 'manual' => 1]);

  // собрать за предыдущий полный месяц и за год
  if ($plan_outcome) foreach ($plan_outcome as $record) {
    $data['outcome']['total']['plan_period'] += $record->amount;
    if ($month_start <= $record->date && $record->date < $period['to']) $data['outcome']['total']['plan_month'] += $record->amount;

    if (!isset($data['outcome']['categories'][$record->category_id])) $data['outcome']['categories'][$record->category_id] = ['name' => $record->category_name, 'plan_month' => 0, 'fact_month' => 0, 'plan_period' => 0, 'fact_period' => 0];
    $data['outcome']['categories'][$record->category_id]['plan_period'] += $record->amount;
    if ($month_start <= $record->date && $record->date < $period['to']) $data['outcome']['categories'][$record->category_id]['plan_month'] += $record->amount;
  }
  if ($fact_outcome) foreach ($fact_outcome as $record) {
    $data['outcome']['total']['fact_period'] += $record->amount;
    if ($month_start <= $record->date && $record->date < $period['to']) $data['outcome']['total']['fact_month'] += $record->amount;

    if (!isset($data['outcome']['categories'][$record->category_id])) $data['outcome']['categories'][$record->category_id] = ['name' => $record->category_name, 'plan_month' => 0, 'fact_month' => 0, 'plan_period' => 0, 'fact_period' => 0];
    $data['outcome']['categories'][$record->category_id]['fact_period'] += $record->amount;
    if ($month_start <= $record->date && $record->date < $period['to']) $data['outcome']['categories'][$record->category_id]['fact_month'] += $record->amount;
  }

  // формируем таблицу
  $header = [
    ['data' => 'Наименование', 'align' => 'center', 'class' => ['td-name']],
    ['data' => 'План (мес.)', 'align' => 'center'],
    ['data' => 'Факт (мес.)', 'align' => 'center'],
    ['data' => '% вып.', 'align' => 'center'],
    ['data' => 'План (год)', 'align' => 'center'],
    ['data' => 'Факт (год)', 'align' => 'center'],
    ['data' => '% вып.', 'align' => 'center'],
  ];

  $rows = [];
  if ($plan_income || $fact_income) {
    $rows[] = [
      'data' => [
        '1. Доходы (итого), в т.ч.:',
        ['data' => helper_number_format($data['income']['total']['plan_month'] ?? 0, 0, ' ', false), 'align' => 'right'],
        ['data' => helper_number_format($data['income']['total']['fact_month'] ?? 0, 0, ' ', false), 'align' => 'right'],
        ['data' => ($data['income']['total']['plan_month'] ? helper_number_format($data['income']['total']['fact_month'] / $data['income']['total']['plan_month'] * 100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
        ['data' => helper_number_format($data['income']['total']['plan_period'] ?? 0, 0, ' ', false), 'align' => 'right'],
        ['data' => helper_number_format($data['income']['total']['fact_period'] ?? 0, 0, ' ', false), 'align' => 'right'],
        ['data' => ($data['income']['total']['plan_period'] ? helper_number_format($data['income']['total']['fact_period'] / $data['income']['total']['plan_period'] * 100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
      ],
    ];

    foreach ($data['income']['categories'] as $cat) {
      $rows[] = [
        'data' => [
          '&nbsp;&nbsp;- ' . drupal_strtolower($cat['name']),
          ['data' => helper_number_format($cat['plan_month'] ?? 0, 0, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($cat['fact_month'] ?? 0, 0, ' ', false), 'align' => 'right'],
          ['data' => ($cat['plan_month'] ? helper_number_format($cat['fact_month'] / $cat['plan_month'] * 100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
          ['data' => helper_number_format($cat['plan_period'] ?? 0, 0, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($cat['fact_period'] ?? 0, 0, ' ', false), 'align' => 'right'],
          ['data' => ($cat['plan_period'] ? helper_number_format($cat['fact_period'] / $cat['plan_period'] * 100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
        ],
      ];
    }
  }
  if ($plan_outcome || $fact_outcome) {
    $rows[] = [
      'data' => [
        '2. Расходы (итого), в т.ч.:',
        ['data' => helper_number_format($data['outcome']['total']['plan_month'] ?? 0, 0, ' ', false), 'align' => 'right'],
        ['data' => helper_number_format($data['outcome']['total']['fact_month'] ?? 0, 0, ' ', false), 'align' => 'right'],
        ['data' => ($data['outcome']['total']['plan_month'] ? helper_number_format($data['outcome']['total']['fact_month'] / $data['outcome']['total']['plan_month'] * 100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
        ['data' => helper_number_format($data['outcome']['total']['plan_period'] ?? 0, 0, ' ', false), 'align' => 'right'],
        ['data' => helper_number_format($data['outcome']['total']['fact_period'] ?? 0, 0, ' ', false), 'align' => 'right'],
        ['data' => ($data['outcome']['total']['plan_period'] ? helper_number_format($data['outcome']['total']['fact_period'] / $data['outcome']['total']['plan_period'] * 100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
      ],
    ];

    foreach ($data['outcome']['categories'] as $cat) {
      $rows[] = [
        'data' => [
          '&nbsp;&nbsp;- ' . drupal_strtolower($cat['name']),
          ['data' => helper_number_format($cat['plan_month'] ?? 0, 0, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($cat['fact_month'] ?? 0, 0, ' ', false), 'align' => 'right'],
          ['data' => ($cat['plan_month'] ? helper_number_format($cat['fact_month'] / $cat['plan_month'] * 100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
          ['data' => helper_number_format($cat['plan_period'] ?? 0, 0, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($cat['fact_period'] ?? 0, 0, ' ', false), 'align' => 'right'],
          ['data' => ($cat['plan_period'] ? helper_number_format($cat['fact_period'] / $cat['plan_period'] * 100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
        ],
      ];
    }
  }

  $output = theme('table', [
    'header' => $header,
    'rows' => $rows,
    'attributes' => ['class' => ['table', 'table-intersect']],
    'sticky' => false,
    'empty' => 'нет данных',
  ]);

  return '<div class="bdds-table scrollable">' . $output . '</div>';
}
