extends HBoxContainer

var _attribute = ""

func set_data(attrib):
	_attribute = attrib
	
	var attrib_info = null
	if attrib in DataManager.tree_attributes:
		attrib_info = DataManager.tree_attributes[attrib]
	else:
		attrib_info = DataManager.dataset_attributes.get(attrib, null)
	
	var attrib_type = DataManager.AttributeType.Text
	if attrib_info != null:
		attrib_type = attrib_info.attrib_type
		
	if attrib_type == DataManager.AttributeType.Text:
		$Label.text = attrib + " (Text):"
	else:
		$Label.text = attrib + " (Numerical):"

func set_attrib_value(val):
	$ValueLineEdit.text = str(val)

func get_attribute():
	return _attribute

func get_attribute_value():
	var value = $ValueLineEdit.text.strip_edges()
	if value == "":
		value = "0"
	return value
