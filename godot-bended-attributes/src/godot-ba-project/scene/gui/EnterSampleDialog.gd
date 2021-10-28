extends ConfirmationDialog

onready var AttribContainer = preload("res://scene/gui/AttribContainer.tscn")
onready var ui_container = $MarginContainer/VBoxContainer/ScrollContainer/AttribContainer
onready var name_edit = $MarginContainer/VBoxContainer/NameContainer/NameEdit

var edit_mode setget set_edit_mode, get_edit_mode
var _edit_sample_data = {}

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

func get_is_visible():
	return $MarginContainer/VBoxContainer/VisibleCheckbox.pressed

func set_edit_mode(new_mode):
	edit_mode = new_mode
	if edit_mode:
		name_edit.editable = false
	else:
		name_edit.editable = true
		
func get_edit_mode():
	return edit_mode
		
func get_sample_data():
	var sample_data = {}
	var attrib_data = {}
	
	for c in ui_container.get_children():
		var attrib = c.get_attribute()
		var attrib_info = null
		
		if attrib in DataManager.tree_attributes:
			attrib_info = DataManager.tree_attributes[attrib]
		else:
			attrib_info = DataManager.dataset_attributes.get(attrib, null)
			
		var value_str = c.get_attribute_value()
		var is_numerical = DataManager.is_numerical_string(value_str)
		var value_final = 0
		
		if attrib_info.attrib_type == DataManager.AttributeType.Text:
			value_final = value_str
		else:
			if not is_numerical:
				UiCanvas.show_message_box("Error", "Value for attribute " + attrib + " must be of a numeric type.")
				return null
			else:
				value_final = float(value_str)
				
		attrib_data[attrib] = value_final
		
	sample_data["name"] = name_edit.text
	sample_data["spline_color"] = Color.gray
	sample_data["attributes"] = attrib_data
	return sample_data
	
func set_sample_data(sample_data):
	if sample_data != null:
		_edit_sample_data = sample_data
	else:
		_edit_sample_data = {}
	
func _on_EnterSampleDialog_about_to_show():
	if edit_mode:
		name_edit.text = _edit_sample_data.get("name", "Error Name Missing")
		$MarginContainer/VBoxContainer/VisibleCheckbox.pressed = _edit_sample_data.get("visible", false)
	else:
		name_edit.text = "New Sample"
		
	for c in ui_container.get_children():
		ui_container.remove_child(c)
		c.queue_free()
		
	for attrib in DataManager.tree_attributes.keys():
		var instance = AttribContainer.instance()
			
		instance.set_data(attrib)
		ui_container.add_child(instance)
		
		if edit_mode:
			instance.set_attrib_value(_edit_sample_data.attributes.get(attrib, 0))
		else:
			instance.set_attrib_value(0)
			
	for attrib in DataManager.dataset_attributes.keys():
		var instance = AttribContainer.instance()
		var attrib_info = DataManager.dataset_attributes[attrib]
		var attrib_type = DataManager.AttributeType.Text
		
		if attrib_info != null:
			attrib_type = attrib_info.attrib_type
			
		instance.set_data(attrib)
		ui_container.add_child(instance)
		
		if edit_mode:
			if attrib_type == DataManager.AttributeType.Text:
				instance.set_attrib_value(_edit_sample_data.attributes.get(attrib, "text_attrib"))
			else:
				instance.set_attrib_value(_edit_sample_data.attributes.get(attrib, 0))
		else:
			if attrib_type == DataManager.AttributeType.Text:
				instance.set_attrib_value("text_attrib")
			else:
				instance.set_attrib_value(0)

func _on_VisibleCheckbox_toggled(button_pressed):
	if edit_mode and _edit_sample_data != null:
		DataManager.set_sample_visibility(_edit_sample_data.name, button_pressed)
