extends HBoxContainer

var filter_name = ""
var filter_instance = null

func set_filter_instance(filter):
	filter_instance = filter
	filter["list_instance"] = self
	set_filter_name(filter.name)

func set_filter_name(name):
	$FilterNameLabel.text = name
	filter_name = name


func _on_DeleteBtn_pressed():
	DataManager.delete_sample_filter(filter_name)


func _on_ShowOnlyBtn_pressed():
	DataManager.apply_filter(filter_name, DataManager.FilterMode.ShowOnly)

func _on_HideOnlyBtn_pressed():
	DataManager.apply_filter(filter_name, DataManager.FilterMode.HideOnly)

func _on_ShowBtn_pressed():
	DataManager.apply_filter(filter_name, DataManager.FilterMode.Show)

func _on_HideBtn_pressed():
	DataManager.apply_filter(filter_name, DataManager.FilterMode.Hide)


func _on_FilterNameLabel_gui_input(event):
	if event is InputEventMouseButton && event.button_index == BUTTON_LEFT && event.pressed == false:
		UiCanvas.show_new_filter_dialog(true, filter_instance)
		#UiCanvas.show_new_sample_dialog(true, DataManager.sample_data.get(sample_name, null))


