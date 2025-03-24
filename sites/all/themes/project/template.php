<?php

/**
 * Implements hook_theme().
 */
function project_theme()
{
  return [
    'table_li' => array(
      'variables' => ['caption' => null, 'header' => null, 'rows' => null, 'attributes' => null],
    ),
  ];
}

function project_table_li($vars)
{
  $caption = $vars['caption'];
  $header = $vars['header'];
  $subheader = isset($vars['subheader']) ? $vars['subheader'] : [];
  $rows = $vars['rows'];
  $attributes = $vars['attributes'];

  $attributes['class'][] = 'table-li-responsive';

  $output = '<div' . drupal_attributes($attributes) . '>';
  $output .= $caption ? '<h5>' . $caption . '</h5>' : '';
  $output .=    '<ul>';

  if (!empty($header)) {
    $output .= '<li class="table-header">';
    foreach($header as $index => $cell) {
      if (!is_array($cell)) $cell = ['data' => $cell];
      $cell['attributes']['class'] = $cell['attributes']['class'] ?? [];
      array_unshift($cell['attributes']['class'], 'col-' . ($index + 1));
      array_unshift($cell['attributes']['class'], 'col');

      $data = $cell['data'] ?? $cell;

      $output .= '<div' . drupal_attributes($cell['attributes']) . '>' . $data . '</div>';
    }
    $output .= '</li>';
  }
  if (!empty($subheader)) {
    $output .= '<li class="table-subheader">';
    foreach($subheader as $index => $cell) {
      if (!is_array($cell)) $cell = ['data' => $cell];
      $cell['attributes']['class'] = $cell['attributes']['class'] ?? [];
      array_unshift($cell['attributes']['class'], 'col-' . ($index + 1));
      array_unshift($cell['attributes']['class'], 'col');

      $data = $cell['data'] ?? $cell;

      $output .= '<div' . drupal_attributes($cell['attributes']) . '>' . $data . '</div>';
    }
    $output .= '</li>';
  }

  if (!empty($rows)) {
    foreach ($rows as $row) {
      $row['attributes']['class'] = $row['attributes']['class'] ?? [];
      array_unshift($row['attributes']['class'], 'table-row');
      $output .= '<li' . drupal_attributes($row['attributes']) . '>';

      $row_data = $row['data'] ?? $row;
      $index = 0;
      foreach ($row_data as $cell) {
        if (!is_array($cell)) $cell = ['data' => $cell];
        $cell['attributes']['data-label'] = (isset($header[$index]) && is_array($header[$index])) ? $header[$index]['attributes']['data-label'] ?? $header[$index]['data'] : $header[$index];

        $cell['attributes']['class'] = $cell['attributes']['class'] ?? [];
        array_unshift($cell['attributes']['class'], 'col-' . ($index + 1));
        array_unshift($cell['attributes']['class'], 'col');
        if (empty($cell['data'])) $cell['attributes']['class'][] = 'col-empty';

        $data = $cell['data'] ?? $cell;

        $output .= '<div' . drupal_attributes($cell['attributes']) . '>' . $data . '</div>';
        $index++;
      }

      $output .= '</li>';
    }
  }

  if(!empty($vars['empty'])) {
    $output .= '<li class="col table-empty">' . $vars['empty'] . '</li>';
  }
  $output .=    '</ul>';
  $output .= '</div>';

  return $output;
}
