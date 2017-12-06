<?php
/**
* @file
* Administration page callbacks for the annotate module.
*/
/**
* Form builder. Configure annotations.
*
* @ingroup forms
* @see system_settings_form().
*/
function annotate_admin_settings() {
$types = node_type_get_types();
foreach($types as $node_type) {
$options[$node_type->type] = $node_type->name;
}
$form['annotate_node_types'] = array(
'#type' => 'checkboxes',
'#title' => t('Users may annotate these content types'),
'#options' => $options,
'#default_value' => variable_get('annotate_node_types', array('page')),
'#description' => t('A text field will be available on these content types to make user-specific notes.'),
);
$form['annotate_deletion'] = array(
'#type' => 'radios',
'#title' => t('Annotations will be deleted'),
'#description' => t('Select a method for deleting annotations.'),
'#options' => array(
t('Never'),
t('Randomly'),
t('After 30 days')
),
'#default_value' => variable_get('annotate_deletion', 0) // Default to Never
);
$form['annotate_limit_per_node'] = array(
'#type' => 'textfield',
'#title' => t('Annotations per node'),
'#description' => t('Enter the maximum number of annotations allowed per
node (0 for no limit).'),
'#default_value' => variable_get('annotate_limit_per_node', 1),
'#size' => 3
);
$form['#submit'][] = 'annotate_admin_settings_submit';
return system_settings_form($form);
}

/**
* Implements hook_node_load()
*/
function annotate_node_load($nodes, $types) {
global $user;
// Check to see if the person viewing the node is the author. If not then
// hide the annotation.
foreach ($nodes as $node) {
if ($user->uid != $node->uid) {
unset($node->annotation);
}
}
}
/**
* Validate annotation settings submission.
*/
function annotate_admin_settings_validate($form, &$form_state) {
$limit = $form_state['values']['annotate_limit_per_node'];
if (!is_numeric($limit)) {
form_set_error('annotate_limit_per_node', t('Please enter number.'));
}
}