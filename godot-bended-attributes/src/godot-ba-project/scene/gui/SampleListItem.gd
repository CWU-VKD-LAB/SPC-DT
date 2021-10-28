extends MarginContainer

var sample_name = ""

func get_sample_name():
	return sample_name

func set_sample_name(new_name):
	$HBoxContainer/SampleLabel.text = new_name
	sample_name = new_name
	
func set_sample_color(new_color):
	$HBoxContainer/ColorPickerButton.color = new_color
	DataManager.set_sample_color(sample_name, new_color)

func get_sample_visible():
	return $HBoxContainer/VisibleCheckbox.pressed

func set_sample_visible(new_vis):
	if $HBoxContainer/VisibleCheckbox.pressed == new_vis:
		return
		
	$HBoxContainer/VisibleCheckbox.pressed = new_vis

func _on_DeleteBtn_pressed():
	DataManager.delete_sample(sample_name)
	
func _on_VisibleCheckbox_toggled(button_pressed):
	DataManager.set_sample_visibility(sample_name, button_pressed)

func _on_ColorPickerButton_color_changed(color):
	DataManager.set_sample_color(sample_name, color)

func _on_SampleLabel_gui_input(event):
	if event is InputEventMouseButton && event.button_index == BUTTON_LEFT && event.pressed == false:
		UiCanvas.show_new_sample_dialog(true, DataManager.sample_data.get(sample_name, null))
