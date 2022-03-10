///-------------------------------------------------------------------------------------------------
// file:	OpenGL3.h
//
// summary:	Declares the OpenGL class
///-------------------------------------------------------------------------------------------------

#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include "stdafx.h"
#include "ClassData.h"
#include "FileHandling.h"
#include "InteractiveSPC.h"
#include <sstream>



///-------------------------------------------------------------------------------------------------
// namespace: System
//
// summary:	.
///-------------------------------------------------------------------------------------------------

using namespace System::Windows::Forms;

///-------------------------------------------------------------------------------------------------
// namespace: OpenGLForm
//
// summary:	.
///-------------------------------------------------------------------------------------------------

namespace OpenGLForm
{
	/// <summary>	THE MAGIC. </summary>
	ClassData data;
	parseData dataParsed;
	/// <summary>	The new file. </summary>
	FileHandling newFile;



	/// <summary>	The fourth graph. </summary>
	InteractiveSPC graph4;



	public ref class COpenGL3 : public System::Windows::Forms::NativeWindow
	{
	public:

		/// <summary>	Height of the world. </summary>
		double worldHeight;
		/// <summary>	Width of the world. </summary>
		double worldWidth;
		/// <summary>	Holds the clicked dimension. </summary>
		int clickedDimension;
		//double normx1, normx2, normy1, normy2;
		double rectx1, rectx2, recty1, recty2;
		double rectOrx1, rectOrx2, rectOry1, rectOry2;
		//int graphNumber;		
		//std::vector<double> x1CoordGraph, x2CoordGraph, y1CoordGraph, y2CoordGraph;
		/// <summary>	Holds mouse X coord when mapped to world. </summary>
		double worldMouseX;
		/// <summary>	Holds mouse Y coord when mapped to world. </summary>
		double worldMouseY;

		/// <summary>	Is made true via mouselistener when dragging the mouse. </summary>
		bool drawingDragged;
		bool drawingRectangleEnabled = false;
		bool drawingRectangleVertex1 = true;
		GLfloat drawingRectangleX1;
		GLfloat drawingRectangleY1;
		GLfloat drawingRectangleX2;
		GLfloat drawingRectangleY2;
		bool mouseButtonDown = false;
		bool drawingAndRectEnabled;
		bool drawingOrRectEnabled;
		bool drawingOrRectComplete;
		/// <summary>	holds which graph to display, -1 for none right now. </summary>
		int graphType = -1;
		bool displayData; 
		
		bool attributeSwapMode = false;
		bool isXAxisInvertMode = false;
		bool isYAxisInvertMode = false;

		/// <summary>	The original wh. </summary>
		int originalWH;
		/// <summary>	The original ww. </summary>
		int originalWW;
		int cpPosx;
		int cpPosy;
		


		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// 
		///
		///
		///
		/// <param name="parentForm">	The parent form. </param>
		/// <param name="iWidth">	 	Zero-based index of the width. </param>
		/// <param name="iHeight">   	Zero-based index of the height. </param>
		/// <param name="igraphType">	Type of the igraph. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		COpenGL3(System::Windows::Forms::Form ^ parentForm, GLsizei iWidth, GLsizei iHeight, int igraphType)
		{
			graphType = igraphType;

			CreateParams^ cp = gcnew CreateParams;

			worldWidth = iWidth;
			worldHeight = iHeight;
			drawingDragged = false;
			clickedDimension = -1;

			originalWW = iWidth;
			originalWH = iHeight;
			
			reverseDataAxis = 0;

			// Set the position on the form
			cp->X = 201;
			cpPosx = 201;
			cp->Y = 33;
			cpPosy = 33;
			cp->Height = iHeight;
			cp->Width = iWidth;

			// Specify the form as the parent.
			cp->Parent = parentForm->Handle;
			
			// Create as a child of the specified parent and make OpenGL compliant (no clipping)
			cp->Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_BORDER | WS_TABSTOP;

			// Create the actual window
			this->CreateHandle(cp);

			m_hDC = GetDC((HWND)this->Handle.ToPointer());

			if (m_hDC)
			{
				MySetPixelFormat(m_hDC);
				ReSizeGLScene(iWidth, iHeight);
				//InitGL();
			}
		}


		void resetDrawingArea() {
			SetWindowPos((HWND)this->Handle.ToPointer(), NULL, 201, 33, originalWW, originalWH, SWP_SHOWWINDOW);
			SetFocus((HWND)this->Handle.ToPointer());
			cpPosx = 201;
		}

		// CHANGE BACKGROUND COLOR
		void setClassColor(float R, float G, float B, int classnumber) {
			if (classnumber >= 0 && classnumber <= data.numOfClasses) {
			
				graph4.data.setClassColor(R, G, B, classnumber);
			
			}

		}

		// CHANGE TRANSPARENCY OF PLOTTED DATA
		void setDataTransparency(float alpha, int classNum) {
			graph4.data.setClassTransparency(alpha, classNum);
		}

		// CHANGE TRANSPARENCT OF THE BACKGROUND GRAPH
		void setBackgroundTransparency(float alpha) {
			graph4.setBackgroundTransparency(alpha);
		}

		// CHANGE LIGHTNESS OF BACKGROUND GRAPH COLORS
		void setBackgroundColorLightness(float lightnessCoeff) {
			graph4.setBackgroundColorLightness(lightnessCoeff);
		}

		// Set line termination mode state
		void setLineTerminationMode(bool state) {
			graph4.isLineTerminationMode = state;
		}
		// Set highlight missclassification mode state
		void setHighlightMisclassficationsMode(bool state) {
			graph4.isHighlightMisclassficationsMode = state;
		}

		// Set point color mode state
		void setPointColorMode(bool state) {
			graph4.isPointColorMode = state;
		}

		// Set line color mode
		void setLineColorMode(bool state) {
			graph4.isLineColorMode = state;
		}

		// Set line termination color mode state
		void setColorTerminationMode(bool state) {
			graph4.isColorTerminationMode = state;
		}

		// Set draw rectangle state
		void setDrawingRectangleState(bool state) {
			graph4.isRectangleMode = state;
			if (!state) {
				graph4.rectX1List.clear();
				graph4.rectX2List.clear();
				graph4.rectY1List.clear();
				graph4.rectY2List.clear();
			}
		}

		void drawRectanglesOnGray() {
			graph4.drawRectanglesOnGray();
		}

		// toggle attribute flip mode
		void toggleSwapAttributeAxesMode() {
			attributeSwapMode = !attributeSwapMode;
			graph4.swapAttributeAxesMode = !graph4.swapAttributeAxesMode;
		}

		int getClassSize() {
			//int numberOfClasses = 0; 
			//for (int i = 1; i <= data.classNum.size(); i++)
			//{
			//	if (data.classNum[i] != data.classNum[i - 1])
			//		numberOfClasses = numberOfClasses + 1;
			//}
			return graph4.data.numOfClasses;
			//return numberOfClasses;
		}

		std::vector<int> getContinueClassList() {
			return graph4.data.continueElements;
		}



		/* RENDERING FOR DIFFERENT GRAPHS *//////////////////////////////////////////////////////////////////////////////////////



	
		System::Void Render4(System::Void)
		{
			graph4.display();
		}



		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Swap open gl buffers. </summary>
		///
		////////////////////////////////////////////////////////////////////////////////////////////////////

		System::Void SwapOpenGLBuffers(System::Void)
		{
			SwapBuffers(m_hDC);
		}


		void clearAllGraphData() 
		{
			data.~ClassData();
			dataParsed.~parseData();

			graph4.data.~ClassData();
			graph4.dataParsed.~parseData();


			data.resetSomeVars();
			
			graph4.data.resetSomeVars();

		}


		void setFileName(ClassData &file) {
			// clearAllGraphData(); // why do we clear here?


			/* Opens the file and process the data */
			//data.fileName = "input.csv";
			data.fileName = file.fileName;
			data.worldWidth = worldWidth;
			data.worldHeight = worldHeight;
			data.xmax = 0;
			data.ymax = 0;
			newFile.openFile(data);
			// newFile.openParserFile(dataParsed,data); // should already be open when we load parser
			// newFile.sortGraphNotBasedOnParser(data); // OLD
			newFile.sortGraphBasedOnParser(data);
			newFile.normalizeData(data);

			// C-SPC
			graph4.data = data;
			graph4.dataParsed = dataParsed;
		
			graph4.data.numPlots = int(data.xdata[0].size());

			for (int y = 0; y < graph4.data.numPlots; y++)
			{

				graph4.data.plotWidth.push_back(worldWidth / (graph4.data.numPlots + 1)); // Width size for each graph
				graph4.data.plotHeight.push_back(worldHeight / (2)); // Height size for each graph
				graph4.data.nonOrthoY2.push_back(1);
				graph4.data.nonOrthoX1.push_back(-1);
				graph4.data.nonOrthoY1.push_back(-1);
				graph4.data.nonOrthoX2.push_back(1);
				
			}
	
			graph4.data.computeDecisionTreeBranches();
			graph4.fillPlotLocations(); // Creates starting graph positions, and fills example data for now.
			graph4.data.setClassColors();
			graph4.data.setContinueClassColors();

			//for (int j = 0; j < 3; j++)
			//{
			//	graph4.drawData(200,300,300,400, 0, j);
			//}

			//graph4.data.setClassColor(255.0, 0.0, 0.0, 1);
		
			originalWH = worldHeight; // Saves orginals to use for increments like for zooming.
			originalWW = worldWidth;
		}


		void setParserFileName(parseData &pfile) {
			


			/* Opens the file and process the data */
			//data.fileName = "input.csv";
			dataParsed.parserFileName = pfile.parserFileName;
			newFile.openParserFile(dataParsed, data);
			// graph4.dataParsed.parsedData = dataParsed.parsedData;
			graph4.dataParsed = dataParsed;
			pfile = dataParsed;
			graph4.data.parsedData = dataParsed.parsedData;
		}

		void seedDataTerminationPoints() {
			graph4.data.computeDecisionTreeBranches();
			graph4.data.seedDataTerminationPoints();
		}


		void zoom(int scaler) {
			int width = originalWW;
			int height = originalWH;


	
			if (graphType == 4) { // C-SPC

				graph4.data.worldWidth -= width / 20 * scaler;
				graph4.data.worldHeight -= height / 20 * scaler;
				graph4.data.leftWidth += width / 20 * scaler;
				graph4.data.bottomHeight += height / 20 * scaler;

				graph4.data.panamount -= scaler * 2;
			}


		}

		void panHorizontal(bool right) {



			 if (graphType == 4) { // C-SPC
				if (right) {
					graph4.data.pan_x -= graph4.data.panamount;
				}
				else {
					graph4.data.pan_x += graph4.data.panamount;
				}
			}



		}


		void panVertical(bool up) {



			 if (graphType == 4) { // C-SPC
				if (up) {
					graph4.data.pan_y -= graph4.data.panamount;
				}
				else {
					graph4.data.pan_y += graph4.data.panamount;
				}
			}
	
		}


	private:
		/// <summary>	The device-context. </summary>
		/// <summary>	The device-context. </summary>
		HDC m_hDC;
		/// <summary>	The hglrc. </summary>
		HGLRC m_hglrc;


	protected:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Destructor. </summary>
		///
		////////////////////////////////////////////////////////////////////////////////////////////////////

		~COpenGL3(System::Void)
		{
			this->DestroyHandle();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	My set pixel format. </summary>
		///
		/// <param name="hdc">	The hdc. </param>
		///
		/// <returns>	A GLint. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GLint MySetPixelFormat(HDC hdc)
		{
			static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
			{
				sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
				1,											// Version Number
				PFD_DRAW_TO_WINDOW |						// Format Must Support Window
				PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
				PFD_DOUBLEBUFFER,							// Must Support Double Buffering
				PFD_TYPE_RGBA,								// Request An RGBA Format
				16,										// Select Our Color Depth
				0, 0, 0, 0, 0, 0,							// Color Bits Ignored
				0,											// No Alpha Buffer
				0,											// Shift Bit Ignored
				0,											// No Accumulation Buffer
				0, 0, 0, 0,									// Accumulation Bits Ignored
				16,											// 16Bit Z-Buffer (Depth Buffer)  
				0,											// No Stencil Buffer
				0,											// No Auxiliary Buffer
				PFD_MAIN_PLANE,								// Main Drawing Layer
				0,											// Reserved
				0, 0, 0										// Layer Masks Ignored
			};

			GLint  iPixelFormat;

			// get the device context's best, available pixel format match 
			if ((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
			{
				MessageBox::Show("ChoosePixelFormat Failed");
				return 0;
			}

			// make that match the device context's current pixel format 
			if (SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
			{
				MessageBox::Show("SetPixelFormat Failed");
				return 0;
			}

			if ((m_hglrc = wglCreateContext(m_hDC)) == NULL)
			{
				MessageBox::Show("wglCreateContext Failed");
				return 0;
			}

			if ((wglMakeCurrent(m_hDC, m_hglrc)) == NULL)
			{
				MessageBox::Show("wglMakeCurrent Failed");
				return 0;
			}


			return 1;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Initializes the gl. </summary>
		///
		/// <param name="parameter1">	The first parameter. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		bool InitGL(GLvoid)										// All setup for opengl goes here
		{

			return TRUE;										// Initialisation went ok
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Re size gl scene. </summary>
		///
		/// <param name="width"> 	The width. </param>
		/// <param name="height">	The height. </param>
		///
		/// <returns>	A GLvoid. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize and initialise the gl window
		{
			if (height == 0)										// Prevent A Divide By Zero By
			{
				height = 1;										// Making Height Equal One
			}
		}

		double getWorldMouseX() {

			// this will get the mouse position relative to the child window
			LPPOINT pts = new POINT;
			GetCursorPos(pts);

			::ScreenToClient((HWND)this->Handle.ToPointer(), pts); // gets the mouse coordinate relative to the OpenGL view

			double xWorld = pts->x;
			delete pts;
			return xWorld;
		} // Converts raw mouse X coordinates to world coordinates


		double getWorldMouseY() {

			// this will get the mouse position relative to the child window
			LPPOINT pts = new POINT;
			GetCursorPos(pts);

			::ScreenToClient((HWND)this->Handle.ToPointer(), pts); // Gets the mouse position relative to the OpenGL view

			double yWorld = pts->y;
			delete pts;
			return yWorld;
		} // Converts raw mouse Y coordinates to world coordinates



// Processing mouse button clicks

		virtual void WndProc(Message %msg) override
		{
			// get the X and Y coordinates of the mouse position
			this->worldMouseX = this->getWorldMouseX();
			this->worldMouseY = this->getWorldMouseY();
			// Get keys pressed and pergorm action
	
		
			switch (msg.Msg)
			{

			case WM_LBUTTONDOWN:
			{

				if (drawingRectangleEnabled) {

					if (drawingRectangleVertex1) {
						drawingRectangleX1 = worldMouseX;
						drawingRectangleY1 = worldMouseY;
					}
					else {
						drawingRectangleX2 = worldMouseX;
						drawingRectangleY2 = worldMouseY;
						drawingRectangleEnabled = !drawingRectangleEnabled;
						graph4.rectX1List.push_back((GLfloat)drawingRectangleX1);
						graph4.rectY1List.push_back((GLfloat)drawingRectangleY1);
						graph4.rectX2List.push_back((GLfloat)drawingRectangleX2);
						graph4.rectY2List.push_back((GLfloat)drawingRectangleY2);
						/*graph4.rectX1 = drawingRectangleX1;
						graph4.rectY1 = drawingRectangleY1;
						graph4.rectX2 = drawingRectangleX2;
						graph4.rectY2 = drawingRectangleY2;
						drawingRectangleEnabled = false;*/
						setDrawingRectangleState(true);
					}
					drawingRectangleVertex1 = !drawingRectangleVertex1;
					break;
				}
				
				if (graphType == 1) { // S-CPC
					//No graph dragging
				}


				else if (graphType == 4) { // SPC

					{
						graph4.plotNumClicked = graph4.findClickedGraph(worldMouseX, worldMouseY);
						int plotNumClicked = graph4.plotNumClicked;
						ClassData* dataPtr = &graph4.data;
						if (attributeSwapMode && plotNumClicked >= 0 && plotNumClicked < graph4.data.numPlots) {
							if (graph4.swappedPlots.find(plotNumClicked) != graph4.swappedPlots.end()) {
								graph4.swappedPlots.insert(plotNumClicked);
							}
							else {
								graph4.swappedPlots.erase(plotNumClicked);
							}
							
							std::string tmp = graph4.data.parsedAttributePairs[plotNumClicked][0];
							dataPtr->parsedAttributePairs[plotNumClicked][0] = graph4.data.parsedAttributePairs[plotNumClicked][1];
							dataPtr->parsedAttributePairs[plotNumClicked][1] = tmp;
							for (int i = 0; i < dataPtr->parsedData.size(); i++) {
								if (dataPtr->parsedData[i][4] == plotNumClicked) {
									float tmp = dataPtr->parsedData[i][0];
									dataPtr->parsedData[i][0] = dataPtr->parsedData[i][1];
									dataPtr->parsedData[i][1] = tmp;
									tmp = dataPtr->parsedData[i][2];
									dataPtr->parsedData[i][2] = dataPtr->parsedData[i][3];
									dataPtr->parsedData[i][3] = tmp;
								}
							}
							graph4.dataParsed.parsedData = dataPtr->parsedData;
						}

						if (isXAxisInvertMode) {
							std::set<int>* xAxisInvert = &graph4.plotsWithXAxisInverted;
							if (xAxisInvert->find(plotNumClicked) == xAxisInvert->end()) {
								xAxisInvert->insert(plotNumClicked);
							}
							else {
								xAxisInvert->erase(plotNumClicked);
							}
							xAxisInvert = &graph4.data.plotsWithXInverted;
							if (xAxisInvert->find(plotNumClicked) == xAxisInvert->end()) {
								xAxisInvert->insert(plotNumClicked);
							}
							else {
								xAxisInvert->erase(plotNumClicked);
							}
						}

						if (isYAxisInvertMode) {
							std::set<int>* yAxisInvert = &graph4.plotsWithYAxisInverted;
							if (yAxisInvert->find(plotNumClicked) == yAxisInvert->end()) {
								yAxisInvert->insert(plotNumClicked);
							}
							else {
								yAxisInvert->erase(plotNumClicked);
							}
							yAxisInvert = &graph4.data.plotsWithYInverted;
							if (yAxisInvert->find(plotNumClicked) == yAxisInvert->end()) {
								yAxisInvert->insert(plotNumClicked);
							}
							else {
								yAxisInvert->erase(plotNumClicked);
							}
						}

						bool colliding = (graph4.plotNumClicked != -1);
						if (colliding && displayData == false && reverseDataAxis == 0)
						{
							graph4.dragging = true;
							graph4.updatePlotLocation(worldMouseX, worldMouseY, graph4.plotNumClicked);
							/*graph4.data.xPlotCoordinates[graph4.graphClicked] = worldMouseX - graph4.data.pan_x;
							graph4.data.yPlotCoordinates[graph4.graphClicked] = worldMouseY - graph4.data.pan_y;*/
						}

						
						if (displayData)
						{
							displaySelectedData();
						}
					}
				}



			}
			break;
			case WM_RBUTTONDOWN:
			{

				if (graphType == 1) { // S-CPC
					// NOTHING YET
				}

				else if (graphType == 3) { // S-SPC
					// NOTHING YET
				}
				else if (graphType == 4) { // C-SPC
					// NOTHING YET
				}
			}
			break;
			case WM_MOUSEMOVE:
			{

				//if (drawingDragged)
				//{
				if (graphType == 1) { // S-CPC
					//graph1.data.yPlotCoordinates[lineClicked] = worldMouseY;
				}
				else if (graphType == 2) { // C-CPC
					// NOTHING YET
				}

				else if (graphType == 4) { // C-SPC
					// DRAGGING
					if (graph4.dragging) {
						graph4.updatePlotLocation(worldMouseX, worldMouseY, graph4.plotNumClicked);
						/*graph4.data.xPlotCoordinates[graph4.graphClicked] = worldMouseX - graph4.data.pan_x;
						graph4.data.yPlotCoordinates[graph4.graphClicked] = worldMouseY - graph4.data.pan_y;*/
					}
				}

				//if (graph4.data.drawRect && mouseButtonDown == true)
				//{
				//	graph4.data.temprx2 = worldMouseX;
				//	graph4.data.temprectAndCoordy2 = worldMouseY;
				//	drawRectArea();
				//}

				//}
			}
			break;
			case WM_LBUTTONUP:
			{
				{

					if (displayData)
					{
						displaySelectedData();
					}

					if (reverseDataAxis == 1)
					{
						setReverseDataX();

					}
					else if (reverseDataAxis == 2)
					{
						setReverseDataY();
					}
				}
				//check if the mouse click was on a graph and detect whcih 
				if (graph4.dragging) {
					graph4.updatePlotLocation(worldMouseX, worldMouseY, graph4.plotNumClicked);
					graph4.dragging = false;
					graph4.plotNumClicked = -1;
				}


				if (this->drawingDragged) {

					this->drawingDragged = false;

				}


			}
			break;
			case WM_KEYDOWN:
			{
				switch ((int)msg.WParam) {
				case VK_UP:
				{
					MessageBox::Show("Key Up");
					graph4.data.setClassColor(255.0, 0.0, 0.0, 1);
				}
				break;
				}
			}
			break;
			}
			NativeWindow::WndProc(msg);
		}

		
	private:
		void setReverseDataX();
		void setReverseDataY();
	public:
	// 1=x, 2=y
	int reverseDataAxis;


	
	

public:

	int getNumberOfGraphs();
	System::String^ logLineCoords(System::String^);
	void showHideLines();
	void displayClassOnTop(int classNumber);
	void showHideClass(int classNumber, int hideVar);
	void parserFileOpen();
	void showTrainigData();
	void showTestingData();
	void displaySelectedData();
};
}
























int OpenGLForm::COpenGL3::getNumberOfGraphs()
{
	
	return graph4.data.numPlots;
}


System::String^ OpenGLForm::COpenGL3::logLineCoords(System::String^ func)
{
	System::String^ log;
	log = gcnew System::String("");
	for (int i = 0; i < graph4.data.plotWidth.size(); i++)
	{
		log = log + graph4.data.xPlotCoordinates[i] + " " + graph4.data.plotWidth[i] + " ";

	}	

	return func + "-" + log + "\n";
}



void OpenGLForm::COpenGL3::showHideLines()
{
	graph4.data.showHideLinesVar = !(graph4.data.showHideLinesVar);
	
}





void OpenGLForm::COpenGL3::displayClassOnTop(int classNumber)
{
	graph4.data.classToDisplayOnTop = classNumber + 1; 
}


void OpenGLForm::COpenGL3::showHideClass(int classNumber, int hideVar)
{
	for (int i = 0; i < graph4.data.xdata.size(); i++)
	{
		if ((classNumber+1) == graph4.data.classNum[i])
		{
			graph4.data.dataTransparency[i] = hideVar;
		}
	}
	
}







void OpenGLForm::COpenGL3::parserFileOpen()
{
	graph4.data.fOpen = true;
}


void OpenGLForm::COpenGL3::showTrainigData()
{
	
	std::vector<int> dataSizePerClass;
	std::vector<int> dataIndexPerClass;
	int temp=0;
	for (int t = 0; t < graph4.data.xdata.size(); t++)
	{
		graph4.data.dataTransparency[t] = 128;
	}
	for (int j = 1; j <= graph4.data.numOfClasses; j++)
	{
		for (int k = 0; k < graph4.data.xdata.size(); k++)
		{
			if (j == graph4.data.classNum[k])
				temp += 1;

		}
		dataSizePerClass.push_back(temp);
		temp = 0;
	}

	for (int p = 0; p < dataSizePerClass.size(); p++)
	{
		dataIndexPerClass.push_back(temp);
		temp += dataSizePerClass[p];
	}
	

	for (int i = 0; i < dataIndexPerClass.size(); i++)
	{
		for (int k = dataIndexPerClass[i]; k < (dataIndexPerClass[i] + dataSizePerClass[i]*0.2); k++)
		{
			graph4.data.dataTransparency[k] = 0;
		}
	}
	dataSizePerClass.clear();
	dataIndexPerClass.clear();
}


void OpenGLForm::COpenGL3::showTestingData()
{
	for (int t = 0; t < graph4.data.xdata.size(); t++)
	{
		graph4.data.dataTransparency[t] = 128;
	}

	std::vector<int> dataSizePerClass;
	std::vector<int> dataIndexPerClass;
	int temp = 0;
	for (int t = 0; t < graph4.data.xdata.size(); t++)
	{
		graph4.data.dataTransparency[t] = 128;
	}
	for (int j = 1; j <= graph4.data.numOfClasses; j++)
	{
		for (int k = 0; k < graph4.data.xdata.size(); k++)
		{
			if (j == graph4.data.classNum[k])
				temp += 1;

		}
		dataSizePerClass.push_back(temp);
		temp = 0;
	}

	for (int p = 0; p < dataSizePerClass.size(); p++)
	{
		dataIndexPerClass.push_back(temp);
		temp += dataSizePerClass[p];
	}


	for (int i = 0; i < dataIndexPerClass.size(); i++)
	{
		for (int k = dataIndexPerClass[i]; k < (dataIndexPerClass[i] + dataSizePerClass[i] * 0.8); k++)
		{
			graph4.data.dataTransparency[k] = 0;
		}
	}
}


void OpenGLForm::COpenGL3::displaySelectedData()
{
	float rectClickedX, rectClickedY; 
	int rectIndex;
	
	rectClickedX = (worldMouseX - (graph4.data.xPlotCoordinates[graph4.plotNumClicked] - graph4.data.plotWidth[graph4.plotNumClicked] / 2)) / graph4.data.plotWidth[graph4.plotNumClicked];
	rectClickedY = 1 - (worldMouseY - (graph4.data.yPlotCoordinates[graph4.plotNumClicked] - graph4.data.plotHeight[graph4.plotNumClicked] / 2)) / graph4.data.plotHeight[graph4.plotNumClicked];

	for (int p = 0; p < graph4.dataParsed.parsedData.size(); p++)
	{
		if ((graph4.dataParsed.parsedData[p][4] == graph4.plotNumClicked) &&
		   (rectClickedX >= graph4.dataParsed.parsedData[p][0] && rectClickedX <= graph4.dataParsed.parsedData[p][2]
		   && rectClickedY >= graph4.dataParsed.parsedData[p][1] && rectClickedY <= graph4.dataParsed.parsedData[p][3]))
		{
			rectIndex = p;
			break;
		}
		
	}

	for (int i = 0; i < graph4.data.xdata.size(); i++)
	{
		if (graph4.data.xdata[i][graph4.plotNumClicked] < graph4.dataParsed.parsedData[rectIndex][0]||
			graph4.data.xdata[i][graph4.plotNumClicked] > graph4.dataParsed.parsedData[rectIndex][2] ||
			graph4.data.ydata[i][graph4.plotNumClicked] < graph4.dataParsed.parsedData[rectIndex][1] ||
			graph4.data.ydata[i][graph4.plotNumClicked] > graph4.dataParsed.parsedData[rectIndex][3] )
		
		{
		    graph4.data.dataTransparency[i] = 0.1; 
	    }


	}
}

void OpenGLForm::COpenGL3::setReverseDataX()
{

	for (int k = 0; k < graph4.data.xdata.size(); k++)
	{
		graph4.data.xdata[k][graph4.plotNumClicked] = 1 - graph4.data.xdata[k][graph4.plotNumClicked];
	}
	for (int i = 0; i < graph4.dataParsed.parsedData.size(); i++)
	{
		if (graph4.plotNumClicked == (int)graph4.dataParsed.parsedData[i][4])
		{
			graph4.dataParsed.parsedData[i][0] = 1 - graph4.dataParsed.parsedData[i][0];
			graph4.dataParsed.parsedData[i][2] = 1 - graph4.dataParsed.parsedData[i][2];

		}
	}
}


void OpenGLForm::COpenGL3::setReverseDataY()
{
	for (int k = 0; k < graph4.data.ydata.size(); k++)
	{
		graph4.data.ydata[k][graph4.plotNumClicked] = 1 - graph4.data.ydata[k][graph4.plotNumClicked];
	}
	for (int i = 0; i < graph4.dataParsed.parsedData.size(); i++)
	{
		if (graph4.plotNumClicked == (int)graph4.dataParsed.parsedData[i][4])
		{
			graph4.dataParsed.parsedData[i][1] = 1 - graph4.dataParsed.parsedData[i][1];
			graph4.dataParsed.parsedData[i][3] = 1 - graph4.dataParsed.parsedData[i][3];

		}
	}
}