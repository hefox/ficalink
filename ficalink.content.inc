<?php

/**
 * Helper to implementation of hook_content_default_fields().
 */
function ficalink_content_default_fields() {
  $fields = array();
  
  // get some useful default display fields
  $tag_display_fields = ficalink_sane_display_fields(array('format' => 'link', 'exclude' => 0));
  $default_display_fields = ficalink_sane_display_fields();
  $no_label = ficalink_sane_display_fields(NULL, array('label' => 'hidden'));
  
  // Want to be able to name space all fields module supplies
  $namespace =  variable_get('ficalink_namespace', '');
  $namespace = 'field_' . (!empty($namespace)?$namespace . '_':'') ;
  $tags = array(
    'authors' => array(
      'widget' => array(
        'description' => 'Careful of two authors with the same name; try to distinguish them if ya can.',
      ),
    ),
    'characters' => array(),
    'genres' => array(),
    'series' => array(),
    'tags' => array(),
    'warnings' => array(),
  );

  $default = array(
    'type_name' => 'fic',
    'display_settings' => $tag_display_fields,
    'widget_active' => '1',
    'type' => 'content_taxonomy',
    'required' => '0',
    'multiple' => '1',
    'module' => 'content_taxonomy',
    'active' => '1',
    'save_term_node' => 1,
    'parent' => '0',
    'parent_php_code' => '',
    'depth' => '',
    'widget' => array(
      'new_terms' => 'insert',
      'extra_parent' => '0',
      'maxlength' => '255',
      'active_tags' => NULL,
      'default_value' => array(array('tid'=>NULL)),
      'default_value_php' => NULL,
      'description' => '',
      'type' => 'content_taxonomy_autocomplete',
      'module' => 'content_taxonomy_autocomplete',
    ),
  );

  foreach($tags as $key => $base) {
    $fields['fic-' . $namespace . $key] = $base + $default;
    $fields['fic-' . $namespace . $key]['field_name'] = $namespace . $key;
    $fields['fic-' . $namespace . $key]['widget'] += $default['widget'];
    $machine = ($base['machine_name']?$base['machine_name']:$key);
    if (empty($base['widget']['label'])) {
      $fields['fic-' . $namespace . $key]['widget']['label'] = ucfirst($key);
    }
    $fields['fic-' . $namespace . $key]['vid'] = exportables_machine_load_id('taxonomy_vocabulary', $machine); 
  }

  // TODO rename body field to description?
  $fields[] = array(
    'field_name' => $namespace . 'body',
    'type_name' => 'fic',
    'display_settings' => $default_display_fields,
    'widget_active' => '1',
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '1',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
          'format' => '1',
          '_error_element' => 'default_value_widget][field_body][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'rows' => '5',
      'size' => 60,
      'label' => 'Description',
      'weight' => '2',
      'description' => '',
      'type' => 'text_textarea',
      'module' => 'text',
    ),
  );

  // Exported field: field_links
  $fields[] = array(
    'field_name' => $namespace . 'links',
    'type_name' => 'fic',
    'display_settings' => $no_label,
    'widget_active' => '1',
    'type' => 'link',
    'required' => '0',
    'multiple' => '1',
    'module' => 'link',
    'active' => '1',
    'attributes' => array(
      'target' => 'default',
      'rel' => '',
      'class' => '',
    ),
    'display' => array(
      'url_cutoff' => '80',
    ),
    'url' => 0,
    'title' => 'optional',
    'title_value' => '',
    'enable_tokens' => 0,
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'title' => '',
          'url' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Links',
      'weight' => '3',
      'description' => '',
      'type' => 'link',
      'module' => 'link',
    ),
  );

  if (variable_get('ficalink_text', 1)) {

    // Exported field: field_text
    $fields[] = array(
      'field_name' => $namespace . 'text',
      'type_name' => 'fic',
      'display_settings' => $default_display_fields,
      'widget_active' => '1',
      'type' => 'text',
      'required' => '0',
      'multiple' => '0',
      'module' => 'text',
      'active' => '1',
      'text_processing' => '1',
      'max_length' => '',
      'allowed_values' => '',
      'allowed_values_php' => '',
      'widget' => array(
        'rows' => '30',
        'size' => 60,
        'default_value' => array(
          '0' => array(
            'value' => '',
            '_error_element' => 'default_value_widget][field_text][0][value',
          ),
        ),
        'default_value_php' => NULL,
        'label' => 'Text',
        'weight' => '-1',
        'description' => '<b>You may add the text of the fiction. Bare in mind that if this is not legal work, you should have permission to post it. </b>',
        'type' => 'text_textarea',
        'module' => 'text',
      ),
    );
  }
  // Translatables
  array(
    t('Authors'),
    t('Characters'),
    t('Description'),
    t('Genres'),
    t('Links'),
    t('Series'),
    t('Tags'),
    t('Text'),
    t('Warnings'),
  );

  return $fields;
}


