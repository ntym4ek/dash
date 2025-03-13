<?php

/**
 * ----- Текущий период
 */
function dep_2v_2g_head_period_page($closed = '')
{
  // Страница разбита на сектора, загружаемые через ajax,
  // так как формирование всей страницы целиком занимает продолжительное время.
  // Функция, отвечающая за содержимое, определяется атрибутом data-func

  $period = dash_get_page_time_bounds(DASH_PERIOD_START_MONTH_TD);

  $output = '<div class="section-title">';
  $output .=    '<h3>Отгрузка</h3>';
  $output .=    '<div class="underline"></div>';
  if (dash_is_current_season($period)) {
    $output .=  '<p>Год к году за период</p>';
  } else {
    $output .=  '<p>Год к году</p>';
  }
  $output .= '</div>';

  if (dash_is_current_season($period)) {
    $output .= '<div class="row">';
    $form =       drupal_get_form('dash_period_filter_form', ['wrapper' => 'summary-ship-td', 'callback' => 'dep_2v_2g_block_period_1', 'period' => $period]);
    $output .=   '<div class="col-xs-12 col-md-4">' . drupal_render($form) . '</div>';
    $output .= '</div>';
  }
  $output .= '<div class="ajax-load" data-func="block_period_1"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';

  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12"><h5 class="chart-title">Суммарная отгрузка</h5></div>';
  $form =      drupal_get_form('dep_2v_2g_td_column_plan_ship_period_to_period_filter_form');
  $output .=   '<div class="col-xs-12 col-md-4">' . drupal_render($form) . '</div>';
  $output .= '</div>';
  $output .= '<div class="ajax-load" data-func="block_period_2" data-delay="100"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';

  $output .= '<div class="section-title">';
  $output .= '<h3>Отгрузка</h3>';
  $output .=    '<div class="underline"></div>';
  if (dash_is_current_season($period)) {
    $output .= '<p>За текущий период</p>';
  } else {
    $output .= '<p>За период ' . date('Y', $period['from']) . ' - ' . date('Y', $period['end']) . '</p>';
  }
  $output .= '</div>';


  $output .= '<div class="row">';
  $output .=    '<div class="col-xs-12"><h5 class="chart-title">Отгрузка по номенклатуре (' . DASH_REV_SUFFIX . ')</h5></div>';
  $form =       drupal_get_form('dep_2v_2g_column_ship_plan_by_nom_filter_form');
  $output .=    '<div class="col-xs-12 col-md-4">' . drupal_render($form) . '</div>';
  $output .= '</div>';
  $output .= '<div class="ajax-load" data-func="block_period_3" data-delay="500"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';

  $output .=  '<div class="section-title">' .
                '<h3>Финансы</h3>' .
                '<div class="underline"></div>' .
                '<p>За период ' . date('Y', $period['from']) . '-' . date('Y', $period['end']) . '</p>' .
              '</div>';
  $output .= '<div class="ajax-load" data-func="block_period_4" data-delay="700"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';


  $output .=  '<div class="section-title">' .
              '<h3>Планы</h3>' .
              '<div class="underline"></div>' .
              '<p>На период ' . date('Y', $period['from']) . '-' . date('Y', $period['end']) . '</p>' .
              '</div>';

  // план-факт отгрузки по номенклатуре в литрах
  $output .= '<div class="row">';
  $output .=    '<div class="col-xs-12"><h5 class="chart-title">Выполнение плана отгрузки по номенклатуре (шт/л/кг)</h5></div>';
  $form =       drupal_get_form('dep_2v_2g_ship_plan_nom_filter_form');
  $output .=    '<div class="col-xs-12 col-md-4">' . drupal_render($form) . '</div>';
  $output .= '</div>';
  $output .= '<div class="ajax-load" data-func="block_period_5" data-delay="2000"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';

  // план-факт линейные
  $output .= '<div class="row">';
  $output .=    '<div class="col-xs-12"><h5 class="chart-title">Выполнение плана отгрузки (' . DASH_REV_SUFFIX . ')</h5></div>';
  $form =       drupal_get_form('dep_2v_2g_line_ship_plan_fact_by_month_filter_form');
  $output .=    '<div class="col-xs-12 col-md-4">' . drupal_render($form) . '</div>';
  $output .= '</div>';
  $output .= '<div class="ajax-load" data-func="block_period_6" data-delay="3000"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';

  // отгрузка по регионам
  $output .= '<div class="row">';
  $output .=    '<div class="col-xs-12"><h5 class="chart-title">Выполнение плана отгрузки по регионам (' . DASH_REV_SUFFIX . ')</h5></div>';
  $form =       drupal_get_form('dep_2v_2g_ship_plan_region_filter_form');
  $output .=    '<div class="col-xs-12 col-md-4">' . drupal_render($form) . '</div>';
  $output .= '</div>';
  $output .= '<div class="ajax-load" data-func="block_period_7" data-delay="4000"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';


  return '<div class="charts" data-module="dep_2v_2g" data-period-from="' . $period['from'] . '" data-period-to="' . $period['to'] . '" data-period-end="' . $period['end'] . '">' . $output . '</div>';
}

function dep_2v_2g_block_period_1($period = null)
{
  $period = $period ?? ['from' => $_GET['period_from'], 'to' => $_GET['period_to'], 'end' => $_GET['period_end']];

  $output  = '<div id="summary-ship-td" class="row">';
  $data_arr =   dash_column_revenue_period_to_period('td', $period['from'], $period['to']);
  $data_arr['#title'] = 'Суммарная отгрузка за сезон по данным бухгалтерии (' . DASH_REV_SUFFIX . ')';
  $output .=    '<div class="col-xs-12 col-md-6">' . drupal_render($data_arr) . '</div>';

  $data_arr =   dash_pie_revenue_year('td', $period['from'], $period['to']);
  $output .=    '<div class="col-xs-12 col-md-6">' . drupal_render($data_arr) . '</div>';
  $output .= '</div>';

  return $output;
}

