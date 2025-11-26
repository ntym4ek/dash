<?php

function reports_ah_period_to_form($form, &$form_state)
{
  // дефолтные значения
  $month_default    = date('n');
  $year_default     = date('Y');

  $month          = empty($form_state["values"]["month"])   ? $month_default : $form_state["values"]['month'];
  $year           = empty($form_state["values"]['year'])    ? $year_default : $form_state["values"]['year'];

  $month_start = gmmktime(0, 0, 0, $month, 1, $year);

  $form = [
    '#prefix' => '<div id="period-to-form-wrapper">',
    '#suffix' => '</div>',
    'filters' => [
      '#type' => 'container',
      '#suffix' => '<div class="text-notes">Отчёт составлен на начало месяца.</div>',
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
    '#markup' => reports_ah_page_content($month_start)
  ];

  return $form;
}
function reports_ah_period_to_form_callback($form, &$form_state)
{
  return $form;
}

function reports_ah_page_content($period_to)
{
  $season_01 = dash_season_bounds_get_by_date(1, $period_to);

  // слайд 1
  $output  =  '<div class="section-title">' .
                '<h3>Динамика производства продукции по направлениям за период ' . date('Y') . ' (в ' . DASH_PROD_SUFFIX . ')</h3>' .
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
                '<h3>План/факт производства продукции и оказания услуг за ' . date('Y') . ' (в ' . DASH_PROD_SUFFIX . ')</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12">';
  $output .=     '<div class="ajax-load" data-func="ah_block_021" data-delay="0"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .=   '<div class="col-xs-12">';
  $output .=     '<div class="ajax-load" data-func="ah_block_022" data-delay="0"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 6
  $output .=  '<div class="section-title">' .
                '<h3>Динамика поступления денежных средств ' . date('Y') . ' (в ' . DASH_REV_SUFFIX . ' с НДС)</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12 col-lg-8">';
  $output .=     '<div class="ajax-load" data-func="ah_block_061" data-delay="0"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .=   '<div class="col-xs-12 col-lg-4">';
  $output .=     '<div class="ajax-load" data-func="ah_block_062" data-delay="0"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 9
  $output .=  '<div class="section-title">' .
                '<h3>Кредиты и внутренние займы (в ' . DASH_REV_SUFFIX . ')</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12">';
  $output .=     '<div class="ajax-load" data-func="ah_block_090" data-delay="000"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 10
  $output .=  '<div class="section-title">' .
                '<h3>Динамика дебиторской задолженности покупателей (в ' . DASH_REV_SUFFIX . ' с НДС)</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12">';
  $output .=     '<div class="ajax-load" data-func="ah_block_100" data-delay="000"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 12
  $output .=  '<div class="section-title">' .
                '<h3>Динамика кредиторской задолженности покупателей (в ' . DASH_REV_SUFFIX . ' с НДС)</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12">';
  $output .=     '<div class="ajax-load" data-func="ah_block_120" data-delay="000"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';


  return '<div class="charts" data-module="reports" data-period-from="' . $season_01['from'] . '" data-period-to="' . $season_01['to'] . '" data-period-end="' . $season_01['end'] . '">' . $output . '</div>';
}

function reports_ah_block_011()
{
  // период с начала года по начало заданного месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => gmmktime(0, 0, 0, date('n', $_GET['period_to'])+1, 1, date('Y', $_GET['period_to'])) ?? gmmktime(0, 0, 0, date('n')+1, 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  // факт производства продукции по группам
  $html = reports_ah_production_fact_table($period);

  return $html;
}

function reports_ah_block_012()
{
  // период с начала года по начало заданного месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => gmmktime(0, 0, 0, date('n', $_GET['period_to'])+1, 1, date('Y', $_GET['period_to'])) ?? gmmktime(0, 0, 0, date('n')+1, 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  // факт производства продукции по группам
  $data_arr = reports_ah_production_fact_line($period);

  return drupal_render($data_arr);
}

function reports_ah_block_021()
{
  // период с начала года по начало заданного месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => gmmktime(0, 0, 0, date('n', $_GET['period_to'])+1, 1, date('Y', $_GET['period_to'])) ?? gmmktime(0, 0, 0, date('n')+1, 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  // факт производства продукции по группам
  $html = reports_ah_production_plan_fact_table($period);

  return $html;
}

function reports_ah_block_022()
{
  // период с начала года по начало заданного месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => gmmktime(0, 0, 0, date('n', $_GET['period_to'])+1, 1, date('Y', $_GET['period_to'])) ?? gmmktime(0, 0, 0, date('n')+1, 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  // факт производства продукции по группам
  $data_arr = reports_ah_production_plan_fact_column($period);

  return drupal_render($data_arr);
}

function reports_ah_block_061()
{
  // период с начала года по начало заданного месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => gmmktime(0, 0, 0, date('n', $_GET['period_to'])+1, 1, date('Y', $_GET['period_to'])) ?? gmmktime(0, 0, 0, date('n')+1, 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  // факт производства продукции по группам
  $html = reports_ah_cash_income_fact_table($period);

  return $html;
}

function reports_ah_block_062()
{
  // период с начала года по начало заданного месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => gmmktime(0, 0, 0, date('n', $_GET['period_to'])+1, 1, date('Y', $_GET['period_to'])) ?? gmmktime(0, 0, 0, date('n')+1, 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  // факт производства продукции по группам
  $data_arr = reports_ah_cash_income_fact_column($period);

  return drupal_render($data_arr);
}

function reports_ah_block_100()
{
  // период с начала года по начало текущего месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => $_GET['period_to'] ?? gmmktime(0, 0, 0, date('n'), 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  return reports_ah_debt_debitors_table_output($period);
}
function reports_ah_block_120()
{
  // период с начала года по начало текущего месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => $_GET['period_to'] ?? gmmktime(0, 0, 0, date('n')+1, 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  return reports_ah_debt_creditors_table_output($period);
}
function reports_ah_block_090()
{
  // период с начала года по начало текущего месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => $_GET['period_to'] ?? gmmktime(0, 0, 0, date('n')+1, 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  return reports_ah_debt_credits_loans_table_output($period);
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
      if (!isset($data[$group]['period'][$period_shifted['from']])) $data[$group]['period'][$period_shifted['from']] = 0;
      if (!isset($total['month'][$month_start])) $total['month'][$month_start] = 0;
      if (!isset($total['period'][$period_shifted['from']])) $total['period'][$period_shifted['from']] = 0;

      if ($month_start <= $record->date && $record->date < $next_month_start) {
        $data[$group]['month'][$month_start] += $record->volume;
        $total['month'][$month_start] += $record->volume;
      }

      $data[$group]['period'][$period_shifted['from']] += $record->volume;

      $total['period'][$period_shifted['from']] += $record->volume;
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
      $header[] = ['data' => helper_month_label_ru(date('n', $period_start)) . ' ' . date('Y', $period_start), 'align' => 'center'];
    }
    foreach ($data['Итого']['period'] as $period_start => $volume) {
      $header[] = ['data' => helper_month_label_ru(1) . '-' . helper_month_label_ru(date('n', $period['to'])-1) . ' ' . date('Y', $period_start), 'align' => 'center'];
    }

    foreach ($data as $key => $group) {
      $row = [$key];
      // проходим по колонкам Итого, так как в самих Группах могут быть пропущены месяцы (нет данных)
      foreach ($data['Итого']['month'] as $key_m => $aux) {
        $row[] = ['data' => helper_number_format($group['month'][$key_m] / DASH_PROD_DIVIDER ?? 0, 2, ' ', false), 'align' => 'right'];
      }
      // проходим по колонкам Итого, так как в самих Группах может не быть периодов (нет данных по ним)
      foreach ($data['Итого']['period'] as $key_p => $aux) {
        $row[] = ['data' => helper_number_format($group['period'][$key_p] / DASH_PROD_DIVIDER ?? 0, 2, ' ', false), 'align' => 'right'];
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

  return '<div class="production-client-fact-table">' . $output . '</div>';
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
//        '#min' => 0,
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

    $data[$client_name]['period']['plan'] = ($data[$client_name]['period']['plan'] ?? 0) + $record->volume;

    $total['period']['plan'] = ($total['period']['plan'] ?? 0) + $record->volume;
  }

  foreach ($fact as $record) {
    $client = entity_metadata_wrapper('taxonomy_term', $record->client);
    $client_name = $client->label();

    if (!isset($data[$client_name]['month']['fact'])) $data[$client_name]['month']['fact'] = 0;
    if (!isset($data[$client_name]['period']['fact'])) $data[$client_name]['period']['fact'] = 0;
    if (!isset($total['month']['fact'])) $total['month']['fact'] = 0;
    if (!isset($total['period']['fact'])) $total['period']['fact'] = 0;

    if ($month_start <= $record->date && $record->date < $next_month_start) {
      $data[$client_name]['month']['fact'] += $record->volume;
      $total['month']['fact'] += $record->volume;
    }

    $data[$client_name]['period']['fact'] += $record->volume;

    $total['period']['fact'] += $record->volume;
  }

  if ($total) $data['Итого'] = $total;

    // формируем таблицу
  $header = $rows = [];

  if ($data) {
    $header = [
      ['data' => 'Партнёры', 'align' => 'center', 'class' => ['td-name']],
      ['data' => 'План ' . helper_month_label_ru(date('n', $month_start)) . ' ' . date('Y', $month_start), 'align' => 'center'],
      ['data' => 'Факт ' . helper_month_label_ru(date('n', $month_start)) . ' ' . date('Y', $month_start), 'align' => 'center'],
      ['data' => 'Выполнение (%) ' . helper_month_label_ru(date('n', $month_start)) . ' ' . date('Y', $month_start), 'align' => 'center'],
      ['data' => 'Разница ' . helper_month_label_ru(date('n', $month_start)) . ' ' . date('Y', $month_start), 'align' => 'center'],
      ['data' => 'План ' . helper_month_label_ru(date('n', $period['from'])) . '-' . helper_month_label_ru(date('n', $month_start)) . ' ' . date('Y', $month_start), 'align' => 'center'],
      ['data' => 'Факт ' . helper_month_label_ru(date('n', $period['from'])) . '-' . helper_month_label_ru(date('n', $month_start)) . ' ' . date('Y', $month_start), 'align' => 'center'],
      ['data' => 'Выполнение (%) ' . helper_month_label_ru(date('n', $period['from'])) . '-' . helper_month_label_ru(date('n', $month_start)) . ' ' . date('Y', $month_start), 'align' => 'center'],
      ['data' => 'Разница ' . helper_month_label_ru(date('n', $period['from'])) . '-' . helper_month_label_ru(date('n', $month_start)) . ' ' . date('Y', $month_start), 'align' => 'center'],
    ];

    foreach ($data as $key => $client) {
      $row = [$key];
      $row[] = ['data' => helper_number_format($client['month']['plan'] / DASH_PROD_DIVIDER, 2, ' ', false), 'align' => 'right'];
      $row[] = ['data' => helper_number_format($client['month']['fact'] / DASH_PROD_DIVIDER, 2, ' ', false), 'align' => 'right'];
      $prc = $client['month']['plan'] ? helper_number_format(($client['month']['fact'] / $client['month']['plan'] * 100), 2, ' ', false) : '-';
      $row[] = ['data' => $prc, 'align' => 'right'];
      $row[] = ['data' => helper_number_format(($client['month']['fact'] - $client['month']['plan']) / DASH_PROD_DIVIDER, 2, ' ', false), 'align' => 'right'];

      $row[] = ['data' => helper_number_format($client['period']['plan'] / DASH_PROD_DIVIDER, 2, ' ', false), 'align' => 'right'];
      $row[] = ['data' => helper_number_format($client['period']['fact'] / DASH_PROD_DIVIDER, 2, ' ', false), 'align' => 'right'];
      $prc = $client['period']['plan'] ? helper_number_format(($client['period']['fact'] / $client['period']['plan'] * 100), 2, ' ', false) : '-';
      $row[] = ['data' => $prc, 'align' => 'right'];
      $row[] = ['data' => helper_number_format(($client['period']['fact'] - $client['month']['plan']) / DASH_PROD_DIVIDER, 2, ' ', false), 'align' => 'right'];

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

  return '<div class="production-client-plan-fact-table">' . $output . '</div>';
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

    $total['period']['plan'] += $record->volume;
  }

  foreach ($fact as $record) {
    if ($month_start <= $record->date && $record->date < $next_month_start) {
      $total['month']['fact'] = ($total['month']['fact'] ?? 0) + $record->volume;
    }

    $total['period']['fact'] = ($total['period']['fact'] ?? 0) + $record->volume;
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
    $chart['plan']['#data'][] = $total['month']['plan'] / DASH_PROD_DIVIDER;
    $chart['fact']['#data'][] = $total['month']['fact'] / DASH_PROD_DIVIDER;

    $chart['xaxis']['#labels'][] = helper_month_label_ru(date('n', $period['from'])) . '-' . helper_month_label_ru(date('n', $month_start));
    $chart['plan']['#data'][] = $total['period']['plan'] / DASH_PROD_DIVIDER;
    $chart['fact']['#data'][] = $total['period']['fact'] / DASH_PROD_DIVIDER;
  }

  return $chart;
}

// слайд 6
function reports_ah_cash_income_fact_table($period)
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
    foreach ($data['Итого']['period'] as $period_start => $amount) {
      $header[] = ['data' => helper_month_label_ru(date('n', $period['from'])) . '-' . helper_month_label_ru(date('n', $period['to'])-1) . ' ' . date('Y', $period['to']), 'align' => 'center'];
    }


    foreach ($data as $direction_name => $item_d) {
      $row = [$direction_name];
      // проходим по колонкам Итого, так как в самих Группах могут быть пропущены месяцы (нет данных)
      foreach ($data['Итого']['month'] as $key_m => $aux) {
        $row[] = ['data' => helper_number_format($item_d['month'][$key_m] / DASH_REV_DIVIDER, 2, ' ', false), 'align' => 'right'];
      }
      // проходим по колонкам Итого, так как в самих Группах может не быть периодов (нет данных по ним)
      foreach ($data['Итого']['period'] as $key_p => $aux) {
        $row[] = ['data' => helper_number_format($item_d['period'][$key_p] / DASH_REV_DIVIDER, 2, ' ', false), 'align' => 'right'];
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

  return '<div class="cash-income-fact-table">' . $output . '</div>';
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
      if (!isset($total[$year]['period'])) $total[$year]['period'] = 0;

      if ($month_start <= $record->date && $record->date < $next_month_start) {
        $total[$year]['month'] += $record->amount;
      }

      $total[$year]['period'] += $record->amount;
    }
  }

  if ($total) {
    $chart = array(
      '#type' => 'chart',
      '#chart_type' => 'column',
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
    );

    foreach ($total as $year => $record) {
      $chart[$year] = [
        '#type' => 'chart_data',
        '#title' => (string)$year,
        '#data' => [],
      ];
    }

    $chart['xaxis']['#labels'][] = helper_month_label_ru(date('n', $month_start));
    $chart['xaxis']['#labels'][] = helper_month_label_ru(date('n', $period['from'])) . '-' . helper_month_label_ru(date('n', $month_start));

    foreach ($total as $year => $record) {
      $chart[$year]['#data'][] = $record['month'] / DASH_REV_DIVIDER;
      $chart[$year]['#data'][] = $record['period'] / DASH_REV_DIVIDER;
    }
  }

  return $chart;
}

// слайд 9
function reports_ah_debt_debitors_table_output($period)
{
  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  // Данные за месяц передаются в этом же месяце, но отображать их нужно как данные на 1 число следующего месяца,
  // то есть данные за июль выводятся под заголовком 1 августа.
  $prev_year_start = gmmktime(0, 0, 0, 1, 1, date('Y', $period['from'])-1);
  $prev_year_month_start = gmmktime(0, 0, 0, date('n', $period['to'])-1, 1, date('Y', $period['from'])-1);
  $prev_year_next_month_start = gmmktime(0, 0, 0, date('n', $period['to']), 1, date('Y', $period['from'])-1);
  $year_start = $period['from'];
  $month_start = gmmktime(0, 0, 0, date('n', $period['to'])-1, 1, date('Y', $period['from']));
  $next_month_start = gmmktime(0, 0, 0, date('n', $period['to']), 1, date('Y', $period['from']));

  $data = [
    'row1' => [],
    'kccc' => [],
    'row2' => [],
    'row3' => [],
  ];

  // собрать все записи Дебиторской задолженности (category = 1)
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

  return '<div class="debt-debitors-table">' . $output . '</div>';
}

// слайд 10
function reports_ah_debt_creditors_table_output($period)
{
  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  // Данные за месяц передаются в этом же месяце, но отображать их нужно как данные на 1 число следующего месяца,
  // то есть данные за июль выводятся под заголовком 1 августа.
  $prev_year_start = gmmktime(0, 0, 0, 1, 1, date('Y', $period['from'])-1);
  $prev_year_month_start = gmmktime(0, 0, 0, date('n', $period['to'])-1, 1, date('Y', $period['from'])-1);
  $prev_year_next_month_start = gmmktime(0, 0, 0, date('n', $period['to']), 1, date('Y', $period['from'])-1);
  $year_start = $period['from'];
  $month_start = gmmktime(0, 0, 0, date('n', $period['to'])-1, 1, date('Y', $period['from']));
  $next_month_start = gmmktime(0, 0, 0, date('n', $period['to']), 1, date('Y', $period['from']));

  $data = [
    'row1' => [],
    'kccc' => [],
    'row2' => [],
    'row3' => [],
  ];

  // собрать все записи Кредиторской задолженности (category = 2)
  $credits = dash_debt_get_by_conditions([
    'company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'kinds' => [DASH_DEBT_KIND_CREDITOR],
    'period' => ['from' => $prev_year_start, 'to' => $period['to']]
  ]);
  foreach($credits as $credit) {
    if ($credit->category == 1 && strpos(drupal_strtolower($credit->name), 'поставщик') !== false) { $row_key = 'row1'; }
    elseif ($credit->category == 1 && strpos(drupal_strtolower($credit->name), 'авансам') !== false) { $row_key = 'row2'; }
    elseif ($credit->category == 1 && strpos(drupal_strtolower($credit->name), 'вся') !== false) { $row_key = 'row3'; }
    elseif ($credit->category == 2) { $row_key = 'kccc'; }

    $data[$row_key][$credit->name][$credit->date] = $credit->amount / DASH_REV_DIVIDER;
  }

  // формируем таблицу
  $header = [
    ['data' => 'Наименование', 'align' => 'center', 'class' => ['td-name']],
    ['data' => date('d.m.Y', $prev_year_start), 'align' => 'center'],
    ['data' => date('d.m.Y', $prev_year_next_month_start), 'align' => 'center'],
    ['data' => date('d.m.Y', $year_start), 'align' => 'center'],
    ['data' => date('d.m.Y', $next_month_start), 'align' => 'center'],
  ];

  $rows = [];
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

  return '<div class="debt-creditors-table">' . $output . '</div>';
}

// слайд 12
function reports_ah_debt_credits_loans_table_output($period)
{
  $company = dash_term_get_by_1c_id('companies', DASH_INN_AH);

  // Данные за месяц передаются в этом же месяце, но отображать их нужно как данные на 1 число следующего месяца,
  // то есть данные за июль выводятся под заголовком 1 августа.
  $prev_year_start = gmmktime(0, 0, 0, 1, 1, date('Y', $period['from'])-1);
  $prev_year_month_start = gmmktime(0, 0, 0, date('n', $period['to'])-1, 1, date('Y', $period['from'])-1);
  $prev_year_next_month_start = gmmktime(0, 0, 0, date('n', $period['to']), 1, date('Y', $period['from'])-1);
  $year_start = $period['from'];
  $month_start = gmmktime(0, 0, 0, date('n', $period['to'])-1, 1, date('Y', $period['from']));
  $next_month_start = gmmktime(0, 0, 0, date('n', $period['to']), 1, date('Y', $period['from']));

  $data = [
    'row1' => [],
    'row2' => [],
    'kccc' => [],
  ];

  // собрать все записи по всем кредитам и займам (category = 3,4)
  $debts  = dash_debt_get_by_conditions([
    'company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'kinds' => [DASH_DEBT_KIND_CREDIT, DASH_DEBT_KIND_LOAN],
    'period' => ['from' => $prev_year_start, 'to' => $period['to']]
  ]);
  $loan = 'Внутренние займы всего с %, в т.ч.:';
  foreach($debts as $debt) {
    if ($debt->kind == 3) {
      $data['row1'][$debt->name][$debt->date] = $debt->amount / DASH_REV_DIVIDER;
    }
    elseif ($debt->kind == 4 && $debt->category == 2) {
      if (!isset($data['row2'][$loan][$debt->date])) $data['row2'][$loan][$debt->date] = 0;
      if (!isset($data['row2'][$loan]['percent'][$debt->date])) $data['row2'][$loan]['percent'][$debt->date] = 0;
      $data['row2'][$loan][$debt->date]                   += $debt->amount / DASH_REV_DIVIDER;
      $data['row2'][$loan]['percent'][$debt->date]        += $debt->amount2 / DASH_REV_DIVIDER;
      $data['kccc'][$debt->name][$debt->date]             = $debt->amount / DASH_REV_DIVIDER;
      $data['kccc'][$debt->name]['percent'][$debt->date]  = $debt->amount2 / DASH_REV_DIVIDER;
    }
  }

  // формируем таблицу
  $header = [
    ['data' => 'Наименование', 'align' => 'center', 'class' => ['td-name']],
    ['data' => date('d.m.Y', $prev_year_start), 'align' => 'center'],
    ['data' => date('d.m.Y', $prev_year_next_month_start), 'align' => 'center'],
    ['data' => date('d.m.Y', $year_start), 'align' => 'center'],
    ['data' => date('d.m.Y', $next_month_start), 'align' => 'center'],
    ['data' => 'Неопл. % по займам', 'align' => 'center'],
  ];

  $rows = [];
  foreach ($data as $key => $row) {
    foreach ($row as $name => $item) {
      $rows[] = [
        'data' => [
          ($key == 'kccc' ? '&nbsp;&nbsp;- ': '') . $name,
          ['data' => helper_number_format($item[$prev_year_start] ?? 0, 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($item[$prev_year_month_start] ?? 0, 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($item[$year_start] ?? 0, 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($item[$month_start] ?? 0, 1, ' ', false), 'align' => 'right'],
          ['data' => isset($item['percent'][$month_start]) ? helper_number_format($item['percent'][$month_start], 1, ' ', false) : '', 'align' => 'right'],
        ],
        'class' => [$key == 'row2' ? 'tfoot': ''],
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

  return '<div class="credits-loans-table">' . $output . '</div>';
}
