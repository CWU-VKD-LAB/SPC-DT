///-------------------------------------------------------------------------------------------------
// file:	Form1.h
//
// summary:	Declares the form 1 class
///-------------------------------------------------------------------------------------------------

#pragma once

#include "OpenGL3.h"


namespace OpenGL_on_a_Windows_Form 
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace OpenGLForm;

	/// <summary>	Type of the graph. </summary>
	int graphType = -1;
	/// <summary>	True if fileopened. </summary>
	bool fileopened = false;
	bool parsefileOpened = false;
	/// <summary>	Just to hold the file name for now to send to the graphs. </summary>
	ClassData data;
	parseData dataParsed;
	FileHandling test;

	/// <summary>	The old zoom value. </summary>
	int oldZoomValue = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 	Summary for Form1
	/// 	
	/// 	WARNING: If you change the name of this class, you will need to change the
	/// 	         'Resource File Name' property for the managed resource compiler tool associated
	/// 	         with all .resx files this class depends on.  Otherwise, the designers will not
	/// 	         be able to interact properly with localized resources associated with this form.
	/// </summary>
	///
	/// <remarks>	Barim, 3/3/2019. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Default constructor. </summary>
		///
		////////////////////////////////////////////////////////////////////////////////////////////////////

		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			OpenGL3 = gcnew COpenGL3(this, this->Width - this->panel1->Width - 40 - 200, //Width
				this->Height - this->menuStrip1->Height - this->statusStrip1->Height - 55, //Height
				graphType);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Destructor. </summary>
		///
		////////////////////////////////////////////////////////////////////////////////////////////////////

		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	/// <summary>	The components. </summary>
	private: System::ComponentModel::IContainer^  components;
	private: int previousTrackBar2Value; 
	private: int previousNonOrthoXValue;
	private: int previousNonOrthoYValue;
	private: int previousTrackBar3Value; 
	//private: System::Collections::ArrayList checkedIndex;	  
	protected:
	/// <summary>	The first timer. </summary>
	/// <summary>	The first timer. </summary>
	private: System::Windows::Forms::Timer^  timer1;

	/// <summary>	The fourth graph. </summary>
	private: System::Windows::Forms::Button^  graph4;
	/// <summary>	The first panel. </summary>
	private: System::Windows::Forms::Panel^  panel1;
	/// <summary>	The first menu strip. </summary>
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	/// <summary>	The file tool strip menu item. </summary>


	/// <summary>	The first scroll bar. </summary>
	private: System::Windows::Forms::VScrollBar^  vScrollBar1;
	/// <summary>	The first scroll bar. </summary>
	private: System::Windows::Forms::HScrollBar^  hScrollBar1;
	/// <summary>	The first track bar. </summary>
	private: System::Windows::Forms::TrackBar^  trackBar1;
	/// <summary>	The pannning label. </summary>
	private: System::Windows::Forms::Label^  PannningLabel;
	/// <summary>	The zooming label. </summary>
	private: System::Windows::Forms::Label^  ZoomingLabel;
	/// <summary>	The first status strip. </summary>
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	/// <summary>	The graph 8. </summary>

	/// <summary>	The first group box. </summary>
	private: System::Windows::Forms::GroupBox^  groupBox1;



	private: System::Windows::Forms::ColorDialog^  colorDialog1;
	private: System::Windows::Forms::Button^  colorButton;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Label^  label1;

	private: System::Windows::Forms::Label^  label2;


	private: System::Windows::Forms::Panel^  panel2;




private: System::Windows::Forms::Button^ button3;
private: System::Windows::Forms::Button^ button4;




private: System::Windows::Forms::Button^ button5;






















private: System::Windows::Forms::Button^ button1;
private: System::Windows::Forms::Button^ button6;
private: System::Windows::Forms::Button^ button2;
private: System::Windows::Forms::CheckBox^ checkBox1;
private: System::Windows::Forms::CheckedListBox^ checkedListBox4;
private: System::Windows::Forms::Label^ label9;
private: System::Windows::Forms::Button^ button7;
private: System::Windows::Forms::Button^ button8;
private: System::Windows::Forms::GroupBox^ groupBox2;
private: System::Windows::Forms::TrackBar^ transparencySlider;

private: System::Windows::Forms::Label^ transparencyLabel;
private: System::Windows::Forms::GroupBox^ groupBox3;
private: System::Windows::Forms::TrackBar^ backgroundTransparencySlider;

private: System::Windows::Forms::Label^ backgroundTransparencyLabel;
private: System::Windows::Forms::GroupBox^ groupBox4;
private: System::Windows::Forms::TrackBar^ backgroundColorLightnessSlider;

private: System::Windows::Forms::Label^ backgroundLightness;
private: System::Windows::Forms::Button^ drawRectangleButton;
private: System::Windows::Forms::Button^ clearRectangleButton;
private: System::Windows::Forms::ComboBox^ classTransparencySelection;
private: System::Windows::Forms::CheckBox^ lineTerminationModeCheckbox;
private: System::Windows::Forms::CheckBox^ highlightMisclassificationsCheckbox;
private: System::Windows::Forms::CheckBox^ pointColorMode;
private: System::Windows::Forms::Button^ SwapAttributesButton;
private: System::Windows::Forms::CheckBox^ lineColorCheckbox;
private: System::Windows::Forms::Button^ button9;
































	private:
		/// <summary>	Required designer variable. </summary>

		OpenGLForm::COpenGL3 ^ OpenGL3;

