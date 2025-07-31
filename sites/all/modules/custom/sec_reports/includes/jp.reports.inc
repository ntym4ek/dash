<?php

function reports_jp_period_form($form, &$form_state)
{
  // дефолтные значения
  $year_default = date('Y');
  $year         = empty($form_state["values"]['year'])    ? $year_default : $form_state["values"]['year'];

  $form = [
    '#prefix' => '<div id="period-form-wrapper">',
    '#suffix' => '</div>',
    'filters' => [
      '#type' => 'container',
      '#attributes' => ['class' => ['filters']],
    ],
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
      'callback' => 'reports_jp_period_form_callback',
      'wrapper' => 'period-form-wrapper',
    ],
    '#prefix' => '<div class="row"><div class="col-xs-12 col-md-4"><div class="filter"><div class="period-select-wrapper">',
    '#suffix' => '</div></div></div></div>',
  ];

  $form['content'] = [
    '#markup' => reports_jp_page_content($year)
  ];

  return $form;
}
function reports_jp_period_form_callback($form, &$form_state)
{
  return $form;
}

function reports_jp_page_content($year)
{
  $period = [
    'from' => gmmktime(0, 0, 0, 1, 1, $year),
    'to' => gmmktime(0, 0, 0, date('n'), 1, $year),
    'end' => gmmktime(0, 0, 0, 1, 1, $year+1),
  ];

  // слайд 1
  // Поступление ДС, год, помесячно, план-факт
  $url = url('/data/cash-income/edit', ['query' => ['blocks' => ['jp_block_1', 'jp_block_3'], 'inn' => DASH_INN_ST]]);
  $output  = '<div class="section-title">' .
                '<h3>План-факт поступления денежных средств за ' . date('Y') . ' (в ' . DASH_REV_SUFFIX . ')</h3>' .
                '<a class="edit-link autodialog" data-dialog-width="450" href="' . $url . '">Редактировать данные</a>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '';
  $output .= '<div class="ajax-load" data-func="jp_block_1" data-delay="100"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';

  // слайд 2
  // Отгрузка в деньгах, год, помесячно, план-факт
  $url = url('/data/ship/edit', ['query' => ['blocks' => ['jp_block_2', 'jp_block_3'], 'inn' => DASH_INN_ST]]);
  $output .= '<div class="section-title">' .
                '<h3>План-факт отгрузки продукции за ' . date('Y') . ' (в ' . DASH_REV_SUFFIX . ')</h3>' .
                '<a class="edit-link autodialog" data-dialog-width="550" href="' . $url . '">Редактировать данные</a>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '';
  $output .= '<div class="ajax-load" data-func="jp_block_2" data-delay="200"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';

  // слайд 3
  // сравнение факта Поступления ДС и Отгрузки в деньгах, год, помесячно
  $url = url('/data/ship/edit', ['query' => ['blocks' => ['jp_block_3'], 'inn' => DASH_INN_ST]]);
  $output .= '<div class="section-title">' .
                '<h3>Поступление ДС и отгрузка продукции за ' . date('Y') . ' (в ' . DASH_REV_SUFFIX . ')</h3>' .
                '<div class="underline"></div>' .
              '</div>';
  $output .= '';
  $output .= '<div class="ajax-load" data-func="jp_block_3" data-delay="300"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';

  return '<div class="charts" data-module="reports" data-period-from="' . $period['from'] . '" data-period-to="' . $period['to'] . '" data-period-end="' . $period['end'] . '">' . $output . '</div>';
}

function reports_jp_block_1()
{
  // период с начала года по начало текущего месяца за заданный год
  $period = [
    'from' => $_GET["period_from"],
    'to' => $_GET["period_to"],
    'end' => $_GET["period_end"],
  ];

  // план-факт поступления ДС
  $data_arr = reports_jp_column_cash_plan_fact($period);

  return drupal_render($data_arr);
}

function reports_jp_block_2()
{
  // период с начала года по начало текущего месяца за заданный год
  $period = [
    'from' => $_GET["period_from"],
    'to' => $_GET["period_to"],
    'end' => $_GET["period_end"],
  ];

  // план-факт отгрузки продукции
  $data_arr = reports_jp_column_ship_plan_fact($period);

  return drupal_render($data_arr);
}

