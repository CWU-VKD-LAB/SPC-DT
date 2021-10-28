extends WindowDialog

onready var FiltersVbox = $MarginContainer/MainVbox/Panel/FiltersMarginContainer/FiltersScrollContainer/FiltersVbox
onready var FilterListItem = preload("res://scene/gui/FilterListItem.tscn")

func _ready():
	# warning-ignore:return_value_discarded
	DataManager.connect("tree_data_changed", self, "_on_tree_data_changed")
	# warning-ignore:return_value_discarded
	DataManager.connect("filter_added", self, "_on_filter_added")
	# warning-ignore:return_value_discarded
	DataManager.connect("filter_deleted", self, "_on_filter_deleted")

func _on_tree_data_changed():
	hide()
	for n in FiltersVbox.get_children():
		FiltersVbox.remove_child(n)
		n.queue_free()
	
func _on_filter_added(filter):
	var new_list_item = FilterListItem.instance()
	new_list_item.set_filter_instance(filter)
	FiltersVbox.add_child(new_list_item)

func _on_filter_deleted(filter):
	FiltersVbox.remove_child(filter.list_instance)
	filter.list_instance.queue_free()

func _on_CloseBtn_pressed():
	hide()

func _on_NewFilterBtn_pressed():
	UiCanvas.show_new_filter_dialog()
