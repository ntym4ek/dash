<?php

function reports_td_period_to_form($form, &$form_state)
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
      'callback' => 'reports_td_period_to_form_callback',
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
      'callback' => 'reports_td_period_to_form_callback',
      'wrapper' => 'period-to-form-wrapper',
    ],
    '#suffix' => '</div></div></div></div>',
  ];

  $form['content'] = [
    '#markup' => reports_td_page_content($month_start)
  ];

  return $form;
}
function reports_td_period_to_form_callback($form, &$form_state)
{
  return $form;
}

function reports_td_page_content($period_to)
{
  // слайд 1
  $url = url('/data/plan-prod/edit', ['query' => ['blocks' => ['td_block_1'], 'inn' => DASH_INN_TD]]);
  $output  =  '<div class="section-title">' .
                '<h3>План-факт производства продукции за ' . date('Y') . ' (в ' . DASH_PROD_SUFFIX . ')</h3>' .
                '<a class="edit-link autodialog" data-dialog-width="450" href="' . $url . '">Редактировать данные</a>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '';
  $output .= '<div class="ajax-load" data-func="td_block_1" data-delay="100"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';

  // слайд 2
  $url = url('/data/plan-ship/edit', ['query' => ['blocks' => ['td_block_2'], 'inn' => DASH_INN_TD]]);
  $output .=  '<div class="section-title">' .
                '<h3>План-факт отгрузки продукции (в ' . DASH_REV_SUFFIX . ')</h3>' .
                '<a class="edit-link autodialog" data-dialog-width="550" href="' . $url . '">Редактировать данные</a>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="ajax-load" data-func="td_block_2" data-delay="200"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';

  // слайд 3
  $period = dash_year_time_bounds_get(DASH_PERIOD_START_MONTH_TD);
  $url = url('/data/plan-ship/edit', ['query' => ['blocks' => ['td_block_31', 'td_block_32'], 'inn' => DASH_INN_TD]]);
  $output .=  '<div class="section-title">' .
               '<h3>Отгрузки + контракты на 1 ' . helper_month_label_ru(date('n')) . ' в сравнении по сезонам ' . (date('y', $period['from'])-1) . '-' . (date('y', $period['end'])-1) . ' / ' . date('y', $period['from']) . '-' . date('y', $period['end']) . '</h3>' .
               '<a class="edit-link autodialog" data-dialog-width="500" href="' . $url . '">Редактировать данные</a>' .
               '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12 col-md-6">';
  $output .=     '<h5 class="chart-title">В млрд рублей с НДС</h5>';
  $output .=     '<div class="ajax-load" data-func="td_block_31" data-delay="310"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .=   '<div class="col-xs-12 col-md-6">';
  $output .=     '<h5 class="chart-title">В млн литров</h5>';
  $output .=     '<div class="ajax-load" data-func="td_block_32" data-delay="320"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 4
  $output .=  '<div class="section-title">' .
                '<h3>План-факт поступления денежных средств (в ' . DASH_REV_SUFFIX . ')</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="ajax-load" data-func="td_block_4" data-delay="400"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';

  // слайды 5 и 6
  $output .=  '<div class="section-title">' .
                '<h3>Динамика поступления денежных средств год к году (в ' . DASH_REV_SUFFIX . ')</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12 col-md-6">';
  $output .=     '<h5 class="chart-title">За месяц: ' . helper_month_label_ru(date('n', $period_to)-1) . '</h5>';
  $output .=     '<div class="ajax-load" data-func="td_block_51" data-delay="500"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .=   '<div class="col-xs-12 col-md-6">';
  $output .=     '<h5 class="chart-title">С начала года: ' . helper_month_label_ru(1) . ' - ' . helper_month_label_ru(date('n', $period_to)-1) . '</h5>';
  $output .=     '<div class="ajax-load" data-func="td_block_52" data-delay="550"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $period = dash_year_time_bounds_get(DASH_PERIOD_START_MONTH_TD);
  $output .=   '<div class="col-xs-12 col-md-8">';
  $output .=     '<h5 class="chart-title">За сезон: ' . helper_month_label_ru(DASH_PERIOD_START_MONTH_TD) . ' ' . date('Y', $period['from']) . ' - ' . helper_month_label_ru(date('n', $period_to)-1) . ' ' . date('Y', $period_to) . '</h5>';
  $output .=     '<div class="ajax-load" data-func="td_block_6" data-delay="600"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 7
  $output .=  '<div class="section-title">' .
                '<h3>План-факт. Бюджет движения денежных средств (в ' . DASH_REV_SUFFIX . ' с НДС)</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12">';
  $output .=     '<h5 class="chart-title">На ' . helper_month_label_ru(date('n', $period_to)) . '</h5>';
  $output .=     '<div class="ajax-load" data-func="td_block_7" data-delay="700"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 8
  $output .=  '<div class="section-title">' .
                '<h3>Динамика дебиторской задолженности покупателей (в ' . DASH_REV_SUFFIX . ' с НДС)</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12">';
  $output .=     '<div class="ajax-load" data-func="td_block_8" data-delay="800"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 9
  $output .=  '<div class="section-title">' .
                '<h3>Динамика кредиторской задолженности покупателей (в ' . DASH_REV_SUFFIX . ' с НДС)</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12">';
  $output .=     '<div class="ajax-load" data-func="td_block_9" data-delay="900"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 10
  $output .=  '<div class="section-title">' .
                '<h3>Кредиты и внутренние займы (в ' . DASH_REV_SUFFIX . ')</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12">';
  $output .=     '<div class="ajax-load" data-func="td_block_10" data-delay="1000"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 11
  $url = url('/data/debt/edit', ['query' => ['blocks' => ['td_block_11'], 'inn' => DASH_INN_TD]]);
  $output .=  '<div class="section-title">' .
                '<h3>План-факт по банковским кредитам (в ' . DASH_REV_SUFFIX . ')</h3>' .
                '<a class="edit-link autodialog" data-dialog-width="550" href="' . $url . '">Редактировать данные</a>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12">';
  $output .=     '<div class="ajax-load" data-func="td_block_11" data-delay="1100"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 12
  $output .=  '<div class="section-title">' .
                '<h3>Динамика готовой продукции и сырья (в ' . DASH_REV_SUFFIX . ' без НДС)</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12">';
  $output .=     '<div class="ajax-load" data-func="td_block_12" data-delay="1200"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 13
  $period = dash_year_time_bounds_get(1);
  $url = url('/data/manual-bdr/edit', ['query' => ['blocks' => ['td_block_13'], 'inn' => DASH_INN_TD]]);
  $output .=  '<div class="section-title">' .
                '<h3>План-факт. Бюджет доходов и расходов (в ' . DASH_REV_SUFFIX . ' без НДС)</h3>' .
                '<a class="edit-link autodialog" data-dialog-width="550" href="' . $url . '">Редактировать данные</a>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12">';
  $output .=     '<h5 class="chart-title">За ' . helper_month_label_ru(date('n', $period['from'])) . '-' . helper_month_label_ru(date('n', $period_to)-1) . '</h5>';
  $output .=     '<div class="ajax-load" data-func="td_block_13" data-delay="1300"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  // слайд 14
  $output .=  '<div class="section-title">' .
                '<h3>План-факт расходов на регистрацию (в ' . DASH_REV_SUFFIX . ')</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12">';
  $output .=     '<div class="ajax-load" data-func="td_block_14" data-delay="1400"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';
  $output .=   '</div>';
  $output .= '</div>';

  $period = dash_year_time_bounds_get(1);
  return '<div class="charts" data-module="reports" data-period-from="' . $period['from'] . '" data-period-to="' . $period_to . '" data-period-end="' . $period['end'] . '">' . $output . '</div>';
}

