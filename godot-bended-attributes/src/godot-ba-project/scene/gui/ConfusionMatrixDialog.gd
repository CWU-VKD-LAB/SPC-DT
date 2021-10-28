extends WindowDialog

onready var MatrixContainer = $MainMargins/MainVbox/MatrixContainer
onready var MetricsTextEdit = $MainMargins/MainVbox/MetricsTextEdit

var target_attrib setget _set_target_attribute, _get_target_attribute

func _ready():
	target_attrib = ""
	# warning-ignore:return_value_discarded
	DataManager.connect("tree_data_changed", self, "_on_tree_data_changed")

func _on_tree_data_changed():
	target_attrib = ""
	hide()
	clear_all()

func _set_target_attribute(attrib):
	target_attrib = attrib
	refresh()
	
func _get_target_attribute():
	return target_attrib

func _create_label_control(text):
	var new_label = Label.new()
	new_label.text = text
	new_label.align = Label.ALIGN_CENTER
	new_label.valign = Label.VALIGN_CENTER
	new_label.size_flags_horizontal = Label.SIZE_EXPAND_FILL
	new_label.size_flags_vertical = Label.SIZE_EXPAND_FILL
	new_label.clip_text = true
	
	return new_label
	
func _create_vsep():
	var new_sep = VSeparator.new()
	new_sep.size_flags_horizontal = VSeparator.SIZE_EXPAND_FILL
	new_sep.size_flags_vertical = VSeparator.SIZE_EXPAND_FILL
	
	return new_sep
	
func _create_hsep():
	var new_sep = HSeparator.new()
	new_sep.size_flags_horizontal = HSeparator.SIZE_EXPAND_FILL
	new_sep.size_flags_vertical = HSeparator.SIZE_EXPAND_FILL
	
	return new_sep
	
func clear_all():
	for n in MatrixContainer.get_children():
		MatrixContainer.remove_child(n)
		n.queue_free()
	
	MetricsTextEdit.text = ""
	
