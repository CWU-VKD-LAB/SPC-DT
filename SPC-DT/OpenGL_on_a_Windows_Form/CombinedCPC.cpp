///-------------------------------------------------------------------------------------------------
// file:	CombinedCPC.cpp
//
// summary:	Implements the combined cpc class
///-------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "CombinedCPC.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Constructor. </summary>
///
/// <author>  Phillipe Shin 02/21/2019</author>
	/// <editor>  Paul Collet </editor>
///
/// <param name="given"> 	[in,out] The given. </param>
/// <param name="worldW">	The world w. </param>
/// <param name="worldH">	The world h. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

CombinedCPC::CombinedCPC(ClassData &given, double worldW, double worldH)
{
	this->data = given;

	data.worldWidth = worldW;
	data.worldHeight = worldH;

	data.xmax = 0;
	data.ymax = 0;

	newFile.openFile(data);
	newFile.sortGraph(data);

	data.classsize = int(data.xdata[0].size());

	data.graphwidth = worldW / 2.0;										// Width size for each graph
	data.graphheight = worldH / 2.0;									// Height size for each graph


	fillGraphLocations(); // Creates starting graph positions, and fills example data for now.

	//////////////////////////////////////////////////////////////////////////////////////////////

	display();
}

/* Input: none -- Creates starting x and y graph coordinates */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Fill graph locations. </summary>
///
/// <author>  Phillipe Shin 02/21/2019</author>
/// <editor>  Paul Collet </editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void CombinedCPC::fillGraphLocations() {

	for (int i = 1; i <= (signed)data.xdata.size(); i++) {
		data.xgraphcoordinates.push_back(data.worldWidth / 2);			// 600 is the window width which will need to be variable later.
		data.ygraphcoordinates.push_back(data.graphheight * i + i * 10);

	}

	for (int i = 0; i < (signed)data.xgraphcoordinates.size(); i++)
		data.originalxgraphcoordinates.push_back(data.xgraphcoordinates[i]);

	for (int i = 0; i < (signed)data.ygraphcoordinates.size(); i++)
		data.originalygraphcoordinates.push_back(data.ygraphcoordinates[i]);

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Draws the coordinate points onto the graph and connects them </summary>
///
/// <author>  Phillipe Shin 02/21/2019</author>
/// <editor>  Paul Collet </editor>
///
/// <param name="x1">   	The first x value. </param>
/// <param name="y1">   	The first y value. </param>
/// <param name="index">	Zero-based index of the. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void CombinedCPC::drawData(float x1, float y1, int index)
{
	float xratio = data.graphwidth / data.xmax;							// Normalize data to the graph size
	float yratio = data.graphheight / data.ymax;

	x1 -= (data.graphwidth / 2);                                        // Start x's and y's from the bottom left of the graph
	y1 += (data.graphheight / 2);

	glPushMatrix();                                                     // Makes a new layer
	glTranslatef(x1 + data.pan_x, y1 + data.pan_y, 0);                  // Translates starting position to draw
	glBegin(GL_LINE_STRIP);

	//glColor3ub(0, (index * 50), 100);
	//glColor3ub(0, (data.classNum[index] * 50) + 1, 100);
	int classnum = data.classNum[index] - 1;
	glColor3ub(data.classColor[classnum][0], data.classColor[classnum][1], data.classColor[classnum][2]);

	for (int i = 0; i < (signed)data.classsize; i++) {                  
		glVertex2f(xratio * data.xdata[index][i], -yratio * data.ydata[index][i]);
	}
	glEnd();
	//glPointSize(4);
	//glBegin(GL_POINTS);
	glColor3ub(0, 0, 0);

	for (int i = 0; i < (signed)data.classsize; i++) {                   
		if (i == 0) { 
			glPointSize(4);
		} // (3) Make a circle of the first point large than for the remaining  points in all visualizations (decrease other circles).
		else {
			glPointSize(2);
		}
		glBegin(GL_POINTS);

		glVertex2f(xratio * data.xdata[index][i], -yratio * data.ydata[index][i]);

		glEnd();
	}
	
	glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Create the graphs and display to the window. </summary>
///
/// <author>  Phillipe Shin 02/21/2019</author>
/// <editor>  Paul Collet </editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void CombinedCPC::display() {
	glClearColor(194 / 255.0f, 206 / 255.0f, 218 / 255.0f, 0.0f);		//194, 206, 218. VisCanvas background color.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* Adjust the windows aspect ratio? */
	glOrtho(data.leftWidth, data.worldWidth, data.worldHeight, data.bottomHeight, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // Reset the model-view matrix
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);

	glLineWidth(2); 

	/* Draws a graph for each dimension */
	for (int i = 0; i < 1; i++)       									// Changed I from classsize to one to for combined CPC
	{
		data.drawGraph(data.xgraphcoordinates[i], data.ygraphcoordinates[i]);
	}

	glLoadIdentity();

	/* Plots the data. Outer loop for each dimension. Inner loop for data across each graph.  */
	for (int i = 0; i < (signed)data.classNum.size(); i++) {
		drawData(data.xgraphcoordinates[0], data.ygraphcoordinates[0], i);
	}

	data.drawLabels();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Zooms. </summary>
///
/// <author> Allyn Vo </author>
///
/// <param name="factor">	The factor. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void CombinedCPC::zoom(double factor)
{
	data.graphwidth *= factor;
	data.graphheight *= factor;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Zoom at. </summary>
///
/// <author> Allyn Vo </author>
///
/// <param name="x">	 	The x coordinate. </param>
/// <param name="y">	 	The y coordinate. </param>
/// <param name="factor">	The factor. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void CombinedCPC::zoom_at(double x, double y, double factor) // Zooms in by a factor and towards the mouse location
{
	data.pan_x += (data.worldWidth / 2 - x) / 2;
	data.pan_y += (data.worldHeight / 2 - y) / 2;
	zoom(factor);
}