function reports_td_block_1()
{
  // период с начала года по начало заданного месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => gmmktime(0, 0, 0, date('n', $_GET['period_to'])+1, 1, date('Y', $_GET['period_to'])) ?? gmmktime(0, 0, 0, date('n')+1, 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  // план-факт отгрузки продукции
  $data_arr = reports_column_production_plan_fact($period);

  return drupal_render($data_arr);
}
function reports_td_block_2()
{
  // период с начала года по начало заданного месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => $_GET['period_to'] ?? gmmktime(0, 0, 0, date('n'), 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  // план-факт отгрузки продукции
  $data_arr = reports_line_ship_plan_fact($period);

  return drupal_render($data_arr);
}
function reports_td_block_31()
{
  // период с начала сезона по начало заданного месяца
  $period = dash_year_time_bounds_get(DASH_PERIOD_START_MONTH_TD);
  $period['to'] = $_GET['period_to'] ?? gmmktime(0, 0, 0, date('n'), 1, date('Y'));

  // сравнение отгрузки (план и факт+контракты) по годам в рублях
  $data_arr = reports_column_ship_plan_fact_compare_revenue($period);

  return drupal_render($data_arr);
}
function reports_td_block_32()
{
  // период с начала сезона по начало заданного месяца
  $period = dash_year_time_bounds_get(DASH_PERIOD_START_MONTH_TD);
  $period['to'] = $_GET['period_to'] ?? gmmktime(0, 0, 0, date('n'), 1, date('Y'));

  // сравнение отгрузки (план и факт+контракты) по годам в литрах
  $data_arr = reports_column_ship_plan_fact_compare_qty($period);

  return drupal_render($data_arr);
}
function reports_td_block_4()
{
  // период с начала года по начало заданного месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => $_GET['period_to'] ?? gmmktime(0, 0, 0, date('n'), 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  // план-факт выручки помесячно линейный
  $output = '<div class="row">';
  $data_arr =   reports_line_revenue_plan_fact($period);
  $output .=    '<div id="plan-fact-target" class="col-xs-12">' . drupal_render($data_arr) . '</div>';
  $output .= '</div>';

  return $output;
}
function reports_td_block_51()
{
  // период с начала предыдущего заданному месяца по начало заданного месяца
  $period = [
    'from' => gmmktime(0, 0, 0, date('n', $_GET['period_to'])-1, 1, date('Y')),
    'to' => gmmktime(0, 0, 0, date('n', $_GET['period_to']), 1, date('Y')),
    'end' => gmmktime(0, 0, 0, date('n', $_GET['period_to']), 1, date('Y')),
  ];

  // факт поступления выручки за последний полный месяц
  $data_arr = reports_column_revenue_fact_compare($period);

  return drupal_render($data_arr);
}
function reports_td_block_52()
{
  // период с начала года по начало текущего месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => $_GET['period_to'] ?? gmmktime(0, 0, 0, date('n'), 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  // факт поступления выручки
  $data_arr = reports_column_revenue_fact_compare($period);

  return drupal_render($data_arr);
}
function reports_td_block_6()
{
  // период с начала сезона по начало месяца
  $period = dash_year_time_bounds_get(DASH_PERIOD_START_MONTH_TD);
  $period['to'] = $_GET['period_to'] ?? gmmktime(0, 0, 0, date('n'), 1, date('Y'));

  // факт поступления выручки
  $data_arr = reports_column_revenue_fact_compare($period);

  return drupal_render($data_arr);
}
function reports_td_block_7()
{
  // период с начала года по начало текущего месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => $_GET['period_to'] ?? gmmktime(0, 0, 0, date('n'), 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  return reports_bdds_table_output($period);
}
function reports_td_block_8()
{
  // период с начала года по начало текущего месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => $_GET['period_to'] ?? gmmktime(0, 0, 0, date('n'), 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  return reports_debt_debitors_table_output($period);
}
function reports_td_block_9()
{
  // период с начала года по начало текущего месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => $_GET['period_to'] ?? gmmktime(0, 0, 0, date('n')+1, 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  return reports_debt_creditors_table_output($period);
}
function reports_td_block_10()
{
  // период с начала года по начало текущего месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => $_GET['period_to'] ?? gmmktime(0, 0, 0, date('n')+1, 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  return reports_debt_credits_loans_table_output($period);
}
function reports_td_block_11()
{
  // период с начала года по начало текущего месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => $_GET['period_to'] ?? gmmktime(0, 0, 0, date('n')+1, 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  $data_arr = reports_line_debt_credits_plan_fact($period);

  return drupal_render($data_arr);
}
function reports_td_block_12()
{
  // период с начала года по начало текущего месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => $_GET['period_to'] ?? gmmktime(0, 0, 0, date('n')+1, 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  return reports_remains_progress_table_output($period);
}
function reports_td_block_13()
{
  // период с начала года по начало текущего месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => $_GET['period_to'] ?? gmmktime(0, 0, 0, date('n'), 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  return reports_manual_bdr_table_output($period);
}
function reports_td_block_14()
{
  // период с начала года по начало заданного месяца
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, date('Y')),
    'to' => $_GET['period_to'] ?? gmmktime(0, 0, 0, date('n'), 1, date('Y')),
    'end' => gmmktime(0, 0, 0, 1, 1, date('Y')+1),
  ];

  // план-факт отгрузки продукции
  $data_arr = reports_column_registration_plan_fact($period);

  return drupal_render($data_arr);
}


/**
 * ----- Подготовка графика --------------------------------------------------------------------------------------------
 */
