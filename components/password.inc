<?php
// $Id: password.inc,v 1.21.2.9 2010/12/29 03:56:35 quicksketch Exp $

/**
 * @file
 * Webform module password component.
 */

/**
 * Implementation of _webform_defaults_component().
 */
function _webform_defaults_password() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 0,
    'extra' => array(
      'width' => '',
      'maxlength' => '',
      'field_prefix' => '',
      'field_suffix' => '',
      'disabled' => 0,
      'unique' => 0,
      'title_display' => 0,
      'description' => '',
      'attributes' => array(),
    ),
  );
}

/**
 * Implementation of _webform_theme_component().
 */
function _webform_theme_password() {
  return array(
    'webform_display_password' => array(
      'arguments' => array('element' => NULL),
    ),
  );
}

/**
 * Implementation of _webform_edit_component().
 */
function _webform_edit_password($component) {
  $form = array();
  $form['value'] = array(
    '#type' => 'password',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
    '#size' => 60,
    '#maxlength' => 127,
    '#weight' => 0,
  );
  $form['display']['width'] = array(
    '#type' => 'password',
    '#title' => t('Width'),
    '#default_value' => $component['extra']['width'],
    '#description' => t('Width of the password.') . ' ' . t('Leaving blank will use the default size.'),
    '#size' => 5,
    '#maxlength' => 10,
    '#weight' => 0,
    '#parents' => array('extra', 'width'),
  );
  $form['display']['field_prefix'] = array(
    '#type' => 'password',
    '#title' => t('Label placed to the left of the password'),
    '#default_value' => $component['extra']['field_prefix'],
    '#description' => t('Examples: $, #, -.'),
    '#size' => 20,
    '#maxlength' => 127,
    '#weight' => 1.1,
    '#parents' => array('extra', 'field_prefix'),
  );
  $form['display']['field_suffix'] = array(
    '#type' => 'password',
    '#title' => t('Label placed to the right of the password'),
    '#default_value' => $component['extra']['field_suffix'],
    '#description' => t('Examples: lb, kg, %.'),
    '#size' => 20,
    '#maxlength' => 127,
    '#weight' => 1.2,
    '#parents' => array('extra', 'field_suffix'),
  );
  $form['display']['disabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disabled'),
    '#return_value' => 1,
    '#description' => t('Make this field non-editable. Useful for setting an unchangeable default value.'),
    '#weight' => 11,
    '#default_value' => $component['extra']['disabled'],
    '#parents' => array('extra', 'disabled'),
  );
  $form['validation']['unique'] = array(
    '#type' => 'checkbox',
    '#title' => t('Unique'),
    '#return_value' => 1,
    '#description' => t('Check that all entered values for this field are unique. The same value is not allowed to be used twice.'),
    '#weight' => 1,
    '#default_value' => $component['extra']['unique'],
    '#parents' => array('extra', 'unique'),
  );
  $form['validation']['maxlength'] = array(
    '#type' => 'password',
    '#title' => t('Maxlength'),
    '#default_value' => $component['extra']['maxlength'],
    '#description' => t('Maximum length of the password value.'),
    '#size' => 5,
    '#maxlength' => 10,
    '#weight' => 2,
    '#parents' => array('extra', 'maxlength'),
  );
  return $form;
}

/**
 * Implementation of _webform_render_component().
 */
function _webform_render_password($component, $value = NULL, $filter = TRUE) {
  $element = array(
    '#type' => $component['type'],
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#default_value' => $filter ? _webform_filter_values($component['value'], NULL, NULL, NULL, FALSE) : $component['value'],
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#field_prefix' => empty($component['extra']['field_prefix']) ? NULL : ($filter ? _webform_filter_xss($component['extra']['field_prefix']) : $component['extra']['field_prefix']),
    '#field_suffix' => empty($component['extra']['field_suffix']) ? NULL : ($filter ? _webform_filter_xss($component['extra']['field_suffix']) : $component['extra']['field_suffix']),
    '#description' => $filter ? _webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
    '#attributes' => $component['extra']['attributes'],
    '#theme_wrappers' => array('webform_element_wrapper'),
    '#pre_render' => array('webform_element_title_display'),
    '#post_render' => array('webform_element_wrapper'),
    '#webform_component' => $component,
  );

  if ($component['extra']['disabled']) {
    $element['#attributes']['readonly'] = 'readonly';
  }

  // Enforce uniqueness.
  if ($component['extra']['unique']) {
    $element['#element_validate'][] = 'webform_validate_unique';
  }

  // Change the 'width' option to the correct 'size' option.
  if ($component['extra']['width'] > 0) {
    $element['#size'] = $component['extra']['width'];
  }
  if ($component['extra']['maxlength'] > 0) {
    $element['#maxlength'] = $component['extra']['maxlength'];
  }

  if (isset($value)) {
    $element['#default_value'] = $value[0];
  }

  return $element;
}

/**
 * Implementation of _webform_display_component().
 */
function _webform_display_password($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_password',
    '#theme_wrappers' => $format == 'html' ? array('webform_element', 'webform_element_wrapper') : array('webform_element_text'),
    '#post_render' => array('webform_element_wrapper'),
    '#field_prefix' => $component['extra']['field_prefix'],
    '#field_suffix' => $component['extra']['field_suffix'],
    '#format' => $format,
    '#value' => isset($value[0]) ? $value[0] : '',
    '#webform_component' => $component,
  );
}

/**
 * Format the output of data for this component.
 */
function theme_webform_display_password($element) {
  $prefix = $element['#format'] == 'html' ? filter_xss($element['#field_prefix']) : $element['#field_prefix'];
  $suffix = $element['#format'] == 'html' ? filter_xss($element['#field_suffix']) : $element['#field_suffix'];
  $value = $element['#format'] == 'html' ? check_plain($element['#value']) : $element['#value'];
  return $value !== '' ? ($prefix . $value . $suffix) : ' ';
}

/**
 * Implementation of _webform_analysis_component().
 */
function _webform_analysis_password($component, $sids = array()) {
  $placeholders = count($sids) ? array_fill(0, count($sids), "'%s'") : array();
  $sidfilter = count($sids) ? " AND sid in (" . implode(",", $placeholders) . ")" : "";
  $query = 'SELECT data ' .
    ' FROM {webform_submitted_data} ' .
    ' WHERE nid = %d ' .
    ' AND cid = %d ' . $sidfilter;
  $nonblanks = 0;
  $submissions = 0;
  $wordcount = 0;

  $result = db_query($query, array_merge(array($component['nid'], $component['cid']), $sids));
  while ($data = db_fetch_array($result)) {
    if (drupal_strlen(trim($data['data'])) > 0) {
      $nonblanks++;
      $wordcount += str_word_count(trim($data['data']));
    }
    $submissions++;
  }

  $rows[0] = array(t('Left Blank'), ($submissions - $nonblanks));
  $rows[1] = array(t('User entered value'), $nonblanks);
  $rows[2] = array(t('Average submission length in words (ex blanks)'), ($nonblanks != 0 ? number_format($wordcount/$nonblanks, 2) : '0'));
  return $rows;
}

/**
 * Implementation of _webform_table_component().
 */
function _webform_table_password($component, $value) {
  return check_plain(empty($value[0]) ? '' : $value[0]);
}

/**
 * Implementation of _webform_csv_headers_component().
 */
function _webform_csv_headers_password($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $component['name'];
  return $header;
}

/**
 * Implementation of _webform_csv_data_component().
 */
function _webform_csv_data_password($component, $export_options, $value) {
  return !isset($value[0]) ? '' : $value[0];
}