function reports_jp_block_3()
{
  // период с начала года по начало текущего месяца за заданный год
  $period = [
    'from' => $_GET["period_from"],
    'to' => $_GET["period_to"],
    'end' => $_GET["period_end"],
  ];

  // сравнение факта Поступления ДС и Отгрузки в деньгах, год, помесячно
  $data_arr = reports_jp_line_cash_ship_fact($period);

  return drupal_render($data_arr);
}

function reports_jp_column_cash_plan_fact($period)
{
  $chart = dash_no_data_chart_get();

  // собрать все записи Страды за период
  $company = dash_term_get_by_1c_id('companies', DASH_INN_ST);
  if ($data = dash_cash_income_get_by_conditions(['company_id' => $company->tid, 'period' => $period, 'manual' => 1])) {
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
        if ($record->date >= $month['start'] && $record->date < $month['end']) {
          if ($record->type == DASH_TYPE_PLAN) $plan_fact_by_month[$month['start']]['plan_amount'] += $record->amount;
          if ($record->type == DASH_TYPE_FACT) $plan_fact_by_month[$month['start']]['fact_amount'] += $record->amount;
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

        $plan = $month['plan_amount'] / DASH_REV_DIVIDER;
        $fact = $month['fact_amount'] / DASH_REV_DIVIDER;

        $chart['plan']['#data'][] = $plan;
        $chart['fact']['#data'][] = $fact;

        $tooltip = '<div style="padding: 8px 12px; min-width:140px;white-space: nowrap;">' .
          '<div><div style="display:inline-block; font-weight: 700;">' . helper_month_label_ru(date('n', $month['start'])) . '</div></div>' .
          '<div><div style="display:inline-block; width: 210px;">План:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($plan, 2, ',', ' ') . '</b>&nbsp;' . DASH_REV_SUFFIX . '</div></div>' .
          '<div><div style="display:inline-block; width: 210px;">Факт:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($fact, 2, ',', ' ') . '</b>&nbsp;' . DASH_REV_SUFFIX . '</div></div>' .
          '<div><div style="display:inline-block; width: 210px;">Выполнение:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . ($plan ? number_format($fact/$plan*100, 0, ',', ' ') : '-') . '</b>&nbsp;%</div></div>' .
          '</div>';

        $chart['fact'][] = array(
          '#type' => 'chart_data_item',
          '#title' => $tooltip,
          '#raw_options' => [
            'annotation' => $fact ? number_format($fact, 2, ',', ' ') : null,
          ],
        );
        $chart['plan'][] = array(
          '#type' => 'chart_data_item',
          '#title' => $tooltip,
          '#raw_options' => [
            'annotation' => $plan ? number_format($plan, 2, ',', ' ') : null,
          ],
        );
      }
    }
  }

  return $chart;
}
function reports_jp_column_ship_plan_fact($period)
{
  $chart = dash_no_data_chart_get();

  // собрать все записи Страды за период
  $company = dash_term_get_by_1c_id('companies', DASH_INN_ST);
  if ($data = dash_ship_get_by_conditions(['company_id' => $company->tid, 'period' => $period, 'manual' => 1])) {
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
        if ($record->date >= $month['start'] && $record->date < $month['end']) {
          if ($record->type == DASH_TYPE_PLAN) $plan_fact_by_month[$month['start']]['plan_amount'] += $record->amount;
          if ($record->type == DASH_TYPE_FACT) $plan_fact_by_month[$month['start']]['fact_amount'] += $record->amount;
        }
      }
    }
    ksort($plan_fact_by_month);

    $chart = array(
      '#type' => 'chart',
      '#chart_type' => 'column',
      '#colors' => ['#ccc', DASH_COLOR_AQUA],
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

        $plan = $month['plan_amount'] / DASH_REV_DIVIDER;
        $fact = $month['fact_amount'] / DASH_REV_DIVIDER;

        $chart['plan']['#data'][] = $plan;
        $chart['fact']['#data'][] = $fact;

        $tooltip = '<div style="padding: 8px 12px; min-width:140px;white-space: nowrap;">' .
          '<div><div style="display:inline-block; font-weight: 700;">' . helper_month_label_ru(date('n', $month['start'])) . '</div></div>' .
          '<div><div style="display:inline-block; width: 210px;">План:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($plan, 2, ',', ' ') . '</b>&nbsp;' . DASH_REV_SUFFIX . '</div></div>' .
          '<div><div style="display:inline-block; width: 210px;">Факт:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($fact, 2, ',', ' ') . '</b>&nbsp;' . DASH_REV_SUFFIX . '</div></div>' .
          '<div><div style="display:inline-block; width: 210px;">Выполнение:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . ($plan ? number_format($fact/$plan*100, 0, ',', ' ') : '-') . '</b>&nbsp;%</div></div>' .
          '</div>';

        $chart['fact'][] = array(
          '#type' => 'chart_data_item',
          '#title' => $tooltip,
          '#raw_options' => [
            'annotation' => $fact ? number_format($fact, 2, ',', ' ') : null,
          ],
        );
        $chart['plan'][] = array(
          '#type' => 'chart_data_item',
          '#title' => $tooltip,
          '#raw_options' => [
            'annotation' => $plan ? number_format($plan, 2, ',', ' ') : null,
          ],
        );
      }
    }
  }

  return $chart;
}
function reports_jp_line_cash_ship_fact($period)
{
  $chart = dash_no_data_chart_get();

  // собрать все записи Страды за период
  $company = dash_term_get_by_1c_id('companies', DASH_INN_ST);
  $cash_income  = dash_cash_income_get_by_conditions(['company_id' => $company->tid, 'period' => $period, 'type' => DASH_TYPE_FACT, 'manual' => 1]);
  $ship         = dash_ship_get_by_conditions(['company_id' => $company->tid, 'period' => $period, 'type' => DASH_TYPE_FACT, 'manual' => 1]);
  if ($cash_income || $ship) {
    // сгруппировать помесячно
    // todo не оптимальный обход, надо бы переделать
    $cash_ship_by_month = [];
    foreach (dash_months_list_get($period) as $month) {
      $cash_ship_by_month[$month['start']] = [
        'cash_fact' => 0,
        'ship_fact' => 0,
        'start' => $month['start'],
        'end' => $month['end'],
      ];

      foreach ($cash_income as $record) {
        if ($record->date >= $month['start'] && $record->date < $month['end']) {
         $cash_ship_by_month[$month['start']]['cash_fact'] += $record->amount;
        }
      }
      foreach ($ship as $record) {
        if ($record->date >= $month['start'] && $record->date < $month['end']) {
         $cash_ship_by_month[$month['start']]['ship_fact'] += $record->amount;
        }
      }
    }
    ksort($cash_ship_by_month);

    $chart = array(
      '#type' => 'chart',
      '#chart_type' => 'line',
      '#colors' => [DASH_COLOR_BLUE, DASH_COLOR_AQUA],
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
      'cash' => [
        '#type' => 'chart_data',
        '#title' => 'Поступления ДС',
        '#data' => [],
      ],
      'ship' => [
        '#type' => 'chart_data',
        '#title' => 'Отгрузка продукции',
        '#data' => [],
      ],
    );

    foreach ($cash_ship_by_month as $month) {
      if ($month['cash_fact'] || $month['ship_fact']) {
        $chart['xaxis']['#labels'][] = helper_month_label_ru(date('n', $month['start']));

        $cash_amount = $month['cash_fact'] / DASH_REV_DIVIDER;
        $ship_amount = $month['ship_fact'] / DASH_REV_DIVIDER;

        $chart['cash']['#data'][] = $cash_amount;
        $chart['ship']['#data'][] = $ship_amount;

        $tooltip =  '<div style="padding: 8px 12px; min-width:140px;white-space: nowrap;">' .
                      '<div><div style="display:inline-block; font-weight: 700;">' . helper_month_label_ru(date('n', $month['start'])) . '</div></div>' .
                      '<div><div style="display:inline-block; width: 210px;">Поступления:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($cash_amount, 2, ',', ' ') . '</b>&nbsp;' . DASH_REV_SUFFIX . '</div></div>' .
                      '<div><div style="display:inline-block; width: 210px;">Отгрузки:</div><div style="display:inline-block; text-align: right; width: 110px;"><b>' . number_format($ship_amount, 2, ',', ' ') . '</b>&nbsp;' . DASH_REV_SUFFIX . '</div></div>' .
                    '</div>';

        $chart['cash'][] = array(
          '#type' => 'chart_data_item',
          '#title' => $tooltip,
          '#raw_options' => [
            'annotation' => $cash_amount ? number_format($cash_amount, 2, ',', ' ') : null,
          ],
        );
        $chart['ship'][] = array(
          '#type' => 'chart_data_item',
          '#title' => $tooltip,
          '#raw_options' => [
            'annotation' => $ship_amount ? number_format($ship_amount, 2, ',', ' ') : null,
          ],
        );
      }
    }
  }

  return $chart;
}