/**
 * Helper to implementation of hook_fieldgroup_default_groups().
 */
function ficalink_fieldgroup_default_groups() {
  $groups = array();
  $_namespace = variable_get('ficalink_namespace', '');
  if (!empty($_namespace)) {
    $_namespace .= '_' ;
  }
  $namespace = 'field_' . $_namespace;
  $group_namespace = 'group_' . $_namespace ;
  // Exported group: group_extra
  $groups[] = array(
    'group_type' => 'standard',
    'type_name' => 'fic',
    'group_name' => $group_namespace . 'extra',
    'label' => 'Extra',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'weight' => '-1',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'description' => '',
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-1',
    'fields' => array(),
  );

  // Exported group: group_main
  $groups[] = array(
    'group_type' => 'standard',
    'type_name' => 'fic',
    'group_name' => $group_namespace . 'main',
    'label' => 'Main',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'weight' => '-3',
        'label' => 'hidden',
        'teaser' => array(
          'format' => 'simple',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'simple',
          'exclude' => 0,
        ),
        'description' => '',
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-4',
    'fields' => array(
      '0' => $namespace . 'body',
      '1' => $namespace . 'links',
    ),
  );

  // Exported group: group_tags
  $groups[] = array(
    'group_type' => 'standard',
    'type_name' => 'fic',
    'group_name' => $group_namespace . 'tags',
    'label' => 'Categorization',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'weight' => '-4',
        'label' => 'above',
        'teaser' => array(
          'format' => 'simple',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'simple',
          'exclude' => 0,
        ),
        'description' => '',
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-3',
    'fields' => array(
      '0' => $namespace . 'authors',
      '1' => $namespace . 'warnings',
      '2' => $namespace . 'genres',
      '3' => $namespace . 'characters',
      '4' => $namespace . 'series',
      '5' => $namespace . 'tags',
    ),
  );

  // Exported group: group_text
  $groups[] = array(
    'group_type' => 'standard',
    'type_name' => 'fic',
    'group_name' => $group_namespace . 'text',
    'label' => 'Text',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'weight' => '-2',
        'label' => 'above',
        'teaser' => array(
          'format' => 'simple',
          'exclude' => 1,
        ),
        'full' => array(
          'format' => 'simple',
          'exclude' => 0,
        ),
        'description' => '',
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-2',
    'fields' => array(
      '0' => $namespace . 'text',
    ),
  );

  // Translatables
  array(
    t('Categorization'),
    t('Extra'),
    t('Main'),
    t('Text'),
  );

  return $groups;
}
/**
 * Implementation of hook_menu().
 */
function ficalink_menu() {
  $items = array();
  $items['admin/content/node-type/fic/ficalink-settings'] = array(
    'title' => 'Ficalink settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('ficalink_settings'),
    'access arguments' => array('administer content types'),
    'file' => 'ficalink.admin.inc',
    'type' => MENU_LOCAL_TASK,
  );

  return $items;
}
