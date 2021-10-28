extends WindowDialog

onready var FilterNameEdit = $MainMargins/MainVbox/FilterNameEdit
onready var ExprEdit = $MainMargins/MainVbox/ExpressionEdit
onready var AttribItemList = $MainMargins/MainVbox/AttributeItemList
onready var ClassItemList = $MainMargins/MainVbox/ClassItemList

var edit_mode setget set_edit_mode, get_edit_mode
var _edit_filter_data = null

var original_expr = ""
var parsed_expr = ""

signal confirmed(filter_name, original_expr, parsed_expr, expr_instance)

func _ready():
	pass

func set_edit_mode(mode):
	edit_mode = mode
	if edit_mode:
		FilterNameEdit.editable = false
	else:
		FilterNameEdit.editable = true
	
func get_edit_mode():
	return edit_mode
	
func set_filter_data(filter_data):
	_edit_filter_data = filter_data
	
	
func _on_NewFilterDialog_about_to_show():
	if edit_mode and _edit_filter_data != null:
		FilterNameEdit.text = _edit_filter_data.name
		ExprEdit.text = _edit_filter_data.original_expr
	else:
		FilterNameEdit.text = "New Filter"
		ExprEdit.text = "$ex number attrib$ < 25 and $ex text attrib$ == \"class_0\""
	
	AttribItemList.clear()
	
	if DataManager.tree_attributes == null:
		return
		
	for a in DataManager.tree_attributes.keys():
		AttribItemList.add_item(a)
		
	for a in DataManager.dataset_attributes.keys():
		AttribItemList.add_item(a)

	AttribItemList.select(0)
	
	# ================================
	
	ClassItemList.clear()
	
	if DataManager.tree_classes == null:
		return
		
	for c in DataManager.tree_classes:
		ClassItemList.add_item(c)
		
	ClassItemList.select(0)

func _on_InsertAttributeBtn_pressed():
	var selection = AttribItemList.get_selected_items()
	if len(selection) > 0:
		var attrib = AttribItemList.get_item_text(selection[0])
		ExprEdit.append_at_cursor(" $" + attrib + "$ ")


func _on_InsertClassBtn_pressed():
	var selection = ClassItemList.get_selected_items()
	if len(selection) > 0:
		var sel_class = ClassItemList.get_item_text(selection[0])
		ExprEdit.append_at_cursor(" %prediction% == \"" + sel_class + "\" ")


func _on_CancelBtn_pressed():
	hide()


func _on_OkayBtn_pressed():
	original_expr = ExprEdit.text
	parsed_expr = DataManager.parse_filter_expression(original_expr)
	var expression_instance = Expression.new()
	var error = expression_instance.parse(parsed_expr, ["sample"])

	if error != OK:
		UiCanvas.show_message_box("Error parsing expression", expression_instance.get_error_text())
	else:
		hide()
		emit_signal("confirmed", FilterNameEdit.text, original_expr, parsed_expr, expression_instance)
