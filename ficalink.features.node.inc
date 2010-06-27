<?php

/**
 * Helper to implementation of hook_node_info().
 */
function _ficalink_node_info() {
  $items = array(
    'fic' => array(
      'name' => t('Fiction'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => t('A chapter/one shot/etc.'),
    ),
  );
  return $items;
}