#pragma region Windows Form Designer generated code

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Required method for Designer support - do not modify the contents of this method with the
		/// 	code editor.
		/// </summary>
		///
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->graph4 = (gcnew System::Windows::Forms::Button());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->SwapAttributesButton = (gcnew System::Windows::Forms::Button());
			this->clearRectangleButton = (gcnew System::Windows::Forms::Button());
			this->drawRectangleButton = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->checkedListBox4 = (gcnew System::Windows::Forms::CheckedListBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->ZoomingLabel = (gcnew System::Windows::Forms::Label());
			this->vScrollBar1 = (gcnew System::Windows::Forms::VScrollBar());
			this->hScrollBar1 = (gcnew System::Windows::Forms::HScrollBar());
			this->PannningLabel = (gcnew System::Windows::Forms::Label());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->colorButton = (gcnew System::Windows::Forms::Button());
			this->colorDialog1 = (gcnew System::Windows::Forms::ColorDialog());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->lineColorCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->pointColorMode = (gcnew System::Windows::Forms::CheckBox());
			this->highlightMisclassificationsCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->lineTerminationModeCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->backgroundColorLightnessSlider = (gcnew System::Windows::Forms::TrackBar());
			this->backgroundLightness = (gcnew System::Windows::Forms::Label());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->backgroundTransparencySlider = (gcnew System::Windows::Forms::TrackBar());
			this->backgroundTransparencyLabel = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->classTransparencySelection = (gcnew System::Windows::Forms::ComboBox());
			this->transparencySlider = (gcnew System::Windows::Forms::TrackBar());
			this->transparencyLabel = (gcnew System::Windows::Forms::Label());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->panel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			this->groupBox1->SuspendLayout();
			this->tableLayoutPanel1->SuspendLayout();
			this->panel2->SuspendLayout();
			this->groupBox4->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->backgroundColorLightnessSlider))->BeginInit();
			this->groupBox3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->backgroundTransparencySlider))->BeginInit();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->transparencySlider))->BeginInit();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// graph4
			// 
			this->graph4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->graph4->Location = System::Drawing::Point(9, 9);
			this->graph4->Margin = System::Windows::Forms::Padding(6);
			this->graph4->Name = L"graph4";
			this->graph4->Size = System::Drawing::Size(191, 70);
			this->graph4->TabIndex = 3;
			this->graph4->Text = L"Reset Data";
			this->graph4->UseVisualStyleBackColor = true;
			this->graph4->Click += gcnew System::EventHandler(this, &Form1::graph4_Click);
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->panel1->Controls->Add(this->button9);
			this->panel1->Controls->Add(this->SwapAttributesButton);
			this->panel1->Controls->Add(this->clearRectangleButton);
			this->panel1->Controls->Add(this->drawRectangleButton);
			this->panel1->Controls->Add(this->button7);
			this->panel1->Controls->Add(this->button8);
			this->panel1->Controls->Add(this->checkedListBox4);
			this->panel1->Controls->Add(this->label9);
			this->panel1->Controls->Add(this->checkBox1);
			this->panel1->Controls->Add(this->button6);
			this->panel1->Controls->Add(this->button2);
			this->panel1->Controls->Add(this->button1);
			this->panel1->Controls->Add(this->button4);
			this->panel1->Controls->Add(this->button3);
			this->panel1->ForeColor = System::Drawing::SystemColors::ControlText;
			this->panel1->Location = System::Drawing::Point(0, 0);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(200, 775);
			this->panel1->TabIndex = 4;
			this->panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::panel1_Paint);
			// 
			// SwapAttributesButton
			// 
			this->SwapAttributesButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			this->SwapAttributesButton->Location = System::Drawing::Point(12, 648);
			this->SwapAttributesButton->Name = L"SwapAttributesButton";
			this->SwapAttributesButton->Size = System::Drawing::Size(173, 41);
			this->SwapAttributesButton->TabIndex = 33;
			this->SwapAttributesButton->Text = L"Swap X/Y";
			this->SwapAttributesButton->UseVisualStyleBackColor = true;
			this->SwapAttributesButton->Click += gcnew System::EventHandler(this, &Form1::SwapAttributesButton_Click);
			// 
			// clearRectangleButton
			// 
			this->clearRectangleButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			this->clearRectangleButton->Location = System::Drawing::Point(103, 438);
			this->clearRectangleButton->Name = L"clearRectangleButton";
			this->clearRectangleButton->Size = System::Drawing::Size(85, 61);
			this->clearRectangleButton->TabIndex = 32;
			this->clearRectangleButton->Text = L"Clear Rectangle";
			this->clearRectangleButton->UseVisualStyleBackColor = true;
			this->clearRectangleButton->Click += gcnew System::EventHandler(this, &Form1::clearRectangleButton_Click);
			// 
			// drawRectangleButton
			// 
			this->drawRectangleButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			this->drawRectangleButton->Location = System::Drawing::Point(12, 438);
			this->drawRectangleButton->Name = L"drawRectangleButton";
			this->drawRectangleButton->Size = System::Drawing::Size(85, 61);
			this->drawRectangleButton->TabIndex = 31;
			this->drawRectangleButton->Text = L"Draw Rectangle";
			this->drawRectangleButton->UseVisualStyleBackColor = true;
			this->drawRectangleButton->Click += gcnew System::EventHandler(this, &Form1::drawRectangleButton_Click);
			// 
			// button7
			// 
			this->button7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			this->button7->Location = System::Drawing::Point(99, 577);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(80, 62);
			this->button7->TabIndex = 30;
			this->button7->Text = L"Invert Y";
			this->button7->UseVisualStyleBackColor = true;
			this->button7->Click += gcnew System::EventHandler(this, &Form1::button7_Click_1);
			// 
			// button8
			// 
			this->button8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			this->button8->Location = System::Drawing::Point(12, 577);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(81, 62);
			this->button8->TabIndex = 29;
			this->button8->Text = L"Invert X";
			this->button8->UseVisualStyleBackColor = true;
			this->button8->Click += gcnew System::EventHandler(this, &Form1::button8_Click_1);
			// 
			// checkedListBox4
			// 
			this->checkedListBox4->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->checkedListBox4->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->checkedListBox4->CheckOnClick = true;
			this->checkedListBox4->ForeColor = System::Drawing::SystemColors::WindowText;
			this->checkedListBox4->FormattingEnabled = true;
			this->checkedListBox4->Location = System::Drawing::Point(133, 710);
			this->checkedListBox4->Name = L"checkedListBox4";
			this->checkedListBox4->Size = System::Drawing::Size(59, 2);
			this->checkedListBox4->TabIndex = 28;
			this->checkedListBox4->ThreeDCheckBoxes = true;
			this->checkedListBox4->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::checkedListBox4_SelectedIndexChanged);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			this->label9->Location = System::Drawing::Point(12, 709);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(89, 16);
			this->label9->TabIndex = 27;
			this->label9->Text = L"Hide Classes";
			this->label9->Click += gcnew System::EventHandler(this, &Form1::label9_Click);
			// 
			// checkBox1
			// 
			this->checkBox1->CheckAlign = System::Drawing::ContentAlignment::TopLeft;
			this->checkBox1->Location = System::Drawing::Point(16, 732);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(173, 31);
			this->checkBox1->TabIndex = 14;
			this->checkBox1->Text = L"Enable Data Display Based on Selected Area";
			this->checkBox1->TextAlign = System::Drawing::ContentAlignment::TopLeft;
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox1_CheckedChanged_1);
			// 
			// button6
			// 
			this->button6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			this->button6->Location = System::Drawing::Point(12, 271);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(173, 63);
			this->button6->TabIndex = 26;
			this->button6->Text = L"Show Testing Data";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &Form1::button6_Click_1);
			// 
			// button2
			// 
			this->button2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			this->button2->Location = System::Drawing::Point(12, 182);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(173, 66);
			this->button2->TabIndex = 25;
			this->button2->Text = L"Show Training Data";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click_1);
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			this->button1->Location = System::Drawing::Point(12, 97);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(173, 64);
			this->button1->TabIndex = 24;
			this->button1->Text = L"Upload Parser";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click_1);
			// 
			// button4
			// 
			this->button4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			this->button4->Location = System::Drawing::Point(12, 12);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(173, 63);
			this->button4->TabIndex = 9;
			this->button4->Text = L"Upload Data";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
			// 
			// button3
			// 
			this->button3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			this->button3->Location = System::Drawing::Point(12, 357);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(173, 61);
			this->button3->TabIndex = 8;
			this->button3->Text = L"Move Graph";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// button5
			// 
			this->button5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			this->button5->Location = System::Drawing::Point(9, 682);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(191, 52);
			this->button5->TabIndex = 10;
			this->button5->Text = L"Show/Hide Lines";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::button5_Click);
			// 
			// trackBar1
			// 
			this->trackBar1->BackColor = System::Drawing::SystemColors::Control;
			this->trackBar1->LargeChange = 1;
			this->trackBar1->Location = System::Drawing::Point(4, 71);
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(185, 45);
			this->trackBar1->TabIndex = 9;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &Form1::trackBar1_Scroll);
			// 
			// ZoomingLabel
			// 
			this->ZoomingLabel->AutoSize = true;
			this->ZoomingLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ZoomingLabel->Location = System::Drawing::Point(7, 46);
			this->ZoomingLabel->Name = L"ZoomingLabel";
			this->ZoomingLabel->Size = System::Drawing::Size(43, 16);
			this->ZoomingLabel->TabIndex = 11;
			this->ZoomingLabel->Text = L"Zoom";
			// 
			// vScrollBar1
			// 
			this->vScrollBar1->Location = System::Drawing::Point(152, 18);
			this->vScrollBar1->Minimum = -100;
			this->vScrollBar1->Name = L"vScrollBar1";
			this->vScrollBar1->Size = System::Drawing::Size(42, 50);
			this->vScrollBar1->TabIndex = 7;
			this->vScrollBar1->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &Form1::vScrollBar1_Scroll);
			// 
			// hScrollBar1
			// 
			this->hScrollBar1->Location = System::Drawing::Point(102, 16);
			this->hScrollBar1->Minimum = -100;
			this->hScrollBar1->Name = L"hScrollBar1";
			this->hScrollBar1->Size = System::Drawing::Size(50, 40);
			this->hScrollBar1->TabIndex = 8;
			this->hScrollBar1->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &Form1::hScrollBar1_Scroll);
			// 
			// PannningLabel
			// 
			this->PannningLabel->AutoSize = true;
			this->PannningLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->PannningLabel->Location = System::Drawing::Point(6, 16);
			this->PannningLabel->Name = L"PannningLabel";
			this->PannningLabel->Size = System::Drawing::Size(57, 16);
			this->PannningLabel->TabIndex = 10;
			this->PannningLabel->Text = L"Panning";
			// 
			// menuStrip1
			// 
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(32, 32);
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1530, 24);
			this->menuStrip1->TabIndex = 5;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// statusStrip1
			// 
			this->statusStrip1->ImageScalingSize = System::Drawing::Size(32, 32);
			this->statusStrip1->Location = System::Drawing::Point(0, 778);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(1530, 22);
			this->statusStrip1->TabIndex = 6;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->trackBar1);
			this->groupBox1->Controls->Add(this->PannningLabel);
			this->groupBox1->Controls->Add(this->ZoomingLabel);
			this->groupBox1->Controls->Add(this->vScrollBar1);
			this->groupBox1->Controls->Add(this->hScrollBar1);
			this->groupBox1->Location = System::Drawing::Point(6, 88);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(200, 111);
			this->groupBox1->TabIndex = 7;
			this->groupBox1->TabStop = false;
			// 
			// colorButton
			// 
			this->colorButton->Location = System::Drawing::Point(4, 49);
			this->colorButton->Name = L"colorButton";
			this->colorButton->Size = System::Drawing::Size(192, 45);
			this->colorButton->TabIndex = 0;
			this->colorButton->Text = L"Select Color";
			this->colorButton->UseVisualStyleBackColor = true;
			this->colorButton->Click += gcnew System::EventHandler(this, &Form1::colorButton_Click);
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Single;
			this->tableLayoutPanel1->ColumnCount = 1;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50)));
			this->tableLayoutPanel1->Controls->Add(this->label1, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->colorButton, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->comboBox1, 0, 1);
			this->tableLayoutPanel1->Location = System::Drawing::Point(6, 572);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 4;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(200, 104);
			this->tableLayoutPanel1->TabIndex = 9;
			this->tableLayoutPanel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::tableLayoutPanel1_Paint);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(4, 1);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(127, 16);
			this->label1->TabIndex = 12;
			this->label1->Text = L"Change Class Color";
			// 
			// comboBox1
			// 
			this->comboBox1->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->comboBox1->Cursor = System::Windows::Forms::Cursors::Hand;
			this->comboBox1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(4, 21);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(188, 21);
			this->comboBox1->TabIndex = 1;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox1_SelectedIndexChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(554, 8);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(0, 13);
			this->label2->TabIndex = 11;
			// 
			// panel2
			// 
			this->panel2->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->panel2->Controls->Add(this->lineColorCheckbox);
			this->panel2->Controls->Add(this->pointColorMode);
			this->panel2->Controls->Add(this->highlightMisclassificationsCheckbox);
			this->panel2->Controls->Add(this->lineTerminationModeCheckbox);
			this->panel2->Controls->Add(this->groupBox4);
			this->panel2->Controls->Add(this->groupBox3);
			this->panel2->Controls->Add(this->groupBox2);
			this->panel2->Controls->Add(this->tableLayoutPanel1);
			this->panel2->Controls->Add(this->groupBox1);
			this->panel2->Controls->Add(this->button5);
			this->panel2->Controls->Add(this->graph4);
			this->panel2->Location = System::Drawing::Point(1320, 27);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(209, 748);
			this->panel2->TabIndex = 13;
			// 
			// lineColorCheckbox
			// 
			this->lineColorCheckbox->AutoSize = true;
			this->lineColorCheckbox->Location = System::Drawing::Point(6, 367);
			this->lineColorCheckbox->Name = L"lineColorCheckbox";
			this->lineColorCheckbox->Size = System::Drawing::Size(103, 17);
			this->lineColorCheckbox->TabIndex = 20;
			this->lineColorCheckbox->Text = L"Line Color Mode";
			this->lineColorCheckbox->UseVisualStyleBackColor = true;
			this->lineColorCheckbox->CheckedChanged += gcnew System::EventHandler(this, &Form1::lineColorCheckbox_CheckedChanged);
			// 
			// pointColorMode
			// 
			this->pointColorMode->AutoSize = true;
			this->pointColorMode->Location = System::Drawing::Point(3, 325);
			this->pointColorMode->Name = L"pointColorMode";
			this->pointColorMode->Size = System::Drawing::Size(107, 17);
			this->pointColorMode->TabIndex = 19;
			this->pointColorMode->Text = L"Point Color Mode";
			this->pointColorMode->UseVisualStyleBackColor = true;
			this->pointColorMode->CheckedChanged += gcnew System::EventHandler(this, &Form1::pointColorMode_CheckedChanged);
			// 
			// highlightMisclassificationsCheckbox
			// 
			this->highlightMisclassificationsCheckbox->AutoSize = true;
			this->highlightMisclassificationsCheckbox->Location = System::Drawing::Point(3, 344);
			this->highlightMisclassificationsCheckbox->Name = L"highlightMisclassificationsCheckbox";
			this->highlightMisclassificationsCheckbox->Size = System::Drawing::Size(151, 17);
			this->highlightMisclassificationsCheckbox->TabIndex = 18;
			this->highlightMisclassificationsCheckbox->Text = L"Highlight Misclassifications";
			this->highlightMisclassificationsCheckbox->UseVisualStyleBackColor = true;
			this->highlightMisclassificationsCheckbox->CheckedChanged += gcnew System::EventHandler(this, &Form1::highlightMisclassificationsCheckbox_CheckedChanged);
			// 
			// lineTerminationModeCheckbox
			// 
			this->lineTerminationModeCheckbox->AutoSize = true;
			this->lineTerminationModeCheckbox->Location = System::Drawing::Point(3, 307);
			this->lineTerminationModeCheckbox->Name = L"lineTerminationModeCheckbox";
			this->lineTerminationModeCheckbox->Size = System::Drawing::Size(134, 17);
			this->lineTerminationModeCheckbox->TabIndex = 17;
			this->lineTerminationModeCheckbox->Text = L"Line Termination Mode";
			this->lineTerminationModeCheckbox->UseVisualStyleBackColor = true;
			this->lineTerminationModeCheckbox->CheckedChanged += gcnew System::EventHandler(this, &Form1::lineTerminationModeCheckbox_CheckedChanged);
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->backgroundColorLightnessSlider);
			this->groupBox4->Controls->Add(this->backgroundLightness);
			this->groupBox4->Location = System::Drawing::Point(9, 486);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(200, 84);
			this->groupBox4->TabIndex = 16;
			this->groupBox4->TabStop = false;
			// 
			// backgroundColorLightnessSlider
			// 
			this->backgroundColorLightnessSlider->BackColor = System::Drawing::SystemColors::Control;
			this->backgroundColorLightnessSlider->LargeChange = 1;
			this->backgroundColorLightnessSlider->Location = System::Drawing::Point(3, 43);
			this->backgroundColorLightnessSlider->Maximum = 200;
			this->backgroundColorLightnessSlider->Name = L"backgroundColorLightnessSlider";
			this->backgroundColorLightnessSlider->Size = System::Drawing::Size(185, 45);
			this->backgroundColorLightnessSlider->TabIndex = 10;
			this->backgroundColorLightnessSlider->Value = 2;
			this->backgroundColorLightnessSlider->Scroll += gcnew System::EventHandler(this, &Form1::trackBar2_Scroll_1);
			// 
			// backgroundLightness
			// 
			this->backgroundLightness->AutoSize = true;
			this->backgroundLightness->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.5F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->backgroundLightness->Location = System::Drawing::Point(4, 16);
			this->backgroundLightness->Name = L"backgroundLightness";
			this->backgroundLightness->Size = System::Drawing::Size(161, 15);
			this->backgroundLightness->TabIndex = 11;
			this->backgroundLightness->Text = L"Background Color Lightness";
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->backgroundTransparencySlider);
			this->groupBox3->Controls->Add(this->backgroundTransparencyLabel);
			this->groupBox3->Location = System::Drawing::Point(6, 411);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(200, 74);
			this->groupBox3->TabIndex = 15;
			this->groupBox3->TabStop = false;
			// 
			// backgroundTransparencySlider
			// 
			this->backgroundTransparencySlider->BackColor = System::Drawing::SystemColors::Control;
			this->backgroundTransparencySlider->LargeChange = 1;
			this->backgroundTransparencySlider->Location = System::Drawing::Point(3, 43);
			this->backgroundTransparencySlider->Maximum = 255;
			this->backgroundTransparencySlider->Name = L"backgroundTransparencySlider";
			this->backgroundTransparencySlider->Size = System::Drawing::Size(185, 45);
			this->backgroundTransparencySlider->TabIndex = 10;
			this->backgroundTransparencySlider->Value = 100;
			this->backgroundTransparencySlider->Scroll += gcnew System::EventHandler(this, &Form1::backgroundTransparencySlider_Scroll);
			// 
			// backgroundTransparencyLabel
			// 
			this->backgroundTransparencyLabel->AutoSize = true;
			this->backgroundTransparencyLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.5F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->backgroundTransparencyLabel->Location = System::Drawing::Point(4, 16);
			this->backgroundTransparencyLabel->Name = L"backgroundTransparencyLabel";
			this->backgroundTransparencyLabel->Size = System::Drawing::Size(150, 15);
			this->backgroundTransparencyLabel->TabIndex = 11;
			this->backgroundTransparencyLabel->Text = L"Background Transparency";
			this->backgroundTransparencyLabel->Click += gcnew System::EventHandler(this, &Form1::backgroundTransparencyLabel_Click);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->classTransparencySelection);
			this->groupBox2->Controls->Add(this->transparencySlider);
			this->groupBox2->Controls->Add(this->transparencyLabel);
			this->groupBox2->Location = System::Drawing::Point(6, 205);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(200, 102);
			this->groupBox2->TabIndex = 14;
			this->groupBox2->TabStop = false;
			// 
			// classTransparencySelection
			// 
			this->classTransparencySelection->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->classTransparencySelection->Cursor = System::Windows::Forms::Cursors::Hand;
			this->classTransparencySelection->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->classTransparencySelection->FormattingEnabled = true;
			this->classTransparencySelection->Location = System::Drawing::Point(6, 37);
			this->classTransparencySelection->Name = L"classTransparencySelection";
			this->classTransparencySelection->Size = System::Drawing::Size(188, 21);
			this->classTransparencySelection->TabIndex = 13;
			this->classTransparencySelection->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::classTransparencySelection_SelectedIndexChanged);
			// 
			// transparencySlider
			// 
			this->transparencySlider->BackColor = System::Drawing::SystemColors::Control;
			this->transparencySlider->LargeChange = 1;
			this->transparencySlider->Location = System::Drawing::Point(6, 64);
			this->transparencySlider->Maximum = 255;
			this->transparencySlider->Name = L"transparencySlider";
			this->transparencySlider->Size = System::Drawing::Size(185, 45);
			this->transparencySlider->TabIndex = 10;
			this->transparencySlider->Value = 255;
			this->transparencySlider->Scroll += gcnew System::EventHandler(this, &Form1::transparencySlider_Scroll);
			// 
			// transparencyLabel
			// 
			this->transparencyLabel->AutoSize = true;
			this->transparencyLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->transparencyLabel->Location = System::Drawing::Point(6, 18);
			this->transparencyLabel->Name = L"transparencyLabel";
			this->transparencyLabel->Size = System::Drawing::Size(129, 16);
			this->transparencyLabel->TabIndex = 11;
			this->transparencyLabel->Text = L"Class Transparency";
			// 
			// button9
			// 
			this->button9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			this->button9->Location = System::Drawing::Point(12, 515);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(173, 41);
			this->button9->TabIndex = 34;
			this->button9->Text = L"Draw Rects On Gray";
			this->button9->UseVisualStyleBackColor = true;
			this->button9->Click += gcnew System::EventHandler(this, &Form1::button9_Click_1);
			// 
			// Form1
			// 
			this->BackColor = System::Drawing::SystemColors::Control;
			this->ClientSize = System::Drawing::Size(1530, 800);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Margin = System::Windows::Forms::Padding(12);
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Interactive Shifted Paired Coordinates";
			this->TransparencyKey = System::Drawing::Color::Maroon;
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->backgroundColorLightnessSlider))->EndInit();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->backgroundTransparencySlider))->EndInit();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->transparencySlider))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Converts a str to a standard string. </summary>
		///
		/// <param name="str">	The string. </param>
		///
		/// <returns>	Str as a std::string. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		std::string toStandardString(System::String^ str)
		{
			const char* location = (const char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str).ToPointer();
			std::string temp(location);
			/* Clean up the const char* location memory */
			System::Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr((void*)location));
			return temp;
		}

		// File Opening

		System::Void openFile(System::Void)
		{
			OpenFileDialog^ ofd = gcnew OpenFileDialog;
			ofd->InitialDirectory = ""; // NOTE "c:\\" for future reference.
			ofd->Filter = "Text Files (*.csv, *.txt, *.text)|*.csv; *.txt; *.text";
			ofd->FilterIndex = 1;
			ofd->RestoreDirectory = true;

			if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				/* Convert System::String^ to a std::string */
				System::String^ path = ofd->FileName;

				data.fileName = toStandardString(path);
				

			

				graphType = -1;
				oldZoomValue = 0;
				graph1_oldZoom = 0;
				graph2_oldZoom = 0;
				graph3_oldZoom = 0;
				graph4_oldZoom = 0;
				graph5_oldZoom = 0;
				graph6_oldZoom = 0;
				graph7_oldZoom = 0;
				graph8_oldZoom = 0;
				graph9_oldZoom = 0;

				panel1showing = true;
				panel2showing = true;
				OpenGL3->resetDrawingArea();

				Controls->Clear();
				InitializeComponent();

				graphType = 1;
				OpenGL3->graphType = 1;
				trackBar1->Value = graph1_oldZoom;
				classnumberselectedForColor = -1;

				fileopened = true;
		
				

				
			}
		}
     



		System::Void openParserFile(System::Void)
		{
			OpenFileDialog^ ofpd = gcnew OpenFileDialog;
			ofpd->InitialDirectory = ""; // NOTE "c:\\" for future reference.
			ofpd->Filter = "Text Files (*.csv, *.txt, *.text)|*.csv; *.txt; *.text";
			ofpd->FilterIndex = 1;
			ofpd->RestoreDirectory = true;

			if (ofpd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				/* Convert System::String^ to a std::string */
				System::String^ path = ofpd->FileName;

				//data.fileName = toStandardString(path);
				dataParsed.parserFileName = toStandardString(path);
				parsefileOpened = true;




			}
		}




	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		UNREFERENCED_PARAMETER(sender);
		UNREFERENCED_PARAMETER(e);


		if (graphType == 4)			//Combined SPC graph4
		{
			OpenGL3->Render4();
		}



		OpenGL3->SwapOpenGLBuffers();
	}

			 // BUTTONS TO CHOOSE DISPLAY OF GRAPH TYPE





	private: System::Void graph4_Click(System::Object^  sender, System::EventArgs^  e) {	//SPC graph4
		

		// resets the slider positions to the default values
		// Eventually this should not be hard coded and the actual defaults should be stored globally somewhere
		float defaultDataTransparency = 255;
		float defaultBackgroundTransparency = 100;
		float defaultBackgroundColorLightness = 1.0;
		
		backgroundTransparencySlider->Value = defaultBackgroundTransparency;
		transparencySlider->Value = defaultDataTransparency;
		backgroundColorLightnessSlider->Value = defaultBackgroundColorLightness;
		OpenGL3->setBackgroundTransparency(defaultBackgroundTransparency);
		OpenGL3->setDataTransparency(defaultDataTransparency, -1);
		OpenGL3->setBackgroundColorLightness(defaultBackgroundColorLightness);
		OpenGL3->setColorTerminationMode(false);
		OpenGL3->setHighlightMisclassficationsMode(false);
		OpenGL3->setLineTerminationMode(false);
		

		for (int x = 0; x < checkedListBox4->Items->Count; x++)
		{
			checkedListBox4->SetItemChecked(x, false);
		}
		
		if (fileopened)
		{
			OpenGL3->clearAllGraphData();
			OpenGL3->setFileName(data);
			OpenGL3->setParserFileName(dataParsed);
			checkBox1->Checked = false;
		}
		//if (parsefileOpened)
		//{
		//	
		//	dataParsed. data.parsedData;
		//}

		OpenGL3->seedDataTerminationPoints();

		loadInteractiveSPC();
	}
	protected:
		/// <summary>	The graph 1 old zoom. </summary>
		/// <summary>	The graph 1 old zoom. </summary>
		int graph1_oldZoom;
		/// <summary>	The graph 2 old zoom. </summary>
		int graph2_oldZoom;
		/// <summary>	The graph 3 old zoom. </summary>
		int graph3_oldZoom;
		/// <summary>	The graph 4 old zoom. </summary>
		int graph4_oldZoom;
		/// <summary>	The graph 5 old zoom. </summary>
		int graph5_oldZoom;
		/// <summary>	The graph 6 old zoom. </summary>
		int graph6_oldZoom;
		/// <summary>	The graph 7 old zoom. </summary>
		int graph7_oldZoom;
		/// <summary>	The graph 8 old zoom. </summary>
		int graph8_oldZoom;
		/// <summary>	The graph 9 old zoom. </summary>
		int graph9_oldZoom;


	private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		UNREFERENCED_PARAMETER(sender);
		UNREFERENCED_PARAMETER(e);

		openFile(); // starts select file window
		if (fileopened) {
			OpenGL3->setFileName(data);


			// clear the list to prevent duplicates on update
			this->comboBox1->Items->Clear();
			this->comboBox1->Items->Clear();
			data.numOfClasses = OpenGL3->getClassSize();
			for (int i = 1; i <= data.numOfClasses; i++) {
					this->comboBox1->Items->Add(i); // set the combobox for "Class combobox" to the data in item1 
			}
			if (parsefileOpened) {
				OpenGL3->setParserFileName(dataParsed);
			}



			loadInteractiveSPC();
		}


	}






	private: System::Void hScrollBar1_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e) {
		if (e->NewValue > e->OldValue)
		{
			//moved right
			OpenGL3->panHorizontal(true); // true for going right



		}
		else if (e->NewValue < e->OldValue)
		{
			//moved left
			OpenGL3->panHorizontal(false); // true for going right else false for going left

		}
	}


	private: System::Void vScrollBar1_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e) {
		if (e->NewValue > e->OldValue)
		{
			//moved up
			OpenGL3->panVertical(true); // true for going up else flase for down
		}
		else if (e->NewValue < e->OldValue)
		{
			//moved down
			OpenGL3->panVertical(false); // true for going up else flase for down
		}
	}


	private: System::Void trackBar1_Scroll(System::Object^  sender, System::EventArgs^  e) {
		//int value = trackBar1->Value;
		//MessageBox::Show("" + value);

		if (graphType == 1) {
			OpenGL3->zoom(trackBar1->Value - graph1_oldZoom);
			graph1_oldZoom = trackBar1->Value;
		}
		else if (graphType == 2) {
			OpenGL3->zoom(trackBar1->Value - graph2_oldZoom);
			graph2_oldZoom = trackBar1->Value;
		}
		else if (graphType == 3) {
			OpenGL3->zoom(trackBar1->Value - graph3_oldZoom);
			graph3_oldZoom = trackBar1->Value;
		}
		else if (graphType == 4) {

			OpenGL3->zoom(trackBar1->Value - graph4_oldZoom);
			graph4_oldZoom = trackBar1->Value;
			
		}
		else if (graphType == 5) {
			OpenGL3->zoom(trackBar1->Value - graph5_oldZoom);
			graph5_oldZoom = trackBar1->Value;
		}
		else if (graphType == 6) {
			OpenGL3->zoom(trackBar1->Value - graph6_oldZoom);
			graph6_oldZoom = trackBar1->Value;
		}
		else if (graphType == 7) {
			OpenGL3->zoom(trackBar1->Value - graph7_oldZoom);
			graph7_oldZoom = trackBar1->Value;
		}
		else if (graphType == 8) {
			OpenGL3->zoom(trackBar1->Value - graph8_oldZoom);
			graph8_oldZoom = trackBar1->Value;
		}
		else if (graphType == 9) {
			OpenGL3->zoom(trackBar1->Value - graph9_oldZoom);
			graph9_oldZoom = trackBar1->Value;
		}
	}



	int classnumberselectedForColor = -1;