function dep_2v_2g_block_period_2($period = null)
{
  $period = $period ?? ['from' => $_GET['period_from'], 'to' => $_GET['period_to'], 'end' => $_GET['period_end']];

  // отчёт в млн
  $output = '<div id="summary-target" class="row">';
  $data_arr =   dep_2v_2g_td_column_plan_ship_period_to_period($period, 'amount');
  $output .=    '<div class="col-xs-12 col-md-6">' . drupal_render($data_arr) . '</div>';

  // отчёт в литрах
  $data_arr =   dep_2v_2g_td_column_plan_ship_period_to_period($period, 'qty');
  $output .=    '<div class="col-xs-12 col-md-6">' . drupal_render($data_arr) . '</div>';
  $output .= '</div>';

  return $output;
}

function dep_2v_2g_block_period_3($period = null)
{
  $period = $period ?? ['from' => $_GET['period_from'], 'to' => $_GET['period_to'], 'end' => $_GET['period_end']];

  // выручка по номенклатуре
  $output = '<div class="row">';
  $data_arr =   dep_2v_2g_column_ship_plan_by_nom($period);
  $output .=    '<div id="nom-r-target" class="col-xs-12">' . drupal_render($data_arr) . '</div>';
  $output .= '</div>';

  return $output;
}

function dep_2v_2g_block_period_4($period = null)
{
  $period = $period ?? ['from' => $_GET['period_from'], 'to' => $_GET['period_to'], 'end' => $_GET['period_end']];

  // валовая прибыль, понедельно, из факта
  $output = '<div class="row">';
  $output .=    '<div class="col-xs-12"><h5 class="chart-title">Валовая прибыль, %</h5></div>';
  $data_arr =   dep_2v_2g_column_ship_fact_gross_profit_by_week($period);
  $output .=    '<div class="col-xs-12 col-md-12">' . drupal_render($data_arr) . '</div>';
  $output .= '</div>';

  return $output;
}

function dep_2v_2g_block_period_5($period = null)
{
  $period = $period ?? ['from' => $_GET['period_from'], 'to' => $_GET['period_to'], 'end' => $_GET['period_end']];

  // план-факт отгрузки по номенклатуре в литрах
  $output = '<div class="row">';
  $nom = empty($_GET['nom']) ? 0 : $_GET['nom'];
  $data_arr =   dep_2v_2g_column_ship_plan_fact_by_nom_single($period, $nom);
  $output .=    '<div id="nom-target" class="col-xs-12 col-md-4">' . drupal_render($data_arr) . '</div>';

  $options = ['range' => [0,7]];
  $data_arr =   dep_2v_2g_column_ship_plan_fact_by_nom($period, $options);
  $output .=    '<div class="col-xs-12 col-md-8">' . drupal_render($data_arr) . '</div>';
  $output .= '</div>';

  $output .= '<div class="row">';
  $options = ['range' => [8, 29]];
  $data_arr =   dep_2v_2g_column_ship_plan_fact_by_nom($period, $options);
  $output .=    '<div class="col-xs-12">' . drupal_render($data_arr) . '</div>';
  $output .= '</div>';

  return $output;
}

function dep_2v_2g_block_period_6($period = null)
{
  $period = $period ?? ['from' => $_GET['period_from'], 'to' => $_GET['period_to'], 'end' => $_GET['period_end']];

  // план-факт по отгрузкам помесячно линейный с подсветкой текущего значения к предыдущему на дату
  $output = '<div class="row">';
  $data_arr =   dep_2v_2g_line_ship_plan_fact_by_month($period);
  $output .=    '<div id="plan-target" class="col-xs-12">' . drupal_render($data_arr) . '</div>';
  $output .= '</div>';

  // план-факт выручки помесячно линейный с подсветкой текущего значения к предыдущему на дату
  // фильтр по регионам невозможен, тк в плане нет детализации по клиентам
  $output .= '<div class="row">';
  $output .=    '<div class="col-xs-12"><h5 class="chart-title">Выполнение плана поступления денежных средств (' . DASH_REV_SUFFIX . ')</h5></div>';
  $data_arr =   dep_2v_2g_line_revenue_plan_fact('plan_revenue', $period);
  $output .=    '<div id="plan-m-target" class="col-xs-12">' . drupal_render($data_arr) . '</div>';
  $output .= '</div>';

  return $output;
}

function dep_2v_2g_block_period_7($period = null)
{
  $period = $period ?? ['from' => $_GET['period_from'], 'to' => $_GET['period_to'], 'end' => $_GET['period_end']];

  // отгрузка по регионам
  $output = '<div class="row">';
  $region = empty($_GET['region']) ? 0 : $_GET['region'];
  $data_arr =   dep_2v_2g_bar_ship_plan_fact_by_region_single($period, $region);
  $output .=    '<div id="region-target" class="col-xs-12">' . drupal_render($data_arr) . '</div>';
  $data_arr =   dep_2v_2g_bar_ship_plan_fact_by_region($period, ['regions' => ['list' => [657]]]); // ЦО
  $output .=    '<div class="col-xs-12">' . drupal_render($data_arr) . '</div>';

  $data_arr =   dep_2v_2g_bar_ship_plan_fact_by_region($period, ['regions' => ['exclude' => [657]]]);
  $output .=    '<div class="col-xs-12">' . drupal_render($data_arr) . '</div>';
  $output .= '</div>';

  return $output;
}
