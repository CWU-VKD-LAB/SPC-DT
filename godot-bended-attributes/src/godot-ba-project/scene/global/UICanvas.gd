extends CanvasLayer

onready var CSVFileDialog = $CSVFileDialog
onready var RandomSampleColorDialog = $RandomSampleColorDialog
onready var SetColorDialog = $SetColorDialog
onready var AutoColorDialog = $AutoColorSampleDialog
onready var TreeScalingDialog = $TreeScalingDialog
onready var NodeColorPicker = $HUDMargin/VBoxContainer/BottomBtnBar/NodeColorPicker
onready var BranchColorPicker = $HUDMargin/VBoxContainer/BottomBtnBar/BranchColorPicker

var temp_csv_path = ""
var temp_csv_delim = ","

func _enter_tree():
# warning-ignore:return_value_discarded
	DataManager.connect("tree_data_changed", self, "_on_tree_data_changed")
# warning-ignore:return_value_discarded
	DataManager.connect("spline_mode_changed", self, "_on_spline_mode_changed")
# warning-ignore:return_value_discarded
	DataManager.connect("adv_leaf_color_mode_changed", self, "_on_adv_leaf_color_mode_changed")

func _on_LoadTreeFileBtn_pressed():
	$TreeFileDialog.popup_centered()


func _on_TreeFileDialog_file_selected(path):
	DataManager.load_tree_file(path)

func show_node_popup(node_dict):
	$NodePopup.set_node_data(node_dict)
	$NodePopup.popup_centered()

func _on_adv_leaf_color_mode_changed(is_enabled):
	$HUDMargin/VBoxContainer/TopBtnBar/AdvLeafColorsCheckbox.pressed = is_enabled

func _on_spline_mode_changed(new_mode):
	if new_mode == DataManager.SplineMode.Simple:
		$HUDMargin/VBoxContainer/TopBtnBar/AdvLeafColorsCheckbox.visible = false
		$HUDMargin/VBoxContainer/TopBtnBar/SplineModeBtn.text = "Switch to Advanced View"
	else:
		$HUDMargin/VBoxContainer/TopBtnBar/AdvLeafColorsCheckbox.visible = true
		$HUDMargin/VBoxContainer/TopBtnBar/SplineModeBtn.text = "Switch to Simple View"
	
func _on_tree_data_changed():
	$HUDMargin/VBoxContainer/TopBtnBar/AdvLeafColorsCheckbox.visible = false
	$HUDMargin/VBoxContainer/TopBtnBar/SplineModeBtn.text = "Switch to Advanced View"
	
	if DataManager.tree_data != null:
		$HUDMargin/VBoxContainer/TopBtnBar/ShowSampleBtn.disabled = false
		$HUDMargin/VBoxContainer/TopBtnBar/ShowFiltersBtn.disabled = false
		$HUDMargin/VBoxContainer/TopBtnBar/ShowConfusionMatrixBtn.disabled = false
		$HUDMargin/VBoxContainer/TopBtnBar/SplineModeBtn.disabled = false
	else:
		$HUDMargin/VBoxContainer/TopBtnBar/ShowSampleBtn.disabled = true
		$HUDMargin/VBoxContainer/TopBtnBar/ShowFiltersBtn.disabled = true
		$HUDMargin/VBoxContainer/TopBtnBar/ShowConfusionMatrixBtn.disabled = true
		$HUDMargin/VBoxContainer/TopBtnBar/SplineModeBtn.disabled = true
		
# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass

func _on_ShowSampleBtn_pressed():
	var cur_res = DataManager.cur_resolution
	var res_w = cur_res.x
	var res_h = cur_res.y
	var win_rect = $SampleEditorDialog.get_rect()
	
	var new_pos = Vector2((res_w / 2) - (win_rect.size.x / 2), (res_h / 2) - (win_rect.size.y / 2))
	if new_pos.x < 50:
		new_pos.x = 50
	if new_pos.y < 50:
		new_pos.y = 50
		
	$SampleEditorDialog.set_position(new_pos)
	$SampleEditorDialog.show()
	
	
func show_new_sample_dialog(edit_mode = false, sample_data = null):
	$EnterSampleDialog.edit_mode = edit_mode
	$EnterSampleDialog.set_sample_data(sample_data)
	$EnterSampleDialog.popup_centered()

func show_load_csv_dialog():
	$CSVFileDialog.popup_centered()

func show_random_sample_color_dialog():
	$RandomSampleColorDialog.popup_centered()

func show_auto_color_sample_dialog():
	$AutoColorSampleDialog.popup_centered()

func show_new_filter_dialog(edit_mode = false, filter_data = null):
	$NewFilterDialog.edit_mode = edit_mode
	$NewFilterDialog.set_filter_data(filter_data)
	$NewFilterDialog.popup_centered()

