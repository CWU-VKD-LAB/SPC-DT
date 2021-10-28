extends ConfirmationDialog

onready var AttribItemList = $MarginContainer/VBoxContainer/AttributeItemList

func get_target_attribute():
	return AttribItemList.get_item_text(AttribItemList.get_selected_items()[0])

func get_start_hue():
	return $MarginContainer/VBoxContainer/StartHueHbox/StartHueSpinbox.value
	
func get_end_hue():
	return $MarginContainer/VBoxContainer/EndHueHbox/EndHueSpinbox.value
	
func get_min_saturation():
	return $MarginContainer/VBoxContainer/MinSatHbox/MinSatSpinbox.value
	
func get_max_saturation():
	return $MarginContainer/VBoxContainer/MaxSatHbox/MaxSatSpinbox.value

func get_opacity():
	return $MarginContainer/VBoxContainer/OpacityCompHbox/OpacitySpinbox.value

func get_reverse_color_order():
	return $MarginContainer/VBoxContainer/ReverseColorsHbox/ReverseColorsCheckbox.pressed

func _on_AutoColorSampleDialog_about_to_show():
	AttribItemList.clear()
	
	var target_guess = DataManager.guess_target_attribute()
	
	for a in DataManager.tree_attributes.keys():
		AttribItemList.add_item(a)
		
	AttribItemList.select(0)
		
	for a in DataManager.dataset_attributes.keys():
		AttribItemList.add_item(a)
		if target_guess != null and a == target_guess:
			AttribItemList.select(AttribItemList.get_item_count() - 1)
	
	AttribItemList.ensure_current_is_visible()
