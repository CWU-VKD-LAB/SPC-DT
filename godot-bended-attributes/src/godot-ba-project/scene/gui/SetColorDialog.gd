extends ConfirmationDialog

func get_color_gradient() -> Gradient:
	var first_color = $MainMargins/MainVbox/StartColorHbox/StartColorPicker.color
	var second_color = $MainMargins/MainVbox/EndColorHbox/EndColorPicker.color
	
	var gradient = Gradient.new()
	gradient.set_color(0, first_color)
	gradient.set_color(1, second_color)
	return gradient

func get_visible_only() -> bool:
	return $MainMargins/MainVbox/VisibleOnlyCheckbox.pressed
