<?php

function reports_debt_debitors_table_output($period, $inn)
{
  $company = dash_term_get_by_1c_id('companies', $inn);

  $next_month_start = $period['to'];
  $prev_year_next_month_start = gmmktime(0, 0, 0, date('n', $next_month_start), 1, date('Y', $next_month_start) - 1);

  $year_start = $period['from'];
  $prev_year_start = gmmktime(0, 0, 0, 1, 1, date('Y', $period['from'])-1);

  // В отчёте за месяц мы показываем данные на 1 число следующего месяца,
  // то есть период выборки данных нужно увеличить на один месяц вперед.
  $period_to = strtotime('next month', $next_month_start);

  $data = [
    'row1' => [],
    'kccc' => [],
    'row2' => [],
    'row3' => [],
  ];

  // собрать все записи Дебиторской задолженности (category = 1)
  $debts = dash_debt_get_by_conditions([
    'company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'kinds' => [DASH_DEBT_KIND_DEBITOR],
    'period' => ['from' => $prev_year_start, 'to' => $period_to]
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

  $rows = [];
  foreach ($data as $key => $row) {
    foreach ($row as $name => $item) {
      if (!empty($item[$prev_year_start]) || !empty($item[$prev_year_next_month_start]) || !empty($item[$year_start]) || !empty($item[$next_month_start])) {
        $rows[] = [
          'data' => [
            ($key == 'kccc' ? '&nbsp;&nbsp;- ' : '') . $name,
            ['data' => helper_number_format($item[$prev_year_start] ?? 0, 2, ' ', false), 'align' => 'right'],
            ['data' => helper_number_format($item[$prev_year_next_month_start] ?? 0, 2, ' ', false), 'align' => 'right'],
            ['data' => helper_number_format($item[$year_start] ?? 0, 2, ' ', false), 'align' => 'right'],
            ['data' => helper_number_format($item[$next_month_start] ?? 0, 2, ' ', false), 'align' => 'right'],
          ],
          'class' => [$key == 'row3' ? 'tfoot' : ''],
        ];
      }
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

function reports_debt_creditors_table_output($period, $inn)
{
  $company = dash_term_get_by_1c_id('companies', $inn);

  $next_month_start = $period['to'];
  $prev_year_next_month_start = gmmktime(0, 0, 0, date('n', $next_month_start), 1, date('Y', $next_month_start) - 1);

  $year_start = $period['from'];
  $prev_year_start = gmmktime(0, 0, 0, 1, 1, date('Y', $period['from'])-1);

  // В отчёте за месяц мы показываем данные на 1 число следующего месяца,
  // то есть период выборки данных нужно увеличить на один месяц вперед.
  $period_to = strtotime('next month', $next_month_start);

  $data = [
    'row1' => [],
    'kccc' => [],
    'row2' => [],
    'row3' => [],
  ];

  // собрать все записи Кредиторской задолженности (category = 2)
  $credits = dash_debt_get_by_conditions([
    'company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'kinds' => [DASH_DEBT_KIND_CREDITOR],
    'period' => ['from' => $prev_year_start, 'to' => $period_to]
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
      if (!empty($item[$prev_year_start]) || !empty($item[$prev_year_next_month_start]) || !empty($item[$year_start]) || !empty($item[$next_month_start])) {
        $rows[] = [
          'data' => [
            ($key == 'kccc' ? '&nbsp;&nbsp;- ' : '') . $name,
            ['data' => helper_number_format($item[$prev_year_start] ?? 0, 2, ' ', false), 'align' => 'right'],
            ['data' => helper_number_format($item[$prev_year_next_month_start] ?? 0, 2, ' ', false), 'align' => 'right'],
            ['data' => helper_number_format($item[$year_start] ?? 0, 2, ' ', false), 'align' => 'right'],
            ['data' => helper_number_format($item[$next_month_start] ?? 0, 2, ' ', false), 'align' => 'right'],
          ],
          'class' => [$key == 'row3' ? 'tfoot' : ''],
        ];
      }
    }
  }

  $output = theme('table', [
    'header' => $header,
    'rows' => $rows,
    'attributes' => ['class' => ['table', 'table-intersect']],
    'sticky' => false,
    'empty' => 'нет данных',
  ]);

  return '<div class="debt-creditors-table scrollable">' . $output . '</div>';
}

function reports_debt_credits_loans_table_output($period, $inn)
{
  $company = dash_term_get_by_1c_id('companies', $inn);

  $next_month_start = $period['to'];
  $prev_year_next_month_start = gmmktime(0, 0, 0, date('n', $next_month_start), 1, date('Y', $next_month_start) - 1);

  $year_start = $period['from'];
  $prev_year_start = gmmktime(0, 0, 0, 1, 1, date('Y', $period['from'])-1);

  // В отчёте за месяц мы показываем данные на 1 число следующего месяца,
  // то есть период выборки данных нужно увеличить на один месяц вперед.
  $period_to = strtotime('next month', $next_month_start);

  $data = [
    'row1' => [],
    'row2' => [],
    'kccc' => [],
  ];

  // собрать все записи по всем кредитам и займам (kind = DASH_DEBT_KIND_CREDIT, DASH_DEBT_KIND_LOAN)
  $debts  = dash_debt_get_by_conditions([
    'company_id' => $company->tid, 'type' => DASH_TYPE_FACT, 'kinds' => [DASH_DEBT_KIND_CREDIT, DASH_DEBT_KIND_LOAN],
    'period' => ['from' => $prev_year_start, 'to' => $period_to]
  ]);
  $loan = 'Внутренние займы всего с %, в т.ч.:';
  foreach($debts as $debt) {
    if ($debt->kind == DASH_DEBT_KIND_CREDIT) {
      $data['row1'][$debt->name][$debt->date] = $debt->amount / DASH_REV_DIVIDER;
    }
    elseif ($debt->kind == DASH_DEBT_KIND_LOAN && $debt->category == 2) {
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
      if (!empty($item[$prev_year_start]) || !empty($item[$prev_year_next_month_start]) || !empty($item[$year_start]) || !empty($item[$next_month_start])) {
        $rows[] = [
          'data' => [
            ($key == 'kccc' ? '&nbsp;&nbsp;- ': '') . $name,
            ['data' => helper_number_format($item[$prev_year_start] ?? 0, 2, ' ', false), 'align' => 'right'],
            ['data' => helper_number_format($item[$prev_year_next_month_start] ?? 0, 2, ' ', false), 'align' => 'right'],
            ['data' => helper_number_format($item[$year_start] ?? 0, 2, ' ', false), 'align' => 'right'],
            ['data' => helper_number_format($item[$next_month_start] ?? 0, 2, ' ', false), 'align' => 'right'],
            ['data' => isset($item['percent'][$next_month_start]) ? helper_number_format($item['percent'][$next_month_start], 1, ' ', false) : '', 'align' => 'right'],
          ],
          'class' => [$key == 'row2' ? 'tfoot': ''],
        ];
      }
    }
  }

  $output = theme('table', [
    'header' => $header,
    'rows' => $rows,
    'attributes' => ['class' => ['table', 'table-intersect']],
    'sticky' => false,
    'empty' => 'нет данных',
  ]);

  return '<div class="credits-loans-table scrollable">' . $output . '</div>';
}

function reports_remains_progress_table_output($period, $inn)
{
  $company = dash_term_get_by_1c_id('companies', $inn);

  $next_month_start = $period['to'];
  $prev_year_next_month_start = gmmktime(0, 0, 0, date('n', $next_month_start), 1, date('Y', $next_month_start) - 1);

  $year_start = $period['from'];
  $prev_year_start = gmmktime(0, 0, 0, 1, 1, date('Y', $period['from'])-1);

  // В отчёте за месяц мы показываем данные на 1 число следующего месяца,
  // то есть период выборки данных нужно увеличить на один месяц вперед.
  $period_to = strtotime('next month', $next_month_start);

  $data = [
    'row1' => [],
    'row2' => [],
    'row3' => [],
  ];

  // собрать все записи Остатков готовой продукции и сырья
  $remains = dash_remains_brief_get_by_conditions([
    'company_id' => $company->tid, 'period' => ['from' => $prev_year_start, 'to' => $period_to]
  ]);
  foreach ($remains as $remain) {
    if ($remain->category == 1) {
      $row_key = 'row1';
    } elseif ($remain->category == 2) {
      $row_key = 'row2';
    }

    $data[$row_key][$remain->date] = $remain->amount / DASH_REV_DIVIDER;
    $data['row3'][$remain->date] = ($data['row3'][$remain->date] ?? 0) + $remain->amount / DASH_REV_DIVIDER;
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
      if (!empty($row[$prev_year_start]) || !empty($row[$prev_year_next_month_start]) || !empty($row[$year_start]) || !empty($row[$next_month_start])) {
        if ($key == 'row1') $name = 'Остатки готовой продукции и товаров (всего)';
        if ($key == 'row2') $name = 'Остатки сырья (всего)';
        if ($key == 'row3') $name = 'Итого ГП и сырьё';
        $rows[] = [
          'data' => [
            $name,
            ['data' => helper_number_format($row[$prev_year_start] ?? 0, 2, ' ', false), 'align' => 'right'],
            ['data' => helper_number_format($row[$prev_year_next_month_start] ?? 0, 2, ' ', false), 'align' => 'right'],
            ['data' => helper_number_format($row[$year_start] ?? 0, 2, ' ', false), 'align' => 'right'],
            ['data' => helper_number_format($row[$next_month_start] ?? 0, 2, ' ', false), 'align' => 'right'],
          ],
          'class' => [$key == 'row3' ? 'tfoot' : ''],
        ];
      }
    }
  }
  $output = theme('table', [
    'header' => $header,
    'rows' => $rows,
    'attributes' => ['class' => ['table', 'table-intersect']],
    'sticky' => false,
    'empty' => 'нет данных',
  ]);

  return '<div class="remains-table scrollable">' . $output . '</div>';
}


/**
 * Данную таблицу Бобков заполняет не накопительным итогом, а итоговыми значениями,
 * то есть суммировать не нужно. Берём последние значения.
 *
 * Нужно учитывать, что данные за месяц вносятся в конце следующего месяца.
 * Например, в таблице "План-факт. Бюджет доходов и расходов" (block_13)
 * при выводе данных на начало октября нужно выводить за август.
 */
function reports_manual_bdr_table_output($period, $inn)
{
  // Данные выводятся за месяц, предшествующий отчётному на -2,
  // например, если отчётный месяц Февраль, то данные выводятся за Декабрь.
  $month_num = date('n', $period['to']);
  $month_start = gmmktime(0, 0, 0, $month_num-2, 1, date('Y', $period['to']));

  // переопределить Отчётный период,
  // так как, например, для Февраля период, за который нужно выбирать данные - прошлый год
  $period_to_month_start = gmmktime(0, 0, 0, $month_num-1, 1, date('Y', $period['to']));
  $period = dash_season_bounds_by_date_get(1, $period_to_month_start);

  $company = dash_term_get_by_1c_id('companies', $inn);

  // собрать все записи Прихода ДС за период
  $data = [];
  if ($bdr = dash_manual_bdr_get_by_conditions(['company_id' => $company->tid, 'period' => $period])) {
    foreach ($bdr as $record) {
      if ($record->date == $month_start) {
        if (!isset($data[$record->category_id])) $data[$record->category_id] = ['name' => $record->category_name, 'plan_period' => 0, 'fact_period' => 0];
        if ($record->type == DASH_TYPE_PLAN) {
          $data[$record->category_id]['plan_period'] = $record->amount;
        }
        if ($record->type == DASH_TYPE_FACT) {
          $data[$record->category_id]['fact_period'] = $record->amount;
        }
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
    if (isset($data[$term->tid])) {
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
            ['data' => helper_number_format($item['plan_period'] ?? 0, 0, ' ', false), 'align' => 'right'],
            ['data' => helper_number_format($item['fact_period'] ?? 0, 0, ' ', false), 'align' => 'right'],
            ['data' => ($item['plan_period'] ? helper_number_format($item['fact_period'] / $item['plan_period'] * 100, 0, ' ', false) . ' %' : '-'), 'align' => 'right'],
          ],
        ];
      }
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
