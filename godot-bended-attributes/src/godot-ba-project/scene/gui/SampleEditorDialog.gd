extends WindowDialog

const AUTO_HUE_SCALE_FEW_CAT = 0.654
const AUTO_HUE_SCALE_MANY_CAT = 0.835

onready var SampleListItem = preload("res://scene/gui/SampleListItem.tscn")
onready var SampleVBox = $MainMargins/MainVBox/SamplesPanel/ScrollContainer/SamplesVBox

onready var rng = RandomNumberGenerator.new()

# Called when the node enters the scene tree for the first time.
func _ready():
	rng.randomize()
# warning-ignore:return_value_discarded
	DataManager.connect("tree_data_changed", self, "_on_tree_data_changed")
# warning-ignore:return_value_discarded
	DataManager.connect("sample_data_added", self, "_on_sample_data_added")
# warning-ignore:return_value_discarded
	DataManager.connect("sample_data_deleted", self, "_on_sample_data_deleted")
# warning-ignore:return_value_discarded
	DataManager.connect("sample_visibility_changed", self, "_on_sample_visibility_changed")
# warning-ignore:return_value_discarded
	UiCanvas.get_node("RandomSampleColorDialog").connect("confirmed", self, "_on_random_sample_color_dialog_confirmed")
# warning-ignore:return_value_discarded
	UiCanvas.get_node("SetColorDialog").connect("confirmed", self, "_on_SetColorDialog_confirmed")
# warning-ignore:return_value_discarded
	UiCanvas.get_node("AutoColorSampleDialog").connect("confirmed", self, "_on_AutoColorSampleDialog_confirmed")

func _on_NewSampleBtn_pressed():
	UiCanvas.show_new_sample_dialog()

func _on_tree_data_changed():
	hide()
	for node in SampleVBox.get_children():
		SampleVBox.remove_child(node)
		node.queue_free()

func _on_sample_data_added(new_sample):
	var new_instance = SampleListItem.instance()
	new_instance.set_sample_name(new_sample.name)
	new_instance.set_sample_color(Color.gray)
	SampleVBox.add_child(new_instance)
	new_sample["list_instance"] = new_instance

func _on_sample_data_deleted(sample):
	var list_instance = sample.list_instance
	SampleVBox.remove_child(list_instance)
	list_instance.queue_free()
	

func _on_sample_visibility_changed(sample):
	sample.list_instance.set_sample_visible(sample.visible)

func _on_AutoColorSampleDialog_confirmed():
	var target_attribute = UiCanvas.AutoColorDialog.get_target_attribute()
	var start_hue = UiCanvas.AutoColorDialog.get_start_hue()
	var end_hue = UiCanvas.AutoColorDialog.get_end_hue()
	var min_sat = UiCanvas.AutoColorDialog.get_min_saturation()
	var max_sat = UiCanvas.AutoColorDialog.get_max_saturation()
	var opacity = UiCanvas.AutoColorDialog.get_opacity()
	var reverse_color_order = UiCanvas.AutoColorDialog.get_reverse_color_order()
	
	if start_hue > end_hue:
		UiCanvas.show_message_box("Error", "End hue must be greater than starting hue.")
		return
		
	if min_sat > max_sat:
		UiCanvas.show_message_box("Error", "Max saturation must be greater than min saturation.")
		return
		
	if DataManager.sample_data == null:
		return
		
	var attrib_categories = {}
	var sample_count = 0
	
	for s in DataManager.sample_data.keys():
		var sample = DataManager.sample_data[s]
		if sample.attributes.has(target_attribute):
			var attrib_value = str(sample.attributes[target_attribute])
			sample_count += 1
			
			if attrib_categories.has(attrib_value):
				attrib_categories[attrib_value].push_back(sample)
			else:
				attrib_categories[attrib_value] = [sample]	
			
	var cat_list = attrib_categories.keys()
	var num_cat = len(cat_list)
	cat_list.sort()
	if reverse_color_order:
		cat_list.invert()
	
	if num_cat <= 3:
		start_hue *= AUTO_HUE_SCALE_FEW_CAT
		end_hue *= AUTO_HUE_SCALE_FEW_CAT
	else:
		start_hue *= AUTO_HUE_SCALE_MANY_CAT
		end_hue *= AUTO_HUE_SCALE_MANY_CAT
	
	for i in range(num_cat):
		var hue_factor = 0
		if num_cat > 1:
			hue_factor = i / float(num_cat - 1)
			
		var hue = (hue_factor * (end_hue - start_hue)) + start_hue
		var sample_group = attrib_categories[cat_list[i]]
		var group_size = len(sample_group)
		for j in range(group_size):
			var sample = sample_group[j]
			var saturation_factor = 0
			if group_size > 1:
				saturation_factor = j / float(group_size - 1)
			
			var saturation = (saturation_factor * (max_sat - min_sat)) + min_sat
			var final_color = Color.white
			final_color = final_color.from_hsv(hue, saturation, saturation, opacity)
			if sample.has("list_instance"):
				sample.list_instance.set_sample_color(final_color)
		
	UiCanvas.show_message_box("Auto Color Completed.", "The colors for " + str(sample_count) + " samples were changed to one of " + str(num_cat) + " base colors.")
		