func refresh():
	clear_all()
	
	if DataManager.sample_data == null:
		UiCanvas.show_message_box("Error", "You must load some samples first.")
		return
	
	if target_attrib == "":
		UiCanvas.show_message_box("Error", "You must select an attribute for the target.")
		return
	
	if not target_attrib in DataManager.tree_attributes and not target_attrib in DataManager.dataset_attributes:
		UiCanvas.show_message_box("Error", "Uknown target attribute: " + target_attrib)
		return

	var class_labels = DataManager.tree_classes.duplicate()
	var num_classes = len(class_labels)
	
	if $MainMargins/MainVbox/ReverseClassCheckbox.pressed:
		class_labels.invert()
	
	var vis_only = $MainMargins/MainVbox/VisibleSamplesOnlyCheckbox.pressed
	var matrix_width = num_classes + 2
	var matrix_height = num_classes + 2
	
	var panel_width = max(100 * matrix_width, 500)
	var panel_height = max(25 * matrix_height + 300, 300)
	var cur_size = self.rect_size
	
	if cur_size.x < panel_width:
		cur_size.x = panel_width
	if cur_size.y < panel_height:
		cur_size.y = panel_height
		
	self.set_size(cur_size)
	self.rect_min_size = Vector2(panel_width, panel_height)

	MatrixContainer.columns = matrix_width
	MatrixContainer.add_child(_create_label_control(""))
	MatrixContainer.add_child(_create_vsep())
	
	var index_lookup_table = {}
	
	# Top legend
	for i in range(num_classes):
		MatrixContainer.add_child(_create_label_control(class_labels[i]))
		index_lookup_table[class_labels[i]] = i
		
	for _i in range(matrix_width):
		MatrixContainer.add_child(_create_hsep())
		
	var confusion_matrix = []
	var samples_predicted = 0
	
	for _r in range(num_classes):
		var row_list = []
		for _c in range(num_classes):
			row_list.append(0)
		confusion_matrix.append(row_list)
	
	for s in DataManager.sample_data.keys():
		var sample = DataManager.sample_data[s]
		if vis_only and not sample.visible:
			continue
		
		var prediction = str(DataManager.tree_predict(sample))
		var actual = str(sample.attributes.get(target_attrib, null))
		if actual == null:
			target_attrib = ""
			UiCanvas.show_message_box("Error generating confusion matrix.", "Target attribute '" + target_attrib + "' missing for sample '" + s + "'.")
			clear_all()
			return
		elif not index_lookup_table.has(actual):
			target_attrib = ""
			UiCanvas.show_message_box("Error generating confusion matrix.", "Target actual of '" + actual + "' for sample '" + s + "' is not a valid tree class.")
			clear_all()
			return
			
		var row_index = index_lookup_table[actual]
		var col_index = index_lookup_table[prediction]
		samples_predicted += 1
		confusion_matrix[row_index][col_index] += 1
		
	if samples_predicted <= 0:
		target_attrib = ""
		UiCanvas.show_message_box("Error generating confusion matrix.", "No visible samples are available for metrics.")
		clear_all()
		return
		
	for r in range(num_classes):
		MatrixContainer.add_child(_create_label_control(class_labels[r]))
		MatrixContainer.add_child(_create_vsep())
		for c in range(num_classes):
			MatrixContainer.add_child(_create_label_control(str(confusion_matrix[r][c])))
		
	var sum_diagonal = 0
	for i in range(num_classes):
		sum_diagonal += confusion_matrix[i][i]
		
	var accuracy = float(sum_diagonal) / float(samples_predicted)
	MetricsTextEdit.text += "Accuracy: " + str(accuracy) + "\n"
	
	if num_classes == 2:
		
		var denominator = float(confusion_matrix[1][0] + confusion_matrix[1][1])
		var recall_tp = 0
		if denominator != 0:
			recall_tp = float(confusion_matrix[1][1]) / denominator
		MetricsTextEdit.text += "Recall (TP): " + str(recall_tp) + "\n"
		
		denominator = float(confusion_matrix[0][0] + confusion_matrix[0][1])
		var true_negative_rate = 0
		if denominator != 0:
			true_negative_rate = float(confusion_matrix[0][0]) / denominator
		MetricsTextEdit.text += "True Negative Rate (TN): " + str(true_negative_rate) + "\n"
		
		denominator = float(confusion_matrix[0][0] + confusion_matrix[0][1])
		var false_positive_rate = 0
		if denominator != 0:
			false_positive_rate = float(confusion_matrix[0][1]) / denominator
		MetricsTextEdit.text += "False Positive Rate (FP): " + str(false_positive_rate) + "\n"
		
		denominator = float(confusion_matrix[1][0] + confusion_matrix[1][1])
		var false_negative_rate = 0
		if denominator != 0:
			false_negative_rate = float(confusion_matrix[1][0]) / denominator
		MetricsTextEdit.text += "False Negative Rate (FN): " + str(false_negative_rate) + "\n"
		
		denominator = float(confusion_matrix[0][1] + confusion_matrix[1][1])
		var precision = 0
		if denominator != 0:
			precision = float(confusion_matrix[1][1]) / denominator
		MetricsTextEdit.text += "Precision: " + str(precision) + "\n"
		
		denominator = (precision + recall_tp)
		var f_measure = 0
		if denominator != 0:
			f_measure = (2 * precision * recall_tp) / denominator
		MetricsTextEdit.text += "F-Measure: " + str(f_measure) + "\n"
		
func _on_RefreshBtn_pressed():
	refresh()

func _on_SelectTargetBtn_pressed():
	UiCanvas.show_select_target_attrib_dialog()

func _on_CloseBtn_pressed():
	hide()

func _on_ReverseClassCheckbox_toggled(_button_pressed):
	if target_attrib != null and target_attrib != "":
		refresh()

func _on_VisibleSamplesOnlyCheckbox_toggled(_button_pressed):
	if target_attrib != null and target_attrib != "":
		refresh()
