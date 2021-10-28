extends Node2D

const DEFAULT_TREE_SEPARATION_WIDTH = 240
const DEFAULT_TREE_LAYER_HEIGHT = 200

export(float) var TREE_NODE_SEPARATION_WIDTH = DEFAULT_TREE_SEPARATION_WIDTH
export(float) var TREE_NODE_MIN_CHILD_SEP = 80
export(float) var TREE_LAYER_HEIGHT = DEFAULT_TREE_LAYER_HEIGHT

onready var _treeContainer = $TreeContainer
onready var _splineContainer = $SplineContainer
onready var TreeNode = preload("res://scene/TreeNode.tscn")
onready var SampleSpline = preload("res://scene/SampleSpline.tscn")
onready var RangeNode = preload("res://scene/RangeNode.tscn")
onready var rng = RandomNumberGenerator.new()

var _root_instance = null
var _node_layer_counts = []
var _max_depth = 0

func _ready():
	rng.randomize()
	
# warning-ignore:return_value_discarded
	DataManager.connect("tree_data_changed", self, "_on_tree_data_changed")
# warning-ignore:return_value_discarded
	DataManager.connect("sample_data_deleted", self, "_on_sample_data_deleted")
# warning-ignore:return_value_discarded
	DataManager.connect("sample_visibility_changed", self, "_on_sample_visibility_changed")
# warning-ignore:return_value_discarded
	DataManager.connect("sample_color_changed", self, "_on_sample_color_changed")
# warning-ignore:return_value_discarded
	DataManager.connect("sample_data_changed", self, "_on_sample_data_changed")
# warning-ignore:return_value_discarded
	DataManager.connect("spline_mode_changed", self, "_on_spline_mode_changed")
# warning-ignore:return_value_discarded
	DataManager.connect("spline_update_needed", self, "_on_spline_update_needed")
# warning-ignore:return_value_discarded
	UiCanvas.TreeScalingDialog.connect("confirmed", self, "_on_tree_scaling_dialog_confirmed")
	
func _on_tree_scaling_dialog_confirmed():
	var height_scale = UiCanvas.TreeScalingDialog.get_height_scale()
	var width_scale = UiCanvas.TreeScalingDialog.get_width_scale()
	
	TREE_LAYER_HEIGHT = int(DEFAULT_TREE_LAYER_HEIGHT * height_scale)
	TREE_NODE_SEPARATION_WIDTH = int(DEFAULT_TREE_SEPARATION_WIDTH * width_scale)
	
	update_tree_position()
	
	# Call deferred needed because tree positions aren't fully updated until end of frame
	# This effectively delays the refresh_all_splines() function call until next frame
	self.call_deferred("refresh_all_splines")
	
func _on_tree_data_changed():
	clear_all_splines()
	build_tree()
	update_tree_position()
	$Camera2D.reset_camera()

func _on_sample_data_deleted(sample):
	clear_spline(sample)

func _on_sample_visibility_changed(sample):
	if sample.visible:
		add_sample_spline(sample)
	else:
		clear_spline(sample)

func _on_sample_color_changed(sample):
	var _spline_instance = sample.get("spline_instance", null)
	if _spline_instance != null:
		_spline_instance.spline_color = sample.spline_color
		_spline_instance.update()

func _on_sample_data_changed(sample):
	if sample.visible:
		clear_spline(sample)
		add_sample_spline(sample)

func refresh_all_splines():
	if DataManager.sample_data == null:
		return
		
	# Recreate all visible splines
	for s in DataManager.sample_data.keys():
		var sample = DataManager.sample_data[s]
		if sample.visible:
			add_sample_spline(sample)
			
func _on_spline_mode_changed(_new_mode):
	refresh_all_splines()
			
func _on_spline_update_needed():
	refresh_all_splines()
	
func clear_all_splines():
	for node in _splineContainer.get_children():
		_splineContainer.remove_child(node)
		node.queue_free()

func clear_spline(sample_data):
	var _spine_instance = sample_data.get("spline_instance", null)
	if _spine_instance != null:
		_splineContainer.remove_child(_spine_instance)
		_spine_instance.curve.clear_points()
		_spine_instance.queue_free()
		sample_data["spline_instance"] = null

func add_sample_spline(sample_data):
	if DataManager.spline_mode == DataManager.SplineMode.Simple:
		add_simple_sample_spline(sample_data)
	else:
		add_advanced_sample_spline(sample_data)
	
