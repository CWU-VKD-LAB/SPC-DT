extends Camera2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var DEFAULT_POSITION = Vector2(0, 200)
var PAN_VELOCITY = 800
var ZOOM_KEY_VELOCITY = 1
var ZOOM_SCROLL_VELOCITY = 6

var _previousPosition: Vector2 = Vector2(0, 0);
var _moveCamera: bool = false;
var _scroll_up_ticks = 0
var _scroll_down_ticks = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	reset_camera()

func reset_camera():
	global_position = DEFAULT_POSITION
	zoom = Vector2(1, 1)
	
func _physics_process(delta):
	var move_dir = Vector2.ZERO
#	if Input.is_action_pressed("ui_left"):
#		move_dir += Vector2(-1, 0)
#	if Input.is_action_pressed("ui_right"):
#		move_dir += Vector2(1, 0)
#	if Input.is_action_pressed("ui_up"):
#		move_dir += Vector2(0, -1)
#	if Input.is_action_pressed("ui_down"):
#		move_dir += Vector2(0, 1)
#
#	if Input.is_action_pressed("camera_zoom_in"):
#		#zoom -= Vector2(ZOOM_KEY_VELOCITY, ZOOM_KEY_VELOCITY) * delta
#		zoom -= zoom * ZOOM_KEY_VELOCITY * delta
#	if Input.is_action_pressed("camera_zoom_out"):
#		zoom += zoom * ZOOM_KEY_VELOCITY * delta
		
	while _scroll_up_ticks > 0:
		zoom -= zoom * ZOOM_SCROLL_VELOCITY * delta
		_scroll_up_ticks -= 1
		
	while _scroll_down_ticks > 0:
		zoom += zoom * ZOOM_SCROLL_VELOCITY * delta
		_scroll_down_ticks -= 1
		
	if zoom.x < 0.05:
		zoom = Vector2(0.05, 0.05)
	elif zoom.x > 10:
		zoom = Vector2(10, 10)
		
	global_position += move_dir.normalized() * PAN_VELOCITY * delta * zoom
	
	#if Input.is_action_just_pressed("camera_reset"):
		#reset_camera()



func _unhandled_input(event: InputEvent):
	if event is InputEventMouseButton && event.button_index == BUTTON_LEFT:
		get_tree().set_input_as_handled()
		if event.is_pressed():
			_previousPosition = event.position
			_moveCamera = true
		else:
			_moveCamera = false
			
	elif event is InputEventMouseMotion && _moveCamera:
		get_tree().set_input_as_handled();
		position += (_previousPosition - event.position) * zoom
		_previousPosition = event.position
	
	elif event is InputEventMouseButton && event.is_pressed():
		if event.button_index == BUTTON_WHEEL_UP:
			_scroll_up_ticks += 1
		elif event.button_index == BUTTON_WHEEL_DOWN:
			_scroll_down_ticks += 1

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
