extends Node2D

export(Vector2) var dest_pos
export(Color) var line_color = Color.black
export(bool) var draw_range_ticks = false
export(float) var range_ticks_padding = 40
export(float) var range_tick_length = 15

var _start_tick_position = null
var _end_tick_position = null

# Called when the node enters the scene tree for the first time.
func _ready():
	line_color = UiCanvas.BranchColorPicker.color
	UiCanvas.BranchColorPicker.connect("color_changed", self, "_on_branch_color_picker_changed")
	
func _on_branch_color_picker_changed(color):
	line_color = color
	update()
	
func get_tick_positions():
	return [to_global(_start_tick_position), to_global(_end_tick_position)]
	
func update_range_positions():
	var dest_local = to_local(dest_pos)
	var dir_vector = position.direction_to(dest_local)
	var line_length = position.distance_to(dest_local)
	_start_tick_position = position + (dir_vector * range_ticks_padding)
	_end_tick_position = position + (dir_vector * (line_length - range_ticks_padding))
	
func _draw():
	var dest_local = to_local(dest_pos)
	var dir_vector = position.direction_to(dest_local)
	var cross_vector = dir_vector.rotated(PI / 2) * range_tick_length
	var line_length = position.distance_to(dest_local)
	_start_tick_position = position + (dir_vector * range_ticks_padding)
	_end_tick_position = position + (dir_vector * (line_length - range_ticks_padding))
	
	draw_line(position, dest_local, line_color, 4.0, true)
	
	if draw_range_ticks:
		draw_line(_start_tick_position - cross_vector, _start_tick_position + cross_vector, line_color, 2.0, true)
		draw_line(_end_tick_position - cross_vector, _end_tick_position + cross_vector, line_color, 2.0, true)
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