private: System::Void colorButton_Click(System::Object^  sender, System::EventArgs^  e) {
	// FOR CHANGING COLOR
	if (colorDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		//this->colorPanel->BackColor = this->colorDialog1->Color;
		float red = this->colorDialog1->Color.R;
		float green = this->colorDialog1->Color.G;
		float blue = this->colorDialog1->Color.B;

		OpenGL3->setClassColor(red, green, blue, classnumberselectedForColor);
	}

	
}

private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	classnumberselectedForColor = comboBox1->SelectedIndex+1;

}

		 bool panel1showing = true;

		 bool panel2showing = true;



		 int loadInteractiveSPC();

private: System::Void panel1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
}
private: System::Void comboBox2_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	if (fileopened) {
		OpenGL3->setFileName(data);
	}

}

private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {

}
private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e) {
	UNREFERENCED_PARAMETER(sender);
	UNREFERENCED_PARAMETER(e);
	
	openFile(); // starts select file window

	if (fileopened) {
		OpenGL3->setFileName(data);
		//OpenGL3->setParserFileName(dataParsed);
		OpenGL3->seedDataTerminationPoints();

		// clear the list to prevent duplicates on update
		this->comboBox1->Items->Clear();
		this->comboBox1->Items->Clear();
		this->classTransparencySelection->Items->Clear();
		this->classTransparencySelection->Items->Clear();
		
		this->classTransparencySelection->Items->Add("All");

		data.numOfClasses = OpenGL3->getClassSize();
		data.continueElements = OpenGL3->getContinueClassList();
		for (int i = 1; i <= data.numOfClasses; i++) {
			this->comboBox1->Items->Add(i); // set the combobox for "Class combobox" to the data in item1 
			this->classTransparencySelection->Items->Add(i);
		}

		for (int i = 0; i < data.continueElements.size(); i++) {
			this->comboBox1->Items->Add(data.continueElements[i]);
			this->classTransparencySelection->Items->Add(data.continueElements[i]);
		}

		
		data.numOfClasses = OpenGL3->getClassSize();

		
		data.numOfClasses = OpenGL3->getClassSize();

		loadInteractiveSPC();

		
		// Note: not sure why we do this twice in this method

		data.numOfClasses = OpenGL3->getClassSize();

		for (int i = 0; i < data.numOfClasses; i++) {
			data.classTransparencies.push_back(255); // Make all classes fully opaque to begin
		}

		this->checkedListBox4->Items->Clear();
		this->checkedListBox4->Items->Clear();

		this->classTransparencySelection->Items->Clear();
		this->classTransparencySelection->Items->Clear();

		this->classTransparencySelection->Items->Add("All");

		data.numOfClasses = OpenGL3->getClassSize();
		for (int i = 1; i <= data.numOfClasses; i++) {
			this->checkedListBox4->Items->Add(i); // set the combobox for "Class combobox" to the data in item1 
			this->classTransparencySelection->Items->Add(i);
		}
	}
}


