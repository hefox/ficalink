<?php

/**
 * Settings for ficalink feature
 */
function ficalink_settings() {
  $form = array();

  $form['#ficalink_namespace'] =  variable_get('ficalink_namespace', '');
  $form['#ficalink_text'] =  variable_get('ficalink_text', 1);

  $form['ficalink_namespace'] = array(
    '#title' => t('Fieldname namespace'),
    '#default_value' => variable_get('ficalink_namespace', ''),
    '#description' => t("If you want all the ficalink fields to share a common namespace, put the namespace below"),
    '#type' => 'textfield',
    '#size' => 10,
  );
  $form['ficalink_text'] = array(
    '#title' => t('Supply text field'),
    '#default_value' => variable_get('ficalink_text', 1),
    '#description' => t("Supply a text area for users to input text of a fiction."),
    '#type' => 'checkbox',
  );

  $form = system_settings_form($form);
  
  $form['#submit'][] = 'ficalink_settings_submit';
  $form['#submit'][] = 'ficalink_clean_fields_submit';
  return $form;
}

function ficalink_calculate_field_name($one, $two, $fieldname) {
  if (empty($one)) {
    return str_replace('field_', 'field_' . $two . '_', $fieldname);
  }
  return str_replace('__', '_', str_replace($one, $two, $fieldname));
}

/*
 * Second submission function, ran after batch. 
 *
 */
function ficalink_clean_fields_submit($form, $form_state) {

  module_load_include('inc', 'content', 'includes/content.crud');
  content_clear_type_cache(TRUE);
  $old = $form['#ficalink_namespace'];
  $_fields = ficalink_content_default_fields();
  $fields = array();
  foreach($_fields as $field) {
    $old_name = ficalink_calculate_field_name($form_state['values']['ficalink_namespace'], $form['#ficalink_namespace'], $field['field_name']);
    $field_exists = content_fields($old_name, 'fic');
    if (!empty($old_name) && !empty($field_exists)) {
      content_field_instance_delete($old_name, 'fic', FALSE);
      $deleted = TRUE;
    }
  }
  if (!empty($deleted)) {
    content_clear_type_cache(TRUE);
    menu_rebuild();
  }
}

/*
 * First submission handler, adds batch to rename nodes
 */
function ficalink_settings_submit($form, $form_state) {
  // see if resave is needed
  if ($form_state['values']['ficalink_namespace'] != $form['#ficalink_namespace']) {
    // make new fields
    features_revert(array('ficalink' => array('content', 'node', 'fieldgroup')));
    $result = db_query("SELECT nid from {node} where type = 'fic'");
    while ($row = db_fetch_object($result)) {
      $nids[] = $row->nid;
    }

    if (!empty($nids)) {
      $batch = array(
        'operations' => array(
          array('_ficalink_update', array($nids, $form['#ficalink_namespace'], $form_state['values']['ficalink_namespace'])),
        ),
        'title' => t('Remapping nodes'),        'progress_message' => '',
        'error_message' => t('The update has encountered an error.'),
        'file' => drupal_get_path('module', 'ficalink') .'/ficalink.admin.inc',
      );
      batch_set($batch);
    }
  }
}

function _ficalink_update($nids, $old_namespace, $new_namespace, &$context) {
  $_fields = ficalink_content_default_fields();
  $fields = array();
  foreach($_fields as $field) {
    $fields[] = ltrim(str_replace('field_' . $new_namespace, '', $field['field_name']), '_');
  }
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($nids);
    $context['sandbox']['nids'] = $nids;
  }
  $total = $context['sandbox']['progress'] + 10;
  if ($total > $context['sandbox']['max']) {
    $total = $context['sandbox']['max'];
  }

  for (; $context['sandbox']['progress'] < $total ; $context['sandbox']['progress']++) {
    $nid = $nids[$context['sandbox']['progress']];
    $node = node_load($nid);
    if (empty($node->nid)) continue;
    foreach ($fields as $key) {
      $old_key = 'field_' . (!empty($old_namespace)? $old_namespace . '_' : '' ) . $key;
      $new_key = 'field_' . (!empty($new_namespace)? $new_namespace . '_' : '' ) . $key;
      $node->$new_key =  $node->$old_key;
    }

    node_save($node);
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  } else {
     $context['finished'] = 1;
  }
}
