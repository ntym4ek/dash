<?php

/**
 * Вывод графиков для Менеджеров управления
 *
 * ----- Текущий период
 */
function dep_2v_2g_manager_period_page()
{
  // страница разбита на сектора, загружаемые через ajax,
  // так как формирование всей страницы целиком занимает продолжительное время

  $period = dash_page_time_bounds_get(DASH_PERIOD_START_MONTH_TD);

  $time_yesterday = gmmktime(0, 0, 0, date('n', $period['to']), date('j', $period['to'])-1, date('Y', $period['to']));
  $output = '<div class="section-title">';
  $output .= '<h3>Отгрузка</h3>';
  $output .= '<div class="underline"></div>';
  if (dash_is_current_season($period)) {
    $output .= '<p>Год к году за период с ' . date('d', $period['from']) . '.' . date('m', $period['from']) . ' по ' . date('d', $time_yesterday) . '.' . date('m', $time_yesterday) . '</p>';
  } else {
    $output .= '<p>Год к году';
  }
  $output .= '</div>';

  $output .= '<div class="row">';
  $output .=   '<div class="col-xs-12"><h5 class="chart-title">Суммарная отгрузка</h5></div>';
  $form =      drupal_get_form('dep_2v_2g_td_column_plan_ship_period_to_period_filter_form');
  $output .=   '<div class="col-xs-12 col-md-4">' . drupal_render($form) . '</div>';
  $output .= '</div>';
  $output .= '<div class="ajax-load" data-func="manager_2" data-delay="100"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';

  $output .= '<div class="section-title">';
  $output .= '<h3>Отгрузка</h3>';
  $output .= '<div class="underline"></div>';
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
  $output .= '<div class="ajax-load" data-func="manager_3" data-delay="500"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';


  $output .=  '<div class="section-title">' .
                '<h3>Планы</h3>' .
                '<div class="underline"></div>' .
                '<p>На период  ' . date('Y', $period['from']) . '-' . date('Y', $period['end']) . '</p>' .
              '</div>';

  // план-факт линейные
  $output .= '<div class="row">';
  $output .=    '<div class="col-xs-12"><h5 class="chart-title">Выполнение плана отгрузки (' . DASH_REV_SUFFIX . ')</h5></div>';
  $form =       drupal_get_form('dep_2v_2g_line_ship_plan_fact_by_month_filter_form');
  $output .=    '<div class="col-xs-12 col-md-4">' . drupal_render($form) . '</div>';
  $output .= '</div>';
  $output .= '<div class="ajax-load" data-func="manager_6" data-delay="3000"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';

  // отгрузка по регионам
  $output .= '<div class="row">';
  $output .=    '<div class="col-xs-12"><h5 class="chart-title">Выполнение плана отгрузки по регионам (' . DASH_REV_SUFFIX . ')</h5></div>';
  $form =       drupal_get_form('dep_2v_2g_ship_plan_amount_region_filter_form');
  $output .=    '<div class="col-xs-12 col-md-4">' . drupal_render($form) . '</div>';
  $output .= '</div>';
  $output .= '<div class="ajax-load" data-func="manager_7" data-delay="4000"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';


  return '<div class="charts" data-module="dep_2v_2g" data-period-from="' . $period['from'] . '" data-period-to="' . $period['to'] . '" data-period-end="' . $period['end'] . '">' . $output . '</div>';
}

function dep_2v_2g_manager_2($period = null)
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

function dep_2v_2g_manager_3($period = null)
{
  $period = $period ?? ['from' => $_GET['period_from'], 'to' => $_GET['period_to'], 'end' => $_GET['period_end']];

  // выручка по номенклатуре
  $output = '<div class="row">';
  $data_arr =   dep_2v_2g_column_ship_plan_by_nom($period);
  $output .=    '<div id="nom-r-target" class="col-xs-12">' . drupal_render($data_arr) . '</div>';
  $output .= '</div>';

  return $output;
}

function dep_2v_2g_manager_6($period = null)
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

function dep_2v_2g_manager_7($period = null)
{
  $period = $period ?? ['from' => $_GET['period_from'], 'to' => $_GET['period_to'], 'end' => $_GET['period_end']];

  // отгрузка по регионам
  $output = '<div class="row">';
  $regions = empty($_GET['region']) ? [] : [$_GET['region']];
  $data_arr =   dep_2v_2g_bar_ship_plan_fact_by_region_single($period, ['regions' => ['list' => $regions]]);
  $output .=    '<div id="region-amount-target" class="col-xs-12">' . drupal_render($data_arr) . '</div>';
  $data_arr =   dep_2v_2g_bar_ship_plan_fact_by_region($period, ['regions' => ['list' => [657]]]); // ЦО
  $output .=    '<div class="col-xs-12">' . drupal_render($data_arr) . '</div>';

  $data_arr =   dep_2v_2g_bar_ship_plan_fact_by_region($period, ['regions' => ['exclude' => [657]]]);
  $output .=    '<div class="col-xs-12">' . drupal_render($data_arr) . '</div>';
  $output .= '</div>';

  return $output;
}