func add_simple_sample_spline(sample_data):
	clear_spline(sample_data)
		
	var _spline_instance = SampleSpline.instance()
	_spline_instance.spline_color = sample_data.spline_color
	_splineContainer.add_child(_spline_instance)
	add_simple_spline_to_node(_spline_instance, _root_instance, sample_data)
	_spline_instance.smooth(true)
	sample_data["spline_instance"] = _spline_instance
	
func add_simple_spline_to_node(_spline_instance, node_instance, sample_data):
	_spline_instance.curve.add_point(node_instance.global_position)
	
	var attrib = node_instance.attribute
	var threshold = node_instance.threshold
	var operator = node_instance.operator
		
	if attrib == null:
		return # Leaf node?
		
	var sample_value = sample_data.attributes[attrib]
	var condition_passed = false

	var expr_error = DataManager.node_condition_expression.parse("value " + operator + " threshold", ["value", "threshold"])
	if expr_error != OK:
		print("Warning: Error evaluting node condition operator '" + operator + "'. Defaulting to '<'.")
		condition_passed = sample_value < threshold
	else:
		condition_passed = bool(DataManager.node_condition_expression.execute([sample_value, threshold], self))
	
	var child_instance = null
	var rotation_angle = null
	
	if condition_passed and node_instance.left_child_instance != null:
		child_instance = node_instance.left_child_instance
		rotation_angle = PI / 2
	elif not condition_passed and node_instance.right_child_instance != null:
		child_instance = node_instance.right_child_instance
		rotation_angle = -1 * PI / 2
	else:
		return # No child even though there should be
		
	var paraent_pos = node_instance.global_position
	var child_pos = child_instance.global_position
	var distance = paraent_pos.distance_to(child_pos)
	
	var mid_point = ((child_pos - paraent_pos) / 2) + paraent_pos
	var rotated_point = (child_pos - paraent_pos).normalized().rotated(rotation_angle) * (distance / 4) * rng.randf_range(0.5, 1.25)
	mid_point += mid_point.normalized() * distance * rng.randf_range(-0.1, 0.1)
	mid_point += rotated_point
	_spline_instance.curve.add_point(mid_point)
	
	add_simple_spline_to_node(_spline_instance, child_instance, sample_data)

func add_advanced_sample_spline(sample_data):
	clear_spline(sample_data)
		
	var start_position = Vector2(0, -1 * DEFAULT_TREE_LAYER_HEIGHT / 4.0)
	var _spline_instance = SampleSpline.instance()
	
	_spline_instance.spline_color = sample_data.spline_color
	_splineContainer.add_child(_spline_instance)
	_spline_instance.curve.add_point(start_position)
	
	add_advanced_spline_to_node(_spline_instance, _root_instance, start_position, sample_data)
	
	_spline_instance.smooth(true, true)
	var num_points = _spline_instance.curve.get_point_count()
	
	if num_points == 3 and _spline_instance.has_leaf_child:
		_spline_instance.curve.clear_points()
	else:
		if num_points > 2:
			_spline_instance.curve.remove_point(0)
		if num_points > 3:
			_spline_instance.curve.remove_point(0)
		
	sample_data["spline_instance"] = _spline_instance
	
func is_leaf_node(node_instance):
	if node_instance == null:
		return true
	return node_instance.leaf_class != null
	
