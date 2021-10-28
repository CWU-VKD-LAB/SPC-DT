extends ConfirmationDialog

func get_height_scale():
	return $MainMargins/MainVbox/HeightScaleHbox/HeightScaleSpinbox.value
	
func get_width_scale():
	return $MainMargins/MainVbox/WidthScaleHbox/WidthScaleSpinbox.value