private: System::Void trackBar2_Scroll(System::Object^ sender, System::EventArgs^ e) 
{
	


}

	   private: System::Void comboBox3_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) 
	   {
		

	   }
public:
	//void log(System::String^ logText);
private: System::Void trackBar3_Scroll(System::Object^ sender, System::EventArgs^ e)
{
	
}
private: System::Void button5_Click(System::Object^ sender, System::EventArgs^ e)
{
	OpenGL3->showHideLines(); 
}
private: System::Void label6_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void button6_Click(System::Object^ sender, System::EventArgs^ e) 
{
	
}
private: System::Void button7_Click(System::Object^ sender, System::EventArgs^ e) 
{
	
}
private: System::Void label4_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void checkBox1_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
{
	
}
private: System::Void checkBox2_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
{
	
}
private: System::Void button8_Click(System::Object^ sender, System::EventArgs^ e) 
{

}
private: System::Void comboBox4_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) 
{
	
}
private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) 
{
	/*if (OpenGL3->nonOrtho == 1)
	{
		if (textBox1->Text == "" || textBox1->Text == "-")
		{
			OpenGL3->nonOrthoXAngle = previousNonOrthoXValue;
		}

		else
			OpenGL3->nonOrthoXAngle = (float)Convert::ToDouble(textBox1->Text);

		OpenGL3->nonOrthoXDiff = OpenGL3->nonOrthoXAngle - previousNonOrthoXValue;
		previousNonOrthoXValue = OpenGL3->nonOrthoXAngle;

	}*/

	
}