func add_advanced_spline_to_node(_spline_instance, node_instance, start_position, sample_data):
	var attrib = node_instance.attribute
	var threshold = node_instance.threshold
	var operator = node_instance.operator
		
	# Leaf node?
	if attrib == null:
		return
		
	var sample_value = sample_data.attributes[attrib]
	var condition_passed = false

	var expr_error = DataManager.node_condition_expression.parse("value " + operator + " threshold", ["value", "threshold"])
	if expr_error != OK:
		print("Warning: Error evaluting node condition operator '" + operator + "'. Defaulting to '<'.")
		condition_passed = sample_value < threshold
	else:
		condition_passed = bool(DataManager.node_condition_expression.execute([sample_value, threshold], self))
		
	var child_instance = null
	var rotation_angle = null
	var range_positions = null
	var node_ranges = null
	
	if condition_passed and node_instance.left_child_instance != null:
		child_instance = node_instance.left_child_instance
		rotation_angle = PI / 2
		range_positions = node_instance.get_left_child_range_positions()
		node_ranges = node_instance.get_left_child_ranges()
	elif not condition_passed and node_instance.right_child_instance != null:
		child_instance = node_instance.right_child_instance
		rotation_angle = -1 * PI / 2
		range_positions = node_instance.get_right_child_range_positions()
		node_ranges = node_instance.get_right_child_ranges()
	else:
		return # No child node, even though there should be
		
	var child_is_leaf = is_leaf_node(child_instance)
	if child_is_leaf:
		var leaf_spline = SampleSpline.instance()
		leaf_spline.spline_color = sample_data.spline_color
		_spline_instance.add_child(leaf_spline)
		_spline_instance.has_leaf_child = true
		leaf_spline.curve.add_point(start_position)
		leaf_spline.z_index = 2
		_spline_instance = leaf_spline
	
	var spline_dest = null
	
	# min == threshold ?
	if node_ranges[0] == node_ranges[1]:
		spline_dest = range_positions[0]
	else:
		var offset = sample_value - node_ranges[0]
		var delta = abs(offset / (node_ranges[1] - node_ranges[0]))
		var branch_direction = range_positions[0].direction_to(range_positions[1])
		var branch_distance = range_positions[0].distance_to(range_positions[1])
		spline_dest = range_positions[0] + (branch_direction * branch_distance * delta)

	var distance = start_position.distance_to(spline_dest)
	var mid_point = ((spline_dest - start_position) / 2) + start_position
	var rotated_point = (spline_dest - start_position).normalized().rotated(rotation_angle)
		
	rotated_point *= (distance / 4)
	mid_point += rotated_point
	_spline_instance.curve.add_point(mid_point)
	_spline_instance.curve.add_point(spline_dest)
	
	var new_range_node = RangeNode.instance()
	new_range_node.global_position = spline_dest
	new_range_node.sample_data = sample_data
	_spline_instance.add_child(new_range_node)
	
	if child_is_leaf:
		_spline_instance.smooth(true, true)
		if node_instance == _root_instance:
			_spline_instance.curve.remove_point(0)
	
	add_advanced_spline_to_node(_spline_instance, child_instance, spline_dest, sample_data)
		
func build_tree():
	if _root_instance != null:
		_treeContainer.remove_child(_root_instance)
		_root_instance.queue_free()
		_root_instance = null
		
	_node_layer_counts = []
	_max_depth = 0
	var root_dict = DataManager.tree_data
	if root_dict != null:
		_root_instance = build_node(root_dict, _treeContainer, 0)
	
func update_tree_position():
	if _root_instance == null:
		return
		
	_root_instance.global_position = Vector2.ZERO
	update_node_position(_root_instance.left_child_instance, 0, 1)
	update_node_position(_root_instance.right_child_instance, 1, 1)
	
func update_node_position(node, side, depth):
	if node == null:
		return
	
	var layer_nodes = pow(2, _max_depth - depth)
		
	var layer_width = layer_nodes * TREE_NODE_SEPARATION_WIDTH
	
	var children_separation = layer_width / 2
	if children_separation < TREE_NODE_MIN_CHILD_SEP:
		children_separation = TREE_NODE_MIN_CHILD_SEP
		
	var parent_position = node.parent_instance.global_position
	
	if side == 0:
		node.global_position = Vector2(parent_position.x - children_separation, parent_position.y + TREE_LAYER_HEIGHT)
	else:
		node.global_position = Vector2(parent_position.x + children_separation, parent_position.y + TREE_LAYER_HEIGHT)
		
	#node.parent_instance.update_range_positions()
	node.parent_instance.update_child_lines()
	
	update_node_position(node.left_child_instance, 0, depth + 1)
	update_node_position(node.right_child_instance, 1, depth + 1)
	
	# node.set_circle_radius_mult(_max_depth + 1 - depth)
	
	if node.left_child_instance == null and node.right_child_instance == null:
		#node.update_range_positions()
		node.update_child_lines()
	
func build_node(node_dict, parent, depth):
	while len(_node_layer_counts) <= depth:
		_node_layer_counts.append(0)
	
	_node_layer_counts[depth] += 1
	
	if depth > _max_depth:
		_max_depth = depth
	
	var node_instance = TreeNode.instance()
	parent.add_child(node_instance)
	
	node_dict["instance"] = node_instance
	node_instance.node_dict = node_dict
	
	node_instance.attribute = node_dict.get("attribute", null)
	node_instance.threshold = node_dict.get("threshold", null)
	node_instance.operator = node_dict.get("operator", null)
	node_instance.leaf_class = node_dict.get("leaf_class", null)
	node_instance.tree_depth = depth
	node_instance.parent_instance = parent
	
	var left_child = node_dict.get("child_left", null)
	var right_child = node_dict.get("child_right", null)
	
	if left_child != null:
		node_instance.left_child_instance = build_node(node_dict.child_left, node_instance, depth + 1)
		
	if right_child != null:
		node_instance.right_child_instance = build_node(node_dict.child_right, node_instance, depth + 1)
	
	return node_instance
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