func _on_random_sample_color_dialog_confirmed():
	var min_color = UiCanvas.RandomSampleColorDialog.get_darkest_color_value()
	var max_color = UiCanvas.RandomSampleColorDialog.get_brightest_color_value()
	var opacity = UiCanvas.RandomSampleColorDialog.get_opacity_value()
	var visible_only = UiCanvas.RandomSampleColorDialog.get_visible_only()
	
	if min_color > max_color:
		UiCanvas.show_message_box("Error", "Darkest Color cannont be greater than the Brightest Color.")
		return
		
	if DataManager.sample_data == null:
		return
		
	var samples_changed = 0
		
	for n in SampleVBox.get_children():
		if visible_only and not n.get_sample_visible():
			continue
		
		var random_color = Color.white
		random_color.a = opacity
		random_color.r = rng.randf_range(min_color, max_color)
		random_color.g = rng.randf_range(min_color, max_color)
		random_color.b = rng.randf_range(min_color, max_color)
		n.set_sample_color(random_color)
		samples_changed += 1

	UiCanvas.show_message_box("Color Randomization Completed.", "The colors for " + str(samples_changed) + " samples were changed.")

func _on_SetColorDialog_confirmed():
	var gradient = UiCanvas.SetColorDialog.get_color_gradient()
	var visible_only = UiCanvas.SetColorDialog.get_visible_only()
	
	if DataManager.sample_data == null:
		return
		
	var samples_to_change = []
		
	for n in SampleVBox.get_children():
		if visible_only and not n.get_sample_visible():
			continue
		samples_to_change.append(n)
		
	var num_samples = len(samples_to_change)
	for i in range(num_samples):
		var offset = float(i) / float(num_samples)
		samples_to_change[i].set_sample_color(gradient.interpolate(offset))

	UiCanvas.show_message_box("Set Colors Completed.", "The colors for " + str(num_samples) + " samples were changed.")

func _on_CloseBtn_pressed():
	hide()

func _on_LoadCsvBtn_pressed():
	UiCanvas.show_load_csv_dialog()


func _on_ShowAllBtn_pressed():
	for n in SampleVBox.get_children():
		#n.set_sample_visible(true)
		DataManager.set_sample_visibility(n.get_sample_name(), true)


func _on_HideAllBtn_pressed():
	for n in SampleVBox.get_children():
		#n.set_sample_visible(false)
		DataManager.set_sample_visibility(n.get_sample_name(), false)


func _on_RandomizeColorsBtn_pressed():
	UiCanvas.show_random_sample_color_dialog()

func _on_SetColorsBtn_pressed():
	UiCanvas.show_set_color_dialog()

func _on_AutoColorsBtn_pressed():
	UiCanvas.show_auto_color_sample_dialog()