private: System::Void textBox3_TextChanged(System::Object^ sender, System::EventArgs^ e) 
{
	
}
private: System::Void textBox4_TextChanged(System::Object^ sender, System::EventArgs^ e) 
{	
}
private: System::Void label7_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void comboBox5_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
}





private: System::Void button9_Click(System::Object^ sender, System::EventArgs^ e) 
{
	OpenGL3->drawingAndRectEnabled = true;
	OpenGL3->drawingOrRectEnabled = false;
	//OpenGL3->clipAndData();
}
private: System::Void button10_Click(System::Object^ sender, System::EventArgs^ e) 
{

	OpenGL3->drawingOrRectEnabled = true;
	OpenGL3->drawingAndRectEnabled = false;
	
}
private: System::Void checkBox3_CheckedChanged(System::Object^ sender, System::EventArgs^ e) 
{

	}
	
private: System::Void button11_Click(System::Object^ sender, System::EventArgs^ e) 
{

}
private: System::Void button12_Click(System::Object^ sender, System::EventArgs^ e) 
{
	
}
private: System::Void button1_Click_1(System::Object^ sender, System::EventArgs^ e)
{
	//openParseFile();
	OpenFileDialog^ ofp = gcnew OpenFileDialog;
	if (ofp->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		ofp->InitialDirectory = ""; // NOTE "c:\\" for future reference.
		ofp->Filter = "Text Files (*.csv, *.txt, *.text)|*.csv; *.txt; *.text";
		ofp->FilterIndex = 1;
		ofp->RestoreDirectory = true;

		System::String^ path1 = ofp->FileName;

		dataParsed.parserFileName = toStandardString(path1);


		parsefileOpened = true;
	}
	OpenGL3->setParserFileName(dataParsed);
	data.parsedData = dataParsed.parsedData;
	data.parsedAttributePairs = dataParsed.parsedAttributePairs;
	data.strparsedData = dataParsed.strparsedData;

	//OpenGL3->parserFileOpen();
	//openParserFile(); // starts select file window

	//if (parsefileOpened) {
	//	OpenGL3->setParserFileName(dataParsed);
	//}
	

}
private: System::Void label8_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void button2_Click_1(System::Object^ sender, System::EventArgs^ e) 
{
	OpenGL3->showTrainigData();
}
private: System::Void button6_Click_1(System::Object^ sender, System::EventArgs^ e)
{
	OpenGL3->showTestingData();
}
private: System::Void checkBox1_CheckedChanged_1(System::Object^ sender, System::EventArgs^ e)
{
	OpenGL3->reverseDataAxis = 0;
	if (checkBox1->Checked)
	{
		OpenGL3->displayData = true;
		//OpenGL3->displaySelectedData();
	}
	else
	{
		OpenGL3->displayData = false;
	}
}
	