function reports_column_production_plan_fact($period)
{
  $chart = dash_no_data_chart_get();

  // собрать все записи за период
  if ($data = dash_plan_prod_get_by_conditions(['period' => $period, 'manual' => 1])) {
    // сгруппировать помесячно
    $plan_fact_by_month = [];
    foreach (dash_months_list_get($period) as $month) {
      $plan_fact_by_month[$month['start']] = [
        'plan_output' => 0,
        'fact_output' => 0,
        'start' => $month['start'],
        'end' => $month['end'],
      ];

      foreach ($data as $record) {
        if ($record->date >= $month['start'] && $record->date < $month['end']) {
          if ($record->fact) $plan_fact_by_month[$month['start']]['fact_output'] += $record->output;
          else               $plan_fact_by_month[$month['start']]['plan_output'] += $record->output;
        }
      }
    }
    ksort($plan_fact_by_month);

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

    foreach ($plan_fact_by_month as $month) {
      if ($month['plan_output'] || $month['fact_output']) {
        $chart['xaxis']['#labels'][] = helper_month_label_ru(date('n', $month['start']));

        $plan = $month['plan_output'] / DASH_PROD_DIVIDER;
        $fact = $month['fact_output'] / DASH_PROD_DIVIDER;

        $chart['plan']['#data'][] = $plan;
        $chart['fact']['#data'][] = $fact;

        $tooltip = '<div style="padding: 8px 12px; min-width:140px;white-space: nowrap;">' .
          '<div><div style="display:inline-block; font-weight: 700;">' . helper_month_label_ru(date('n', $month['start'])) . '</div></div>' .
          '<div><div style="display:inline-block; width: 210px;">План:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($plan, 2, ',', ' ') . '</b>&nbsp;' . DASH_PROD_SUFFIX . '</div></div>' .
          '<div><div style="display:inline-block; width: 210px;">Факт:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($fact, 2, ',', ' ') . '</b>&nbsp;' . DASH_PROD_SUFFIX . '</div></div>' .
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
    }
  }

  return $chart;
}
function reports_line_revenue_plan_fact($period)
{
  $chart = dash_no_data_chart_get();

  $plan_bounds = ['from' => $period['from'], 'to' => $period['end']];

  // собрать все записи за период
  $plan = dash_plan_revenue_get_by_conditions(['type' => DASH_TYPE_PLAN_DEPR, 'period' => $plan_bounds]);
  $fact = dash_plan_revenue_get_by_conditions(['type' => DASH_TYPE_FACT_DEPR, 'period' => $period]);

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

    $total_plan = $total_fact = 0;
    $last_plan = $last_fact = 0;
    foreach ($plan_fact_by_month as $month) {
      $chart['xaxis']['#labels'][] = helper_month_label_ru(date('n', $month['start']));

      $plan = $month['plan_amount'] / DASH_REV_DIVIDER;
      $fact = $month['fact_amount'] / DASH_REV_DIVIDER;
      $total_plan += $plan;
      $total_fact += $fact;
      // если это текущий период,
      // то считаем и выводим накопительный факт только до месяца предстоящего текущему месяцу
      $total_fact = ($month['start'] < gmmktime(0, 0, 0, date('n', $period['to']), 1, date('Y'))) ? $total_fact : null;

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
  } else {
    $chart['#colors'] = ['#ccc', DASH_COLOR_DARK_RED];
  }
  if ($total_plan < $last_fact * 1.5) {
    $chart['yaxis']['#max'] = max($last_fact, $total_plan) * 1.5;
  }

  return $chart;
}

function reports_column_revenue_fact_compare($period)
{
  $chart = dash_no_data_chart_get();

  // собрать данные периодов с DASH_YEAR_HISTORY_START года
  $data = [];
  foreach (dash_years_shift_list_get($period, date('n', $period['from'])) as $shift) {
    $period_shifted = dash_period_shifted_get($period, $shift);

    $fact = dash_plan_revenue_get_by_conditions(['type' => DASH_TYPE_FACT_DEPR, 'period' => $period_shifted]);

    $amount = 0;
    if ($fact) foreach ($fact as $record) {
      $amount += $record->amount;
    }

    if ($amount) {
      $data[date('Y', $period_shifted['from'])] = [
        'amount' => $amount / DASH_REV_DIVIDER,
      ];
    }
  }

  if ($data) {
    $chart = array(
      '#type' => 'chart',
      '#chart_type' => 'column',
      '#font' => 'Montserrat',
      '#height' => '500',
      'xaxis' => [
        '#type' => 'chart_xaxis',
      ],
      'yaxis' => [
        '#type' => 'chart_yaxis',
        '#min' => 0,
      ],
      '#raw_options' => [
        'titleTextStyle' => ['bold' => 'true', 'fontSize' => '16'],
        'legend' => ['position' => 'top', 'textStyle' => ['fontSize' => '12']],
        'chartArea' => ['height' => '80%', 'width' => '85%', 'left' => 70],
        'annotations' => [
          'textStyle' => ['fontSize' => 14, 'bold' => 'true'],
        ],
        'tooltip' => ['isHtml' => true],
        'bar' => ['groupWidth' => "70%"],
        'isStacked' => 'true',
      ],
    );

    // чтобы была возможность подписать столбцы и вывести их в разном цвете,
    // график выполнен в виде размещённых друг на друге значений за года,
    // где реальное значение только у нужного года, а остальные имеют значение null
    $year_prev = 0;
    $tooltip = '';
    foreach ($data as $year_1 => $item_1) {
      $label = date('n', $period['from']) == DASH_PERIOD_START_MONTH_TD ? $year_1 . '/' . ($year_1+1) : $year_1;

      $chart[$year_1] = [
        '#type' => 'chart_data',
        '#title' => (string)$label,
        '#data' => [],
      ];
      foreach ($data as $year_2 => $item_2) {
        if ($year_1 == $year_2) {
          $chart['xaxis']['#labels'][] = (string)$label;
          $chart[$year_1]['#data'][] = $item_1['amount'];

          $percent = '-';
          $delta = 0;
          if ($year_prev) {
            $delta = $data[$year_1]['amount'] - $data[$year_prev]['amount'];
            if ($data[$year_prev]['amount']) $percent = number_format(abs($delta) / $data[$year_prev]['amount'] * 100, 0, ',', ' ');
          }
          $tooltip =
            '<div style="padding: 8px 12px; min-width:140px;white-space: nowrap;">' .
            '<div><div style="display:inline-block; font-weight: 700;">' . $year_1 . '</div></div>' .
            '<div><div style="display:inline-block; width: 60px;">' . ($delta > 0 ? 'Рост' : 'Падение') . ':</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . $percent . '</b>&nbsp;%</div></div>' .
            '</div>';

          $year_prev = $year_1;
        } else {
          $chart[$year_1]['#data'][] = null;
        }

        $chart[$year_1][] = array(
          '#type' => 'chart_data_item',
          '#title' => $tooltip,
          '#raw_options' => [
            'annotation' => number_format($item_1['amount'], 1, ',', ' '),
          ],
        );

      }
    }
  }

  return $chart;
}

function reports_line_ship_plan_fact($period)
{
  $chart = dash_no_data_chart_get();

  $plan_bounds = ['from' => $period['from'], 'to' => $period['end']];

  // собрать все записи за период
  $plan = dash_plan_ship_get_by_conditions(['type' => DASH_TYPE_PLAN_DEPR, 'period' => $plan_bounds, 'manual' => 1]);
  $fact = dash_plan_ship_get_by_conditions(['type' => DASH_TYPE_FACT_DEPR, 'period' => $period]);

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
          'annotations' => ['stem' => ['color' => 'transparent', 'length' => -40]],
        ],
        '#data' => [],
      ],
      'fact' => [
        '#type' => 'chart_data',
        '#title' => 'Факт',
        '#raw_options' => [
          'pointSize' => 5,
          'annotations' => ['stem' => ['color' => 'transparent', 'length' => 40]],
        ],
        '#data' => [],
      ],
    );

    $total_plan = $total_fact = 0;
    $last_plan = $last_fact = 0;
    foreach ($plan_fact_by_month as $month) {
      $chart['xaxis']['#labels'][] = helper_month_label_ru(date('n', $month['start']));

      $plan = $month['plan_amount'] / DASH_REV_DIVIDER;
      $fact = $month['fact_amount'] / DASH_REV_DIVIDER;
      $total_plan += $plan;
      $total_fact += $fact;
      // если это текущий период,
      // то считаем и выводим накопительный факт только до месяца предстоящего текущему месяцу
      $total_fact = ($month['start'] < gmmktime(0, 0, 0, date('n', $period['to']), 1, date('Y'))) ? $total_fact : null;

      $chart['plan']['#data'][] = $total_plan;
      $chart['fact']['#data'][] = $total_fact;

      // запоминаем последнее значение для определения цвета графика
      if (isset($total_fact)) {
        $last_fact = $total_fact;
        $last_plan = $total_plan;
      }

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
    $chart['fact']['#raw_options']['annotations']['stem']['length'] = 40;
    $chart['plan']['#raw_options']['annotations']['stem']['length'] = -40;
  } else {
    $chart['#colors'] = ['#ccc', DASH_COLOR_DARK_RED];
    $chart['fact']['#raw_options']['annotations']['stem']['length'] = -40;
    $chart['plan']['#raw_options']['annotations']['stem']['length'] = 40;
  }
  $chart['yaxis']['#max'] = max($last_plan, $last_fact) * 1.5;

  return $chart;
}

