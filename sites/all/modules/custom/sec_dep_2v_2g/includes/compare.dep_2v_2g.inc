<?php

/**
 * ----- Сравнение периодов
 */
function dep_2v_2g_head_compare_page()
{
  $period = dash_season_bounds_upto_current_date_get(DASH_PERIOD_START_MONTH_TD, -1);

  $output =  '<div class="section-title">' .
                '<h3>Отгрузка</h3>' .
                '<div class="underline"></div>' .
                '<p>Сезоны ' . DASH_HISTORY_START_YEAR_TD . '-' . date('Y', $period['from']) . '</p>' .
              '</div>';

  $output .= '<div class="ajax-load" data-func="compare_1"><div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">Пожалуйста, подождите...</div></div></div>';

  return '<div class="charts" data-module="dep_2v_2g" data-period-from="' . $period['from'] . '" data-period-to="' . $period['to'] . '" data-period-end="' . $period['end'] . '">' . $output . '</div>';
}

function dep_2v_2g_compare_1($period = null)
{
  $period = $period ?? ['from' => $_GET['period_from'], 'to' => $_GET['period_to'], 'end' => $_GET['period_end']];

  $output = '<div class="row">';
  $data_arr =   dash_column_revenue_period_to_period('td', $period['from'], $period['to']);
  $output .=    '<div class="col-xs-12 col-md-6">' . drupal_render($data_arr) . '</div>';

  $data_arr =   dash_column_revenue_by_types('td', $period['from'], $period['to']);
  $output .=    '<div class="col-xs-12">' . drupal_render($data_arr) . '</div>';

  $output .= '</div>';

  return $output;
}

