extends ConfirmationDialog

func _ready():
	$MarginContainer/ItemList.select(0)

func get_delimeter():
	var selected_item = $MarginContainer/ItemList.get_selected_items()[0]
	
	if selected_item == 0:
		return ","
	elif selected_item == 1:
		return ";"
	else:
		return "\t"
