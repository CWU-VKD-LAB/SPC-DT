extends ConfirmationDialog


func get_darkest_color_value():
	return $MarginContainer/VBoxContainer/MinCompHbox/MinCompSpinbox.value
	
func get_brightest_color_value():
	return $MarginContainer/VBoxContainer/MaxCompHbox/MaxCompSpinbox.value

func get_opacity_value():
	return $MarginContainer/VBoxContainer/OpacityCompHbox/OpacitySpinbox.value
	
func get_visible_only():
	return $MarginContainer/VBoxContainer/VisibleOnlyHbox/VisibleOnlyCheckbox.pressed