func show_set_color_dialog():
	$SetColorDialog.popup_centered()

func show_select_target_attrib_dialog():
	$SelectTargetAttribDialog.popup_centered()

func _on_EnterSampleDialog_confirmed():
	var sample_data = $EnterSampleDialog.get_sample_data()
	if sample_data == null:
		return
		
	var is_edit_mode = $EnterSampleDialog.edit_mode
	if is_edit_mode:
		DataManager.edit_sample_attributes(sample_data.name, sample_data.attributes)
	else:
		DataManager.add_new_sample(sample_data)
		if $EnterSampleDialog.get_is_visible():
			DataManager.set_sample_visibility(sample_data.name, true)
		
	DataManager.check_spline_update_needed()
	
func show_message_box(title, message):
	$MsgBoxDialog.show_msg_box(title, message)


func _on_720pBtn_pressed():
	get_tree().set_screen_stretch(SceneTree.STRETCH_MODE_2D, SceneTree.STRETCH_ASPECT_EXPAND, Vector2(1280,720))
	DataManager.cur_resolution = Vector2(1280, 720)

func _on_1080pBtn_pressed():
	get_tree().set_screen_stretch(SceneTree.STRETCH_MODE_2D, SceneTree.STRETCH_ASPECT_EXPAND, Vector2(1920,1080))
	DataManager.cur_resolution = Vector2(1920, 1080)
	
func _on_1440pBtn_pressed():
	get_tree().set_screen_stretch(SceneTree.STRETCH_MODE_2D, SceneTree.STRETCH_ASPECT_EXPAND, Vector2(2560,1440))
	DataManager.cur_resolution = Vector2(2560, 1440)
	
func _on_ToggleFullscreenBtn_pressed():
	OS.window_fullscreen = !OS.window_fullscreen

func _on_CSVFileDialog_file_selected(path):
	temp_csv_path = path
	$CsvDelimDialog.popup_centered()

func _on_CsvDelimDialog_confirmed():
	temp_csv_delim = $CsvDelimDialog.get_delimeter()
	var csv_file_info = DataManager.open_samples_csv(temp_csv_path, temp_csv_delim)
	if csv_file_info == null:
		return
		
	$LoadCSVDialog.set_csv_file_path(temp_csv_path)
	$LoadCSVDialog.set_csv_file_info(csv_file_info)
	$LoadCSVDialog.set_csv_file_delim(temp_csv_delim)
	$LoadCSVDialog.popup_centered()


func _on_BackgroundColorPicker_color_changed(color):
	$BackgroundLayer/BackgroundColor.color = color


func _on_ShowFiltersBtn_pressed():
	var cur_res = DataManager.cur_resolution
	var res_w = cur_res.x
	var res_h = cur_res.y
	var win_rect = $FilterSamplesDialog.get_rect()
	
	var new_pos = Vector2((res_w / 2) - (win_rect.size.x / 2), (res_h / 2) - (win_rect.size.y / 2))
	if new_pos.x < 50:
		new_pos.x = 50
	if new_pos.y < 50:
		new_pos.y = 50
	
	$FilterSamplesDialog.set_position(new_pos)
	$FilterSamplesDialog.show()

func _on_NewFilterDialog_confirmed(filter_name, original_expr, _parsed_expr, expr_instance):
	if $NewFilterDialog.edit_mode:
		DataManager.change_sample_filter(filter_name, original_expr, expr_instance)
	else:
		DataManager.create_sample_filter(filter_name, original_expr, expr_instance)

func _on_ShowConfusionMatrixBtn_pressed():
	var cur_res = DataManager.cur_resolution
	var res_w = cur_res.x
	var res_h = cur_res.y
	var win_rect = $ConfusionMatrixDialog.get_rect()
	
	var new_pos = Vector2((res_w / 2) - (win_rect.size.x / 2), (res_h / 2) - (win_rect.size.y / 2))
	if new_pos.x < 50:
		new_pos.x = 50
	if new_pos.y < 50:
		new_pos.y = 50
	
	$ConfusionMatrixDialog.set_position(new_pos)
	$ConfusionMatrixDialog.show()

func _on_SelectTargetAttribDialog_confirmed():
	$ConfusionMatrixDialog.target_attrib = $SelectTargetAttribDialog.get_target_attribute()

func _on_SplineModeBtn_pressed():
	if DataManager.spline_mode == DataManager.SplineMode.Simple:
		DataManager.spline_mode = DataManager.SplineMode.Advanced
	else:
		DataManager.spline_mode = DataManager.SplineMode.Simple

func _on_TreeScalingBtn_pressed():
	$TreeScalingDialog.popup_centered()

func _on_AdvLeafColorsCheckbox_toggled(button_pressed):
	DataManager.adv_leaf_color_mode = button_pressed