private: System::Void checkedListBox4_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
{
	if (checkedListBox4->CheckedItems->Count != 0)
	{
		for (int x = 0; x < checkedListBox4->CheckedItems->Count; x++)
		{
			if (checkedListBox4->CheckedIndices[x] == checkedListBox4->SelectedIndex)
			{
				OpenGL3->showHideClass(checkedListBox4->SelectedIndex, 0);
				break;
			}
			else
				OpenGL3->showHideClass(checkedListBox4->SelectedIndex, 255);
		}

	}

	else
		OpenGL3->showHideClass(checkedListBox4->SelectedIndex, 255);

}
private: System::Void label9_Click(System::Object^ sender, System::EventArgs^ e) {
}
	   bool swapXToggle = false;
private: System::Void button8_Click_1(System::Object^ sender, System::EventArgs^ e) 
{
	checkBox1->Checked = false;
	//OpenGL3->reverseDataAxis = 1;
	OpenGL3->isXAxisInvertMode = !OpenGL3->isXAxisInvertMode;
	if (!swapXToggle) {
		button8->Text = button8->Text + " (BUTTON ACTIVE) ";
	}
	else {
		button8->Text = button8->Text->Substring(0, button8->Text->Length - 17);
	}
	swapXToggle = !swapXToggle;
}
	   bool swapYToggle = false;
