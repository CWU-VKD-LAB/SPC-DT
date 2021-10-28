class_name SmoothPath
extends Path2D

export(float) var spline_length = 25
export(float) var spline_length_factor = 0.1
export(Color) var spline_color setget set_color, get_color
export(bool) var _smooth setget smooth
export(bool) var _straighten setget straighten

var has_leaf_child = false

func _enter_tree():
	curve = Curve2D.new()
	# warning-ignore:return_value_discarded
	DataManager.connect("adv_leaf_color_mode_changed", self, "_on_adv_leaf_color_mode_changed")

func _on_adv_leaf_color_mode_changed(_new_mode):
	if has_leaf_child:
		update()

func set_color(new_color):
	spline_color = new_color
	if has_leaf_child:
		var child = get_node_or_null("SampleSpline")
		if child != null:
			child.spline_color = new_color
			child.update()
	
func get_color():
	return spline_color

func straighten(value):
	if not value: return
	for i in curve.get_point_count():
		curve.set_point_in(i, Vector2())
		curve.set_point_out(i, Vector2())

func smooth(value, even_only = false):
	if not value: return

	var point_count = curve.get_point_count()
	for i in point_count:
		if i == 0 or i == point_count - 1:
			continue
		elif even_only and i % 2 == 0:
			continue
			
		var spline = _get_spline(i)
		curve.set_point_in(i, -spline)
		curve.set_point_out(i, spline)

func _get_spline(i):
	var last_point = _get_point(i - 1)
	var next_point = _get_point(i + 1)
	var spline = last_point.direction_to(next_point) * spline_length_factor * last_point.distance_to(next_point)
	return spline

func _get_point(i):
	var point_count = curve.get_point_count()
	i = wrapi(i, 0, point_count)
	return curve.get_point_position(i)

func _draw():
	var points = curve.get_baked_points()

	if points and points.size() >= 2:
		if has_leaf_child and DataManager.adv_leaf_color_mode:
			draw_polyline(points, Color.gray, 2, true)
		else:
			draw_polyline(points, spline_color, 2, true)
