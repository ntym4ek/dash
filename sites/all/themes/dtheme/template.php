<?php
/**
 * @file
 * The primary PHP file for this theme.
 */

/**
 * Implements hook_theme().
 */
function dtheme_theme()
{
  return [
    'table_li' => array(
      'variables' => ['caption' => null, 'header' => null, 'rows' => null, 'attributes' => null],
    ),
  ];
}

/**
 * Pre-processes variables for the "region" theme hook.
 */
function dtheme_preprocess_region(array &$vars)
{
  $region = $vars['region'];

  // Content region.
  if ($region === 'content') {
    // @todo is this actually used properly?
    $vars['theme_hook_suggestions'][] = 'region__no_wrapper';
  }
}

/**
 * Returns HTML for a region.
 */
function dtheme_region__no_wrapper(&$vars)
{
  $elements = $vars['elements'];

  return $elements['#children'];
}

/**
 * Implements hook_preprocess_page().
 */
function dtheme_preprocess_page(&$vars)
{
  // сменить шаблон страницы на пустой
//  if (arg(0) == 'card') {
//    $vars['theme_hook_suggestions'][] = 'page__empty';
//  }

  if (isset($vars['main_menu'])) {
    $main_menu =  menu_tree('main-menu');
    $main_menu['#attributes']['class'] = ['main-menu'];
    $vars['primary_nav'] = render($main_menu);
  }
  else {
    $vars['primary_nav'] = FALSE;
  }

  if (isset($vars['secondary_menu'])) {
    $main_menu =  menu_tree('user-menu');
    $main_menu['#attributes']['class'] = ['secondary-menu'];
    $vars['secondary_nav'] = render($main_menu);
  }
  else {
    $vars['secondary_nav'] = FALSE;
  }


  /** --------------------------  подключить FontAwesome 5 - */
//  drupal_add_css('https://use.fontawesome.com/releases/v5.11.0/css/all.css', array('type' => 'external'));

  /** --------------------------  подключить Ubuntu - */
  drupal_add_css('https://fonts.googleapis.com/css2?family=Ubuntu:wght@300;400;700&display=swap', array('type' => 'external'));
}


/**
 * Pre-processes variables for the "block" theme hook.
 */
function dtheme_preprocess_block(array &$vars)
{
  // Use a bare template for the page's main content.
  if ($vars['block_html_id'] == 'block-system-main') {
    $vars['theme_hook_suggestions'][] = 'block__no_wrapper';
  }
  $vars['title_attributes_array']['class'][] = 'block-title';
}

/**
 * Returns HTML for a block.
 */
function dtheme_block__no_wrapper(&$vars)
{
  $elements = $vars['elements'];

  return $elements['#children'];
}

function dtheme_table_li($vars)
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
        if (empty($cell['data'])) $cell['attributes']['class'][] = 'empty';

        $data = $cell['data'] ?? $cell;

        $output .= '<div' . drupal_attributes($cell['attributes']) . '>' . $data . '</div>';
        $index++;
      }

      $output .= '</li>';
    }
  }

  $output .=    '</ul>';
  $output .= '</div>';

  return $output;
}

/**
 * Implements theme_menu_tree().
 */
function dtheme_menu_tree(&$vars)
{
  $attributes = $vars['#tree']['#attributes'] ?? ['class' => []];

  // определить глубину уровня
  $depth = 0;
  foreach ($vars["#tree"] as $item) {
    $depth = $item["#original_link"]["depth"] ?? $depth;
  }
  $attributes['class'][] = $depth > 1 ? 'sub-menu' : 'menu';
  $attributes['class'][] = 'level-' . $depth;


  return '<ul' . drupal_attributes($attributes) . '>' . $vars['tree'] . '</ul>';
}