private: System::Void button7_Click_1(System::Object^ sender, System::EventArgs^ e) 
{
	checkBox1->Checked = false;
	//OpenGL3->reverseDataAxis = 2;
	OpenGL3->isYAxisInvertMode = !OpenGL3->isYAxisInvertMode;
	if (!swapYToggle) {
		button7->Text = button7->Text + " (BUTTON ACTIVE) ";
	}
	else {
		button7->Text = button7->Text->Substring(0, button7->Text->Length - 17);
	}
	swapYToggle = !swapYToggle;

}

private: System::Void tableLayoutPanel1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
}
private: System::Void backgroundTransparencyLabel_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void backgroundTransparencySlider_Scroll(System::Object^ sender, System::EventArgs^ e) {
	OpenGL3->setBackgroundTransparency((float)backgroundTransparencySlider->Value);
}
private: System::Void trackBar2_Scroll_1(System::Object^ sender, System::EventArgs^ e) {
	OpenGL3->setBackgroundColorLightness(backgroundColorLightnessSlider->Value / 100.0);
}
	   bool rectButtonToggled = false;
private: System::Void drawRectangleButton_Click(System::Object^ sender, System::EventArgs^ e) {
	rectButtonToggled = !rectButtonToggled;
	if (rectButtonToggled) {
		drawRectangleButton->Text = drawRectangleButton->Text + " (BUTTON ACTIVE) ";
	}
	else {
		drawRectangleButton->Text = drawRectangleButton->Text->Substring(0, drawRectangleButton->Text->Length - 17);
	}
	if (rectButtonToggled) {
		String^ str = "To draw a rectangle, click once for the first corner, then click again for the opposite corner.";
		MessageBox::Show(str);
		OpenGL3->drawingRectangleEnabled = true;
	}
	else {
		OpenGL3->drawingRectangleEnabled = false;
	}
}
private: System::Void clearRectangleButton_Click(System::Object^ sender, System::EventArgs^ e) {
	// Clear rectangle
	OpenGL3->drawingRectangleEnabled = false;
	OpenGL3->setDrawingRectangleState(false);
	OpenGL3->seedDataTerminationPoints();
}

	int classSelectionForTransparency = -1;
