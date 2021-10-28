extends Node2D

export(float) var RADIUS = 10
export(float) var RADIUS_MULTI = 1

# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

func _draw():
	return
	draw_circle(position, RADIUS * RADIUS_MULTI, Color.black)
	draw_circle(position, RADIUS * RADIUS_MULTI * 0.8, Color.gray)

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