function reports_column_ship_plan_fact_compare_revenue($period)
{
  $chart = dash_no_data_chart_get();

  // собрать данные периодов за предыдущий и текущий сезоны
  $data = [];
  foreach ([-1, 0] as $shift) {
    $period_shifted = dash_period_shifted_get($period, $shift);

    // план за весь год (вводится вручную)
    $plan = dash_plan_ship_get_by_conditions(['type' => DASH_TYPE_PLAN_DEPR, 'period' => ['from' => $period_shifted['from'], 'to' => $period_shifted['end']], 'manual' => true]);
    // остальное по текущий месяц
    $fact = dash_plan_ship_get_by_conditions(['type' => DASH_TYPE_FACT_DEPR, 'period' => $period_shifted]);
    $deal = dash_plan_ship_get_by_conditions(['type' => DASH_TYPE_DEAL_DEPR, 'period' => $period_shifted]);

    $plan_amount = 0;
    if ($plan) foreach ($plan as $record) $plan_amount += $record->amount;
    $fact_amount = 0;
    if ($fact) foreach ($fact as $record) $fact_amount += $record->amount;
    $deal_amount = 0;
    if ($deal) foreach ($deal as $record) $deal_amount += $record->amount;

    // в факт включаем контракты
    $data[date('Y', $period_shifted['from'])] = [
      'plan_amount' => $plan_amount / 1000000000,
      'fact_amount' => ($fact_amount + $deal_amount) / 1000000000,
    ];
    //watchdog('debug', date('Y', $period_shifted['from']) . ' год, массив:<br> <pre>@array</pre>', ['@array' => $plan_amount]);
  }

  if ($data) {
    $chart = array(
      '#type' => 'chart',
      '#chart_type' => 'column',
      '#font' => 'Montserrat',
      '#height' => '500',
      'xaxis' => [
        '#type' => 'chart_xaxis',
      ],
      'yaxis' => [
        '#type' => 'chart_yaxis',
        '#min' => 0,
      ],
      '#raw_options' => [
        'titleTextStyle' => ['bold' => 'true', 'fontSize' => '16'],
        'legend' => ['position' => 'top', 'textStyle' => ['fontSize' => '12']],
        'chartArea' => ['height' => '80%', 'width' => '85%', 'left' => 70],
        'annotations' => [
          'textStyle' => ['fontSize' => 14, 'bold' => 'true'],
        ],
        'tooltip' => ['isHtml' => true],
        'bar' => ['groupWidth' => "70%"],
        'isStacked' => 'true',
      ],
    );

    // чтобы была возможность подписать столбцы и вывести их в разном цвете,
    // график выполнен в виде размещённых друг на друге значений за года,
    // где реальное значение только у нужного года, а остальные имеют значение null
    $year_prev = 0;
    $tooltip = '';
    foreach ($data as $year_1 => $item_1) {
      $label = date('n', $period['from']) == DASH_PERIOD_START_MONTH_TD ? $year_1 . '/' . ($year_1+1) : $year_1;

      $chart[$year_1] = [
        '#type' => 'chart_data',
        '#title' => (string)$label,
        '#data' => [],
      ];
      foreach ($data as $year_2 => $item_2) {
        if ($year_1 == $year_2) {
          $chart['xaxis']['#labels'][] = (string)$label;
          $chart[$year_1]['#data'][] = $item_1['fact_amount'];

          $percent = '-';
          $delta = 0;
          if ($year_prev) {
            $delta = $data[$year_1]['fact_amount'] - $data[$year_prev]['fact_amount'];
            if ($data[$year_prev]['fact_amount']) $percent = number_format(abs($delta) / $data[$year_prev]['fact_amount'] * 100, 0, ',', ' ');
          }
          $tooltip =
            '<div style="padding: 8px 12px; min-width:140px;white-space: nowrap;">' .
            '<div><div style="display:inline-block; font-weight: 700;">' . $year_1 . '</div></div>' .
            '<div><div style="display:inline-block; width: 60px;">' . ($delta > 0 ? 'Рост' : 'Падение') . ':</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . $percent . '</b>&nbsp;%</div></div>' .
            '</div>';

          $year_prev = $year_1;
        } else {
          $chart[$year_1]['#data'][] = null;
        }

        $chart[$year_1][] = array(
          '#type' => 'chart_data_item',
          '#title' => $tooltip,
          '#raw_options' => [
            'annotation' => number_format($item_1['fact_amount'], 1, ',', ' '),
          ],
        );
      }
      $chart[$year_1]['#data'][] = null;
    }

    // добавить столбец плана
    $key = array_key_last($data);
    $label = 'План на год ' . (date('n', $period['from']) == DASH_PERIOD_START_MONTH_TD ? $key . '/' . ($key+1) : $key);
    $chart['xaxis']['#labels'][] = $label;
    $chart['plan'] = [
      '#type' => 'chart_data',
      '#title' => $label,
      '#data' => [null, null, $data[$key]['plan_amount']],
    ];

    $percent = '-';
    if ($data[$key]['fact_amount']) $percent = number_format($data[$key]['fact_amount'] / $data[$key]['plan_amount'] * 100, 0, ',', ' ');
    $tooltip =
      '<div style="padding: 8px 12px; min-width:140px;white-space: nowrap;">' .
      '<div><div style="display:inline-block; font-weight: 700;">' . $label . '</div></div>' .
      '<div><div style="display:inline-block; width: 60px;">Выполнение плана:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . $percent . '</b>&nbsp;%</div></div>' .
      '</div>';
    $chart['plan'][] = null;
    $chart['plan'][] = null;
    $chart['plan'][] = [
      '#type' => 'chart_data_item',
      '#title' => $tooltip,
      '#raw_options' => [
        'annotation' => number_format($data[$key]['plan_amount'], 1, ',', ' '),
      ],
    ];

  }

  return $chart;
}

