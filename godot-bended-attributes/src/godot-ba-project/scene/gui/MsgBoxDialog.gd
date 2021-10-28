extends AcceptDialog


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


func show_msg_box(title, message):
	window_title = title
	$MsgTextLabel.text = message
	popup_centered()
