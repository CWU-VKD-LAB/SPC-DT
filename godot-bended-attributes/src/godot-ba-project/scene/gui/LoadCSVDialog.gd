extends WindowDialog

onready var FilePathEdit = $MarginContainer/MainVBox/CSVFileHBox/FilePathEdit
onready var ImportLimitSpinbox = $MarginContainer/MainVBox/ImportLimitContainer/ImportLimitSpinbox
onready var MissingValuesList = $MarginContainer/MainVBox/EmptyValuesContainer/MissingValuesList
onready var NameTypeList = $MarginContainer/MainVBox/NamingTypeContainer/NameTypeList
onready var ColumnNameContainer = $MarginContainer/MainVBox/ColumnNameContainer
onready var ColumnNameList = $MarginContainer/MainVBox/ColumnNameContainer/ColumnNameList
onready var NamePrefixContainer = $MarginContainer/MainVBox/NamePrefixContainer
onready var NamePrefixEdit = $MarginContainer/MainVBox/NamePrefixContainer/NamePrefixEdit

var csv_file_info = null
var csv_file_attributes = null
var csv_file_delim = ","

func _ready():
	FilePathEdit.text = ""

func set_csv_file_path(file_path):
	FilePathEdit.text = file_path

func set_csv_file_info(file_info):
	csv_file_info = file_info
	set_csv_file_attributes(csv_file_info[0])

func set_csv_file_attributes(attribs):
	ColumnNameList.items.clear()
	csv_file_attributes = attribs
	for a in attribs.keys():
		ColumnNameList.add_item(a)

func set_csv_file_delim(delim):
	csv_file_delim = delim

func _on_CancelBtn_pressed():
	csv_file_info[1].close() # Close CSV file handle
	hide()


func _on_LoadCSVDialog_about_to_show():
	ImportLimitSpinbox.value = 100
	MissingValuesList.select(0)
	NameTypeList.select(0)
	ColumnNameList.select(0)
	ColumnNameContainer.visible = true
	NamePrefixContainer.visible = not ColumnNameContainer.visible
	NamePrefixEdit.text = "Sample "


func _on_NameTypeList_item_selected(index):
	ColumnNameContainer.visible = index == 0
	NamePrefixContainer.visible = not ColumnNameContainer.visible

func _on_ImportSamplesBtn_pressed():
	var ignore_empty = MissingValuesList.get_selected_items()[0] == 0
	
	var name_column = null
	if NameTypeList.get_selected_items()[0] == 0:
		name_column = ColumnNameList.get_item_text(ColumnNameList.get_selected_items()[0])
		
	var import_count = DataManager.import_samples_from_csv(csv_file_info[1], csv_file_delim, csv_file_info[0], ImportLimitSpinbox.value, ignore_empty, name_column, NamePrefixEdit.text)
		
	csv_file_info[1].close() # Close CSV file handle
	hide()
	
	if import_count > 0:
		UiCanvas.show_message_box("Import completed.", str(import_count) + " samples were imported.")