function reports_column_ship_plan_fact_compare_qty($period)
{
  $chart = dash_no_data_chart_get();

  // собрать данные периодов за предыдущий и текущий сезоны
  $data = [];
  foreach ([-1, 0] as $shift) {
    $period_shifted = dash_period_shifted_get($period, $shift);

    // план за весь год (вводится вручную)
    $plan = dash_plan_ship_get_by_conditions(['type' => DASH_TYPE_PLAN_DEPR, 'period' => ['from' => $period_shifted['from'], 'to' => $period_shifted['end']], 'manual' => true]);
    // остальное по текущий месяц
    $fact = dash_plan_ship_get_by_conditions(['type' => DASH_TYPE_FACT_DEPR, 'period' => $period_shifted]);
    $deal = dash_plan_ship_get_by_conditions(['type' => DASH_TYPE_DEAL_DEPR, 'period' => $period_shifted]);

    $plan_qty = 0;
    if ($plan) foreach ($plan as $record) $plan_qty += $record->qty;
    $fact_qty = 0;
    if ($fact) foreach ($fact as $record) $fact_qty += $record->qty;
    $deal_qty = 0;
    if ($deal) foreach ($deal as $record) $deal_qty += $record->qty;

    // в факт включаем контракты
    $data[date('Y', $period_shifted['from'])] = [
      'plan_qty' => $plan_qty / 1000000,
      'fact_qty' => ($fact_qty + $deal_qty) / 1000000,
    ];
    //watchdog('debug', date('Y', $period_shifted['from']) . ' год, массив:<br> <pre>@array</pre>', ['@array' => $plan_qty]);
  }

  if ($data) {
    $chart = array(
      '#type' => 'chart',
      '#chart_type' => 'column',
      '#font' => 'Montserrat',
      '#height' => '500',
      'xaxis' => [
        '#type' => 'chart_xaxis',
      ],
      'yaxis' => [
        '#type' => 'chart_yaxis',
        '#min' => 0,
      ],
      '#raw_options' => [
        'titleTextStyle' => ['bold' => 'true', 'fontSize' => '16'],
        'legend' => ['position' => 'top', 'textStyle' => ['fontSize' => '12']],
        'chartArea' => ['height' => '80%', 'width' => '85%', 'left' => 70],
        'annotations' => [
          'textStyle' => ['fontSize' => 14, 'bold' => 'true'],
        ],
        'tooltip' => ['isHtml' => true],
        'bar' => ['groupWidth' => "70%"],
        'isStacked' => 'true',
      ],
    );

    // чтобы была возможность подписать столбцы и вывести их в разном цвете,
    // график выполнен в виде размещённых друг на друге значений за года,
    // где реальное значение только у нужного года, а остальные имеют значение null
    $year_prev = 0;
    $tooltip = '';
    foreach ($data as $year_1 => $item_1) {
      $label = date('n', $period['from']) == DASH_PERIOD_START_MONTH_TD ? $year_1 . '/' . ($year_1+1) : $year_1;

      $chart[$year_1] = [
        '#type' => 'chart_data',
        '#title' => (string)$label,
        '#data' => [],
      ];
      foreach ($data as $year_2 => $item_2) {
        if ($year_1 == $year_2) {
          $chart['xaxis']['#labels'][] = (string)$label;
          $chart[$year_1]['#data'][] = $item_1['fact_qty'];

          $percent = '-';
          $delta = 0;
          if ($year_prev) {
            $delta = $data[$year_1]['fact_qty'] - $data[$year_prev]['fact_qty'];
            if ($data[$year_prev]['fact_qty']) $percent = number_format(abs($delta) / $data[$year_prev]['fact_qty'] * 100, 0, ',', ' ');
          }
          $tooltip =
            '<div style="padding: 8px 12px; min-width:140px;white-space: nowrap;">' .
            '<div><div style="display:inline-block; font-weight: 700;">' . $year_1 . '</div></div>' .
            '<div><div style="display:inline-block; width: 60px;">' . ($delta > 0 ? 'Рост' : 'Падение') . ':</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . $percent . '</b>&nbsp;%</div></div>' .
            '</div>';

          $year_prev = $year_1;
        } else {
          $chart[$year_1]['#data'][] = null;
        }

        $chart[$year_1][] = array(
          '#type' => 'chart_data_item',
          '#title' => $tooltip,
          '#raw_options' => [
            'annotation' => number_format($item_1['fact_qty'], 1, ',', ' '),
          ],
        );
      }
      $chart[$year_1]['#data'][] = null;
    }

    // добавить столбец плана
    $key = array_key_last($data);
    $label = 'План на год ' . (date('n', $period['from']) == DASH_PERIOD_START_MONTH_TD ? $key . '/' . ($key+1) : $key);
    $chart['xaxis']['#labels'][] = $label;
    $chart['plan'] = [
      '#type' => 'chart_data',
      '#title' => $label,
      '#data' => [null, null, $data[$key]['plan_qty']],
    ];

    $percent = '-';
    if ($data[$key]['fact_qty']) $percent = number_format($data[$key]['fact_qty'] / $data[$key]['plan_qty'] * 100, 0, ',', ' ');
    $tooltip =
      '<div style="padding: 8px 12px; min-width:140px;white-space: nowrap;">' .
      '<div><div style="display:inline-block; font-weight: 700;">' . $label . '</div></div>' .
      '<div><div style="display:inline-block; width: 60px;">Выполнение плана:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . $percent . '</b>&nbsp;%</div></div>' .
      '</div>';
    $chart['plan'][] = null;
    $chart['plan'][] = null;
    $chart['plan'][] = [
      '#type' => 'chart_data_item',
      '#title' => $tooltip,
      '#raw_options' => [
        'annotation' => number_format($data[$key]['plan_qty'], 1, ',', ' '),
      ],
    ];

  }

  return $chart;
}

