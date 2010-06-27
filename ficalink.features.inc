<?php


/**
 * Implementation of hook_node_info().
 */
function ficalink_node_info() {
  module_load_include('inc', 'ficalink', 'ficalink.features.node');
  $args = func_get_args();
  return call_user_func_array('_ficalink_node_info', $args);
}

/**
 * Implementation of hook_taxonomy_default_vocabularies().
 */
function ficalink_taxonomy_default_vocabularies() {
  module_load_include('inc', 'ficalink', 'ficalink.features.taxonomy_vocabulary');
  $args = func_get_args();
  return call_user_func_array('_ficalink_taxonomy_default_vocabularies', $args);
}
