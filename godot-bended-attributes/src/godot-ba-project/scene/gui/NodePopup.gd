extends PopupDialog

var _node_dict = {}

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

func set_node_data(node_dict):
	_node_dict = node_dict
	var attrib = node_dict.get("attribute", null)
	var thresh = node_dict.get("threshold", null)
	var range_min = node_dict.get("range_min", null)
	var range_max = node_dict.get("range_max", null)
	var leaf_class = node_dict.get("leaf_class", null)
	
	if attrib != null:
		$MarginContainer/VBoxContainer/AttribContainer/AttribLabel.text = str(attrib)
	else:
		$MarginContainer/VBoxContainer/AttribContainer/AttribLabel.text = "N/A"
		
	if thresh != null:
		$MarginContainer/VBoxContainer/ThreshContainer/ThreshLabel.text = str(thresh)
	else:
		$MarginContainer/VBoxContainer/ThreshContainer/ThreshLabel.text = "N/A"
		
	if range_min != null:
		$MarginContainer/VBoxContainer/RangeMinContainer/RangeMinLabel.text = str(range_min)
	else:
		$MarginContainer/VBoxContainer/RangeMinContainer/RangeMinLabel.text = "N/A"
		
	if range_max != null:
		$MarginContainer/VBoxContainer/RangeMaxContainer/RangeMaxLabel.text = str(range_max)
	else:
		$MarginContainer/VBoxContainer/RangeMaxContainer/RangeMaxLabel.text = "N/A"
		
	if leaf_class != null:
		$MarginContainer/VBoxContainer/LeafClassContainer/LeafClassLabel.text = str(leaf_class)
	else:
		$MarginContainer/VBoxContainer/LeafClassContainer/LeafClassLabel.text = "N/A"
		
func _unhandled_input(event: InputEvent):
	if event is InputEventKey && event.pressed && event.scancode == KEY_ESCAPE:
		visible = false


func _on_CloseBtn_pressed():
	visible = false

func _on_CountSamplesBtn_pressed():
	if DataManager.sample_data == null or DataManager.sample_data.size() <= 0:
		UiCanvas.show_message_box("Error", "You have to load some samples first before you can count them.")
		return
		
	var num_vis = 0
	var node_counter = [_node_dict, 0]
	
	for s in DataManager.sample_data.keys():
		var sample = DataManager.sample_data[s]
		if not sample.visible:
			continue
			
		DataManager.tree_predict(sample, node_counter)
		num_vis += 1
		
	if num_vis <= 0:
		UiCanvas.show_message_box("Error", "There are currently no samples visible.")
		return
		
	var percent_samples = (node_counter[1] / float(num_vis)) * 100.0
	UiCanvas.show_message_box("Sample Counting Completed", str(node_counter[1]) + " out of " + str(num_vis) + " visible samples pass through this node (" + str(percent_samples) + "%)")
