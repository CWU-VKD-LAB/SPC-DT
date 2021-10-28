extends Node2D

var sample_data = null

func _on_TextureRect_pressed():
	if sample_data != null:
		UiCanvas.show_new_sample_dialog(true, sample_data)
