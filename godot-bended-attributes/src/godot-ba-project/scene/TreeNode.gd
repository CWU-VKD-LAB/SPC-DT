extends Node2D

onready var _left_child_line = $LeftChildLine
onready var _right_child_line = $RightChildLine

var node_dict = null
var attribute setget set_attribute, get_attribute
var threshold setget set_threshold, get_threshold
var operator setget set_operator, get_operator
var leaf_class setget set_leaf_class, get_leaf_class

var parent_instance = null
var left_child_instance setget set_left_child, get_left_child
var right_child_instance setget set_right_child, get_right_child

var draw_range_ticks = false

var tree_depth = 0

func _enter_tree():
	left_child_instance = null
	right_child_instance = null

# Called when the node enters the scene tree for the first time.
func _ready():
	$TextureButton.self_modulate = UiCanvas.NodeColorPicker.color
	UiCanvas.NodeColorPicker.connect("color_changed", self, "_on_node_color_picker_changed")
# warning-ignore:return_value_discarded
	DataManager.connect("spline_mode_changed", self, "_on_spline_mode_changed")

func _on_node_color_picker_changed(color):
	$TextureButton.self_modulate = color

func _on_spline_mode_changed(new_mode):
	if new_mode == DataManager.SplineMode.Simple:
		draw_range_ticks = false
	else:
		draw_range_ticks = true
		
	update_child_lines()

func set_attribute(val):
	if val != null:
		$AttribClassLabel.text = val
	else:
		$AttribClassLabel.text = ""
		
	attribute = val
	
func get_attribute():
	return attribute
	
func set_threshold(val):
	if val != null:
		$ThreshLabel.text = str(val)
	else:
		$ThreshLabel.text = ""
		
	threshold = val
	
func get_threshold():
	return threshold

func set_operator(op):
	operator = op
	if operator != null:
		$OperatorLabel.text = operator
		$OperatorLabel.visible = true
	else:
		$OperatorLabel.text = ""
		$OperatorLabel.visible = false
	
func get_operator():
	return operator

func set_leaf_class(val):
	if val != null and attribute == null:
		$AttribClassLabel.text = str(val)
		
	leaf_class = val
	
func get_leaf_class():
	return leaf_class

func get_left_child_range_positions():
	return $LeftChildLine.get_tick_positions()

func get_left_child_ranges():
	if operator == "<" or operator == "<=":
		return [node_dict.threshold, node_dict.range_min]
	else:
		return [node_dict.threshold, node_dict.range_max]

func get_right_child_range_positions():
	return $RightChildLine.get_tick_positions()
	
func get_right_child_ranges():
	if operator == "<" or operator == "<=":
		return [node_dict.threshold, node_dict.range_max]
	else:
		return [node_dict.threshold, node_dict.range_min]

func set_left_child(instance):
	left_child_instance = instance
	
func get_left_child():
	return left_child_instance

func set_right_child(instance):
	right_child_instance = instance
	
func get_right_child():
	return right_child_instance

func update_range_positions():
	if left_child_instance != null:
		left_child_instance.update_range_positions()
	if right_child_instance != null:
		right_child_instance.update_range_positions()
		
func update_child_lines():
	if left_child_instance != null:
		_left_child_line.visible = true
		_left_child_line.dest_pos = left_child_instance.global_position
	else:
		_left_child_line.visible = false
		_left_child_line.dest_pos = global_position
		
	if right_child_instance != null:
		_right_child_line.visible = true
		_right_child_line.dest_pos = right_child_instance.global_position
	else:
		_right_child_line.visible = false
		_right_child_line.dest_pos = global_position
		
	_left_child_line.draw_range_ticks = draw_range_ticks
	_right_child_line.draw_range_ticks = draw_range_ticks
	_left_child_line.update()
	_right_child_line.update()
		


func _on_TextureButton_pressed():
	UiCanvas.show_node_popup(node_dict)