private: System::Void classTransparencySelection_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	classSelectionForTransparency = classTransparencySelection->SelectedIndex - 1;
}
private: System::Void transparencySlider_Scroll(System::Object^ sender, System::EventArgs^ e) {
	OpenGL3->setDataTransparency((float)transparencySlider->Value, classSelectionForTransparency);
}
private: System::Void lineTerminationModeCheckbox_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
	OpenGL3->setLineTerminationMode(lineTerminationModeCheckbox->Checked);
	OpenGL3->setColorTerminationMode(lineTerminationModeCheckbox->Checked);
}
private: System::Void highlightMisclassificationsCheckbox_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
	OpenGL3->setHighlightMisclassficationsMode(highlightMisclassificationsCheckbox->Checked);
}
private: System::Void pointColorMode_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
	OpenGL3->setPointColorMode(pointColorMode->Checked);
}
	   bool swapButtonToggled = false;
private: System::Void SwapAttributesButton_Click(System::Object^ sender, System::EventArgs^ e) {
	OpenGL3->toggleSwapAttributeAxesMode();
	if (!swapButtonToggled) {
		SwapAttributesButton->Text = SwapAttributesButton->Text + " (BUTTON ACTIVE) ";
	}
	else {
		SwapAttributesButton->Text = SwapAttributesButton->Text->Substring(0, SwapAttributesButton->Text->Length - 17);
	}
	swapButtonToggled = !swapButtonToggled;
}
private: System::Void lineColorCheckbox_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
	OpenGL3->setLineColorMode(lineColorCheckbox->Checked);
}
private: System::Void button9_Click_1(System::Object^ sender, System::EventArgs^ e) {
	OpenGL3->drawRectanglesOnGray();
}
};
}



int OpenGL_on_a_Windows_Form::Form1::loadInteractiveSPC()
{
	if (fileopened) {
		graphType = 4;
		OpenGL3->graphType = 4;
		trackBar1->Value = graph4_oldZoom;
	}
	return 0;
} 


//void OpenGL_on_a_Windows_Form::Form1::log(System::String^ logText)
//{
//	this->LogTextBox->Text += logText + "\n";
//}
