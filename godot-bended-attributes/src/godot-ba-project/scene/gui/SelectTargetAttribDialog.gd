extends ConfirmationDialog

onready var TargetAttribList = $MainMargins/MainVbox/TargetAttributeList

func get_target_attribute():
	var selected_items = TargetAttribList.get_selected_items()
	if len(selected_items) <= 0:
		return ""
	else:
		return TargetAttribList.get_item_text(selected_items[0])
	
func _on_SelectTargetAttribDialog_about_to_show():
	TargetAttribList.clear()
	
	var target_guess = DataManager.guess_target_attribute()
	
	for a in DataManager.tree_attributes.keys():
		TargetAttribList.add_item(a)
		
	TargetAttribList.select(0)
	
	for a in DataManager.dataset_attributes.keys():
		TargetAttribList.add_item(a)
		if target_guess != null and target_guess == a:
			TargetAttribList.select(TargetAttribList.get_item_count() - 1)

	TargetAttribList.ensure_current_is_visible()