function reports_bdds_table_output($period)
{
  $company = dash_term_get_by_1c_id('companies', DASH_INN_TD);

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
  $plan = dash_plan_revenue_get_by_conditions(['type' => DASH_TYPE_PLAN_DEPR, 'period' => $period]);
  $fact = dash_plan_revenue_get_by_conditions(['type' => DASH_TYPE_FACT_DEPR, 'period' => $period]);

  // собрать за предыдущий полный месяц и за год
  if ($plan) foreach ($plan as $record) {
    $data['income']['total']['plan_period'] += $record->amount;
    if ($prev_month_start <= $record->date && $record->date < $period['to']) {
      $data['income']['total']['plan_month'] += $record->amount;
    }
  }
  if ($fact) foreach ($fact as $record) {
    $data['income']['total']['fact_period'] += $record->amount;
    if ($prev_month_start <= $record->date && $record->date < $period['to']) {
      $data['income']['total']['fact_month'] += $record->amount;
    }
  }

  // собрать все записи Расхода ДС Торгового Дома за период
  $plan = dash_cash_outcome_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_PLAN, 'period' => $period]);
  $fact = dash_cash_outcome_get_by_conditions(['company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'period' => $period]);

  // собрать за предыдущий полный месяц и за год
  if ($plan) foreach ($plan as $record) {
    $data['outcome']['total']['plan_period'] += $record->amount;
    if ($prev_month_start <= $record->date && $record->date < $period['to']) $data['outcome']['total']['plan_month'] += $record->amount;

    if (in_array($record->category, [198576, 198575, 198574, 198573])) {
      if (!isset($data['outcome']['categories'][$record->category])) $data['outcome']['categories'][$record->category] = ['plan_month' => 0, 'fact_month' => 0, 'plan_period' => 0, 'fact_period' => 0];
      $data['outcome']['categories'][$record->category]['plan_period'] += $record->amount;
      if ($prev_month_start <= $record->date && $record->date < $period['to']) $data['outcome']['categories'][$record->category]['plan_month'] += $record->amount;
    }
  }
  if ($fact) foreach ($fact as $record) {
    $data['outcome']['total']['fact_period'] += $record->amount;
    if ($prev_month_start <= $record->date && $record->date < $period['to']) $data['outcome']['total']['fact_month'] += $record->amount;

    if (in_array($record->category, [198576, 198575, 198574, 198573])) {
      if (!isset($data['outcome']['categories'][$record->category])) $data['outcome']['categories'][$record->category] = ['plan_month' => 0, 'fact_month' => 0, 'plan_period' => 0, 'fact_period' => 0];
      $data['outcome']['categories'][$record->category]['fact_period'] += $record->amount;
      if ($prev_month_start <= $record->date && $record->date < $period['to']) $data['outcome']['categories'][$record->category]['fact_month'] += $record->amount;
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
      ['data' => helper_number_format($data['income']['total']['plan_month'], 0, ' ', false), 'align' => 'right'],
      ['data' => helper_number_format($data['income']['total']['fact_month'], 0, ' ', false), 'align' => 'right'],
      ['data' => ($data['income']['total']['plan_month'] ? helper_number_format($data['income']['total']['fact_month']/$data['income']['total']['plan_month']*100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
      ['data' => helper_number_format($data['income']['total']['plan_period'], 0, ' ', false), 'align' => 'right'],
      ['data' => helper_number_format($data['income']['total']['fact_period'], 0, ' ', false), 'align' => 'right'],
      ['data' => ($data['income']['total']['plan_period'] ? helper_number_format($data['income']['total']['fact_period']/$data['income']['total']['plan_period']*100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
    ],
  ];

  $rows[] = [
    'data' => [
      '2. Расход (итого), в т.ч.:',
      ['data' => helper_number_format($data['outcome']['total']['plan_month'], 0, ' ', false), 'align' => 'right'],
      ['data' => helper_number_format($data['outcome']['total']['fact_month'], 0, ' ', false), 'align' => 'right'],
      ['data' => ($data['outcome']['total']['plan_month'] ? helper_number_format($data['outcome']['total']['fact_month']/$data['outcome']['total']['plan_month']*100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
      ['data' => helper_number_format($data['outcome']['total']['plan_period'], 0, ' ', false), 'align' => 'right'],
      ['data' => helper_number_format($data['outcome']['total']['fact_period'], 0, ' ', false), 'align' => 'right'],
      ['data' => ($data['outcome']['total']['plan_period'] ? helper_number_format($data['outcome']['total']['fact_period']/$data['outcome']['total']['plan_period']*100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
    ],
  ];

  foreach ($data['outcome']['categories'] as $id => $cat) {
    $term = taxonomy_term_load($id);
    $rows[] = [
      'data' => [
        '&nbsp;&nbsp;- ' . drupal_strtolower($term->name),
        ['data' => helper_number_format($cat['plan_month'], 0, ' ', false), 'align' => 'right'],
        ['data' => helper_number_format($cat['fact_month'], 0, ' ', false), 'align' => 'right'],
        ['data' => ($cat['plan_month'] ? helper_number_format($cat['fact_month']/$cat['plan_month']*100, 0, ' ', false) . '%' : '-'), 'align' => 'right'],
        ['data' => helper_number_format($cat['plan_period'], 0, ' ', false), 'align' => 'right'],
        ['data' => helper_number_format($cat['fact_period'], 0, ' ', false), 'align' => 'right'],
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

  return '<div class="bdds-table">' . $output . '</div>';
}

function reports_debt_debitors_table_output($period)
{
  $company = dash_term_get_by_1c_id('companies', DASH_INN_TD);

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
          ['data' => helper_number_format($item[$prev_year_start], 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($item[$prev_year_month_start], 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($item[$year_start], 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($item[$month_start], 1, ' ', false), 'align' => 'right'],
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

function reports_debt_creditors_table_output($period)
{
  $company = dash_term_get_by_1c_id('companies', DASH_INN_TD);

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
          ['data' => helper_number_format($item[$prev_year_start], 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($item[$prev_year_month_start], 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($item[$year_start], 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($item[$month_start], 1, ' ', false), 'align' => 'right'],
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

function reports_debt_credits_loans_table_output($period)
{
  $company = dash_term_get_by_1c_id('companies', DASH_INN_TD);

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
      if (!isset($data['row2'][$loan]['percent'])) $data['row2'][$loan]['percent'] = 0;
      $data['row2'][$loan][$debt->date] += $debt->amount / DASH_REV_DIVIDER;
      $data['row2'][$loan]['percent']    += $debt->amount2 / DASH_REV_DIVIDER;
      $data['kccc'][$debt->name][$debt->date] = $debt->amount / DASH_REV_DIVIDER;
      $data['kccc'][$debt->name]['percent'] = $debt->amount2 / DASH_REV_DIVIDER;
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
          ['data' => helper_number_format($item[$prev_year_start], 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($item[$prev_year_month_start], 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($item[$year_start], 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($item[$month_start], 1, ' ', false), 'align' => 'right'],
          ['data' => isset($item['percent']) ? helper_number_format($item[$month_start], 1, ' ', false) : '', 'align' => 'right'],
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

function reports_line_debt_credits_plan_fact($period)
{
  $chart = dash_no_data_chart_get();

  $company = dash_term_get_by_1c_id('companies', DASH_INN_TD);

  // собрать все записи за период
  $plans = dash_debt_get_by_conditions([
    'company_id' => $company->tid, 'period' => ['from' => $period['from'], 'to' => $period['end']],
    'type' => DASH_TYPE_PLAN, 'kinds' => [DASH_DEBT_KIND_CREDIT], 'manual' => 1
  ]);
  $facts = dash_debt_get_by_conditions([
    'company_id' => $company->tid, 'period' => ['from' => $period['from'], 'to' => $period['to']],
    'type' => DASH_TYPE_FACT, 'kinds' => [DASH_DEBT_KIND_CREDIT]
  ]);

  if ($plans || $facts) {
    // сгруппировать помесячно
    $plan_fact_by_month = [];
    foreach (dash_months_list_get($period) as $month) {
      $plan_fact_by_month[$month['start']] = [
        'plan_amount' => 0,
        'fact_amount' => 0,
        'start' => $month['start'],
        'end' => $month['end'],
      ];

      foreach ($plans as $record) {
        if ($record->date >= $month['start'] && $record->date < $month['end']) {
          $plan_fact_by_month[$month['start']]['plan_amount'] = $record->amount;
        }
      }
      foreach ($facts as $record) {
        if ($record->date >= $month['start'] && $record->date < $month['end']) {
          $plan_fact_by_month[$month['start']]['fact_amount'] = $record->amount;
        }
      }
    }
    ksort($plan_fact_by_month);

    $chart = array(
      '#type' => 'chart',
      '#chart_type' => 'line',
      '#colors' => [DASH_COLOR_RED, DASH_COLOR_BLUE],
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

    $max = 0;
    $min = 9999999;
    foreach ($plan_fact_by_month as $month) {
      $chart['xaxis']['#labels'][] = helper_month_label_ru(date('n', $month['start']));

      $plan_amount = -1 * $month['plan_amount'] / DASH_REV_DIVIDER;
      $fact_amount = -1 * $month['fact_amount'] / DASH_REV_DIVIDER;
      // если это текущий период,
      // то считаем и выводим факт только до месяца предстоящего текущему месяцу
      $fact_amount = ($month['start'] < gmmktime(0, 0, 0, date('n', $period['to']), 1, date('Y'))) ? $fact_amount : null;

      $chart['plan']['#data'][] = $plan_amount;
      $chart['fact']['#data'][] = $fact_amount;

      if ($fact_amount) {
        $max = max($fact_amount, $max);
        $min = min($fact_amount, $min);
      }
      if ($plan_amount) {
        $max = max($plan_amount, $max);
        $min = min($plan_amount, $min);
      }

      $tooltip = '<div style="padding: 8px 12px; min-width:140px;white-space: nowrap;">' .
                   '<div><div style="display:inline-block; font-weight: 700;">' . helper_month_label_ru(date('n', $month['start'])) . '</div></div>' .
                    '<div><div style="display:inline-block; width: 210px;">План:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($plan_amount, 2, ',', ' ') . '</b>&nbsp;млн.</div></div>' .
                    '<div><div style="display:inline-block; width: 210px;">Факт:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($fact_amount, 2, ',', ' ') . '</b>&nbsp;млн.</div></div>' .
                    '<div><div style="display:inline-block; width: 210px;">Выполнение:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . ($plan_amount ? number_format($fact_amount/$plan_amount*100, 0, ',', ' ') : '-') . '</b>&nbsp;%</div></div>' .
                  '</div>';

      $chart['fact'][] = array(
        '#type' => 'chart_data_item',
        '#title' => $tooltip,
        '#raw_options' => [
          'annotation' => $fact_amount ? number_format($fact_amount, 0, null, ' ') : null,
        ],
      );
      $chart['plan'][] = array(
        '#type' => 'chart_data_item',
        '#title' => $tooltip,
        '#raw_options' => [
          'annotation' => $plan_amount ? number_format($plan_amount, 0, null, ' ') : null,
        ],
      );
    }
  }

  $chart['yaxis']['#max'] = $max * 1.2; // нужен чтобы tooltip для высоких графиков помещался вверху и не моргал
  $chart['yaxis']['#min'] = $min * 1.25; // нужен чтобы tooltip для высоких графиков помещался вверху и не моргал

  return $chart;
}

function reports_remains_progress_table_output($period)
{
  $company = dash_term_get_by_1c_id('companies', DASH_INN_TD);

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
    'row3' => [],
  ];

  // собрать все записи Остатков готовой продукции и сырья
  $remains = dash_remains_brief_get_by_conditions([
    'company_id' => $company->tid, 'period' => ['from' => $prev_year_start, 'to' => $period['to']]
  ]);
  foreach ($remains as $remain) {
    if ($remain->category == 1) {
      $row_key = 'row1';
    } elseif ($remain->category == 2) {
      $row_key = 'row2';
    }

    $data[$row_key][$remain->date] = $remain->amount / DASH_REV_DIVIDER;
    if (!isset($data['row3'][$remain->date])) $data['row3'][$remain->name][$remain->date] = 0;
    $data['row3'][$remain->date] += $remain->amount / DASH_REV_DIVIDER;
  }

  // формируем таблицу
  $header = [
    ['data' => 'Наименование', 'align' => 'center', 'class' => ['td-name']],
    ['data' => date('d.m.Y', $prev_year_start), 'align' => 'center'],
    ['data' => date('d.m.Y', $prev_year_next_month_start), 'align' => 'center'],
    ['data' => date('d.m.Y', $year_start), 'align' => 'center'],
    ['data' => date('d.m.Y', $next_month_start), 'align' => 'center'],
  ];

  // если есть данные
  $rows = [];
  if ($data['row3']) {
    foreach ($data as $key => $row) {
      if ($key == 'row1') $name = 'Остатки готовой продукции и товаров (всего)';
      if ($key == 'row2') $name = 'Остатки сырья (всего)';
      if ($key == 'row3') $name = 'Итого ГП и сырьё';
      $rows[] = [
        'data' => [
          $name,
          ['data' => helper_number_format($row[$prev_year_start], 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($row[$prev_year_month_start], 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($row[$year_start], 1, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($row[$month_start], 1, ' ', false), 'align' => 'right'],
        ],
        'class' => [$key == 'row3' ? 'tfoot' : ''],
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

  return '<div class="remains-table">' . $output . '</div>';
}

function reports_column_registration_plan_fact($period)
{
  $chart = dash_no_data_chart_get();
  $company = dash_term_get_by_1c_id('companies', DASH_INN_TD);

  // собрать все записи за период для категории 198574 - расходы на регистрацию
  if ($data = dash_cash_outcome_get_by_conditions(['company_id' => $company->tid, 'period' => $period, 'category_ids' => [198574]])) {
    // сгруппировать помесячно
    $plan_fact_by_month = [];
    foreach (dash_months_list_get($period) as $month) {
      $plan_fact_by_month[$month['start']] = [
        'plan_amount' => 0,
        'fact_amount' => 0,
        'start' => $month['start'],
        'end' => $month['end'],
      ];

      foreach ($data as $record) {
        if ($month['start'] <= $record->date && $record->date < $month['end']) {
          if ($record->type == DASH_TYPE_FACT)  $plan_fact_by_month[$month['start']]['fact_amount'] += $record->amount;
          if ($record->type == DASH_TYPE_PLAN)  $plan_fact_by_month[$month['start']]['plan_amount'] += $record->amount;
        }
      }
    }
    ksort($plan_fact_by_month);

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

    foreach ($plan_fact_by_month as $month) {
      if ($month['plan_amount'] || $month['fact_amount']) {
        $chart['xaxis']['#labels'][] = helper_month_label_ru(date('n', $month['start']));

        $plan_amount = $month['plan_amount'] / DASH_REV_DIVIDER;
        $fact_amount = $month['fact_amount'] / DASH_REV_DIVIDER;

        $chart['plan']['#data'][] = $plan_amount;
        $chart['fact']['#data'][] = $fact_amount;

        $tooltip = '<div style="padding: 8px 12px; min-width:140px;white-space: nowrap;">' .
          '<div><div style="display:inline-block; font-weight: 700;">' . helper_month_label_ru(date('n', $month['start'])) . '</div></div>' .
          '<div><div style="display:inline-block; width: 210px;">План:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($plan_amount, 2, ',', ' ') . '</b>&nbsp;' . DASH_REV_SUFFIX . '</div></div>' .
          '<div><div style="display:inline-block; width: 210px;">Факт:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($fact_amount, 2, ',', ' ') . '</b>&nbsp;' . DASH_REV_SUFFIX . '</div></div>' .
          '<div><div style="display:inline-block; width: 210px;">Выполнение:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . ($plan_amount ? number_format($fact_amount/$plan_amount*100, 0, ',', ' ') : '-') . '</b>&nbsp;%</div></div>' .
          '</div>';

        $chart['fact'][] = array(
          '#type' => 'chart_data_item',
          '#title' => $tooltip,
          '#raw_options' => [
            'annotation' => $fact_amount ? number_format($fact_amount, 0, null, ' ') : null,
          ],
        );
        $chart['plan'][] = array(
          '#type' => 'chart_data_item',
          '#title' => $tooltip,
          '#raw_options' => [
            'annotation' => $plan_amount ? number_format($plan_amount, 0, null, ' ') : null,
          ],
        );
      }
    }
  }

  return $chart;
}

function reports_manual_bdr_table_output($period)
{
  $company = dash_term_get_by_1c_id('companies', DASH_INN_TD);

  // собрать все записи Прихода ДС за период
  $data = [];
  if ($bdr = dash_manual_bdr_get_by_conditions(['company_id' => $company->tid, 'period' => $period])) {
    foreach ($bdr as $record) {
      if (!isset($data[$record->category_id])) $data[$record->category_id] = ['name' => $record->category_name, 'plan_period' => 0, 'fact_period' => 0];
      if ($record->type == DASH_TYPE_PLAN) {
        $data[$record->category_id]['plan_period'] += $record->amount;
      }
      if ($record->type == DASH_TYPE_FACT) {
        $data[$record->category_id]['fact_period'] += $record->amount;
      }
    }
  }

  // формируем таблицу
  $header = [
    ['data' => 'Наименование', 'align' => 'center', 'class' => ['td-name']],
    ['data' => 'План (год)', 'align' => 'center'],
    ['data' => 'Факт (год)', 'align' => 'center'],
    ['data' => '% вып.', 'align' => 'center'],
  ];

  $rows = [];
  $terms = taxonomy_get_tree(taxonomy_vocabulary_machine_name_load('bdr')->vid);
  foreach ($terms as $term) {
    $item = $data[$term->tid];
    if ($item['plan_period'] || $item['fact_period']) {
      // сформировать имя с учётом иерархии
      $label = $term->name;
      if ($term->depth) {
        $label = '&nbsp;&nbsp;' . $label;
        for($i=1; $i <= $term->depth; $i++) { $label = '-' . $label; }
      }

      $rows[] = [
        'data' => [
          $label,
          ['data' => helper_number_format($item['plan_period'], 0, ' ', false), 'align' => 'right'],
          ['data' => helper_number_format($item['fact_period'], 0, ' ', false), 'align' => 'right'],
          ['data' => ($item['plan_period'] ? helper_number_format($item['fact_period'] / $item['plan_period'] * 100, 0, ' ', false) . ' %' : '-'), 'align' => 'right'],
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

  return '<div class="bdds-table">' . $output . '</div>';
}
