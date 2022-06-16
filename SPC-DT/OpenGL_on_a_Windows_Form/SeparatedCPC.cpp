///-------------------------------------------------------------------------------------------------
// file:	SeparatedCPC.cpp
//
// summary:	Implements the class for a separated collocated paired cooridnate graph.
// /-------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "SeparatedCPC.h"

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Default Constructor. </summary>
///
/// <author> Phillipe Shin & Mahlet Saketa 02/21/2019 </author>
/// <editor> Paul Collet </editor>
///
/// <param name="given"> 	[in,out] The given. </param>
/// <param name="worldW">	The world w. </param>
/// <param name="worldH">	The world h. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

SeparatedCPC::SeparatedCPC(ClassData &given, double worldW, double worldH)
{
	this->data = given;

	data.worldWidth = worldW;
	data.worldHeight = worldH;

	data.xmax = 0;
	data.ymax = 0;

	newFile.openFile(data);
	newFile.sortGraph(data);

	data.classsize = int(data.xdata[0].size());

	data.graphwidth = worldW * 0.4;											// Width size for each graph
	data.graphheight = worldH / (data.numOfClasses + 1);					// Height size for each graph


	fillGraphLocations();													// Creates starting graph positions, and fills example data for now.

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	display();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Creates starting x and y graph coordinates and example data sets for now. </summary>
///
/// <author> Phillipe Shin & Mahlet Saketa 02/21/2019 </author>
/// <editor> Paul Collet </editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SeparatedCPC::fillGraphLocations() {

	for (int i = 1; i <= (signed)data.xdata.size(); i++)
	{
		data.xgraphcoordinates.push_back(this->data.worldWidth / 2);		// 600 is the window width which will need to be variable later.
		data.ygraphcoordinates.push_back(this->data.graphheight * i + i * 10);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Draws data sets. </summary>
///
/// <author> Phillipe Shin & Mahlet Saketa 02/21/2019 </author>
/// <editor> Paul Collet </editor>
///
/// <param name="x1">   	The first x value. </param>
/// <param name="y1">   	The first y value. </param>
/// <param name="index">	Zero-based index of the. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SeparatedCPC::drawData(float x1, float y1, int index, int curClass)
{
	float xratio = data.graphwidth / data.xmax;								// Normalize data to the graph size
	float yratio = data.graphheight / data.ymax;

	// Reminder: -x goes left, +x goes right, -y goes up, +y goes down, so origin starts from top left.
	x1 -= (data.graphwidth / 2);
	y1 += (data.graphheight / 2);											// start x's and y's from the bottom left of the graph

	glPushMatrix();															// Makes a new layer
	glTranslatef(x1 + data.pan_x, y1 + data.pan_y, 0);						// Translates starting position to draw

	glBegin(GL_LINE_STRIP);
	glColor3ub(data.classColor[curClass][0], data.classColor[curClass][1], data.classColor[curClass][2]);										// Line color
	for (int i = 0; i < (signed)data.xdata[0].size(); i++)
	{
		glVertex2f(xratio * data.xdata[index][i], -yratio * data.ydata[index][i]);
	}
	glEnd();

	
	//glBegin(GL_POINT);
	glColor3ub(0, 0, 0);

	for (int i = 0; i < data.xdata[0].size() - 1 + 1; i++)					// Draw Right vertex
	{	
		if(i == 0){ 
			//glColor3ub(100, 0, 0); 
			glPointSize(4);
		} // (3) Make a circle of the first point large than for the remaining  points in all visualizations (decrease other circles).
		else{ 
			//glColor3ub(0, 0, 0); 
			glPointSize(2); 
		}
		glBegin(GL_POINTS);
															   //glColor3f(data.red[curClass][i], data.green[curClass][i], data.blue[curClass][i]);
		glVertex2f(	xratio *data.xdata[index][i], -yratio * data.ydata[index][i]);

		glEnd();
	}
	
	glPopMatrix();															// Removes the layer
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Displays this object. </summary>
///
/// <author> Phillipe Shin & Mahlet Saketa 02/21/2019 </author>
/// <editor> Paul Collet </editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SeparatedCPC::display() {

	glClearColor(194 / 255.0f, 206 / 255.0f, 218 / 255.0f, 0.0f);			//194, 206, 218. VisCanvas background color.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//left, right, top, bottom
	glOrtho(data.leftWidth, this->data.worldWidth, 
		this->data.worldHeight, data.bottomHeight, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();														// Reset the model-view matrix

	/* Smoothen lines drawn. */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);

	glLineWidth(2);



	/* Draws a graph for each dimension */
	for (int i = 0; i < (signed)data.numOfClasses; i++)
	{
		data.drawGraph(data.xgraphcoordinates[i], data.ygraphcoordinates[i]);
	}
	glLoadIdentity();														// Reset the model-view matrix

	/* Plots the data. */
	for (int i = 0; i < (signed)data.classNum.size(); i++)
	{
		int curClass = data.classNum[i] - 1;
		drawData(data.xgraphcoordinates[curClass], data.ygraphcoordinates[curClass], i, curClass);
	}

	data.drawLabels();
}

