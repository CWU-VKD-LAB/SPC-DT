/////////
///-------------------------------------------------------------------------------------------------
// file:	Cont_Parallel_Coordinates.cpp
//
// summary:	Implements the container parallel coordinates class
///-------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include <stdlib.h>
#include "Cont_Parallel_Coordinates.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Default constructor.	</summary>
///
/// <author>	Mahlet Saketa			</author>
/// <editor>	Paul Collet, Allyn Vo	</editor>
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////

Cont_Parallel_Coordinates::Cont_Parallel_Coordinates()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Clears the dynamic arrays. </summary>
///
/// <author>	Mahlet Saketa			</author>
/// <editor>	Paul Collet, Allyn Vo	</editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void Cont_Parallel_Coordinates::clearFields() {
	colors = 0;
	red.clear();
	green.clear();
	blue.clear();
	xdatatemp.clear();
	ydatatemp.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Creates starting x and y graph coordinates </summary>
/// <author>	Mahlet Saketa			</author>
/// <editor>	Paul Collet, Allyn Vo	</editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void Cont_Parallel_Coordinates::fillGraphLocations() {                                                                  
	for (int i = 1; i <= data.xdata.size(); i++) {
		data.xgraphcoordinates.push_back(data.worldWidth / 2);
		data.ygraphcoordinates.push_back(data.worldHeight / 2);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Draws data sets </summary>
///
/// <author>	Mahlet Saketa			</author>
/// <editor>	Paul Collet, Allyn Vo	</editor>
///
/// <param name="x1">   	The first x value. </param>
/// <param name="y1">   	The first y value. </param>
/// <param name="index">	Zero-based index of the. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void Cont_Parallel_Coordinates::drawData(float x1, float y1, int index) 
{                                              
	float xratio = data.graphwidth / data.xmax;							// Normalize data to the graph size
	float yratio = data.graphheight / data.ymax;

	x1 -= (data.graphwidth / 2);										// Start x's and y's from the bottom left of the graph
	y1 -= (data.graphheight / 2);

	glPushMatrix();														// Makes a new layer
	glTranslatef(x1 + data.pan_x, y1 + data.pan_y, 0);					// Translates starting position to draw
	glBegin(GL_LINE_STRIP);												// Line color. (i*50) to adjust color for different dimensions

	for (int i = 0; i < data.ydata.size(); i++) {
		//glColor3ub(0, (data.classNum[i] * 50) + 1, 100);
		int classnum = data.classNum[i] - 1;
		glColor3ub(data.classColor[classnum][0], data.classColor[classnum][1], data.classColor[classnum][2]);
		for (int j = 0; j < data.ydata[0].size(); j++) {
			if (j % (data.classsize) == 0) {
				glColor3f(0.8f, 0.8f, 0.8f);
				glVertex2f(xratio * (i), yratio * data.ydata[i][j] /* ydatatemp[i] */);
				glColor3ub(data.classColor[classnum][0], data.classColor[classnum][1], data.classColor[classnum][2]);
				glVertex2f(xratio * (i), yratio * data.ydata[i][j] /* ydatatemp[i] */);
			}
			else {
				glVertex2f(xratio * (i), yratio * data.ydata[i][j] /* ydatatemp[i] */);
			}
			
		}
	}
	glEnd();

	//glPointSize(4);
	//glBegin(GL_POINTS);
	
	for (int i = 0; i < data.ydata.size() /* ydatatemp.size() */; i++) {
		for (int j = 0; j < data.ydata[0].size() /* numOfClasses */; j++) {
			if (j == 0) {
				glPointSize(6);
			} // (3) Make a circle of the first point large than for the remaining  points in all visualizations (decrease other circles).
			else {
				glPointSize(4);
			}
			glBegin(GL_POINTS);

			glColor3ub(128 + (j*30), 0, 0);
			//glColor3ub(((data.classNum[i]-1) * 50) + 1, 0, 100);
			glVertex2f(xratio * (i), yratio * data.ydata[i][j]  /* ydatatemp[i] */);
			glEnd();
		}
	}

	
	glPopMatrix();														// Removes the layer

}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sorts graph by spliting values into x and y coords </summary>
/// <author>	Mahlet Saketa			</author>
/// <editor>	Paul Collet, Allyn Vo	</editor>
///
/// <param name="data">	[in,out] The data. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void Cont_Parallel_Coordinates::sortGraph(ClassData& data) 
{                                   
	float xCoord = 0;
	float yCoord = 0;
	int count = 0;

	data.getLabels();

	for (int i = 1; i < (data.values.size()); i++)						// Columns
	{     
		int nodeClass = stoi(data.values[i][(data.values[0].size() - 1)]);			// Get the class of the node
		if (nodeClass > data.numOfClasses)								// Get the highest class number
			data.numOfClasses = nodeClass;
		data.classNum.push_back(nodeClass);								// Add to vector of class numbers
		for (int j = 1; j < (data.values[i].size() - 1); j++)			// Rows
		{                                          
			yCoord = stof(data.values[i][j]);
			ydatatemp.push_back(yCoord);
			xdatatemp.push_back(j);
			if (yCoord > data.ymax) 
			{
				data.ymax = yCoord;
			}
			++count; //Moved from outer loop to inner
		}
		//count++;
		data.ydata.push_back(ydatatemp);
		data.xdata.push_back(xdatatemp);
		ydatatemp.clear();
		xdatatemp.clear();
	}
	data.xmax = count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Displays the OpenGL window. </summary>
///
/// <author>	Mahlet Saketa			</author>
/// <editor>	Paul Collet, Allyn Vo	</editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void Cont_Parallel_Coordinates::display()
{
	glClearColor(194 / 255.0f, 206 / 255.0f, 218 / 255.0f, 0.0f);		// background color - 194, 206, 218
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(data.leftWidth, data.worldWidth,
		data.worldHeight, data.bottomHeight, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();													// Reset the model-view matrix
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2);


	data.drawGraph(data.xgraphcoordinates[0], data.ygraphcoordinates[0]);

	glLoadIdentity();                                                   // Reset the model-view matrix


	drawData(data.xgraphcoordinates[0], data.ygraphcoordinates[0], 0);

	glColor3d(0, 0, 0);
	data.drawLabels();

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Normalize data. </summary>
///
/// <author> Phillipe Shin 02/24/2019 </author>
/// <editor> Paul Collet </edit>
////////////////////////////////////////////////////////////////////////////////////////////////////

void Cont_Parallel_Coordinates::normalizeData()
{
	std::vector<float> minYcol;
	std::vector<float> maxYcol;
	std::vector<float> temp;

	std::vector<std::vector<float> > convertedValues; // NEW


	float min = 0;
	float max = 0;

	for (int j = 0; j < data.ydata[0].size(); j++) {                                               // Gets the min and max of every column
		for (int i = 0; i < data.ydata.size(); i++) {
			if (data.ydata[i][j] > max) {
				max = data.ydata[i][j];
			}
			if (data.ydata[i][j] < min) {
				min = data.ydata[i][j];
			}
		}
		minYcol.push_back(min);
		maxYcol.push_back(max);
		max = 0;
		min = 0;
	}
	for (int i = 0; i < data.ydata.size(); i++) {                                                  // Normalize the data from 0 - 1
		for (int j = 0; j < data.ydata[0].size(); j++) {
			float original = data.ydata[i][j];
			float currentMin = minYcol[j];
			float currentMax = maxYcol[j];
			float converted = (original - currentMin) / (currentMax - currentMin);

			temp.push_back(converted);
		}
		convertedValues.push_back(temp);
		temp.clear();
	}
	data.ydata.clear();
	data.ydata = convertedValues;																	// Fill ydata coordinates with normalized data  
	
	data.xmax = data.ydata.size();																// Change xMax and yMax to normalized data
	data.ymax = 1;

	convertedValues.clear();
}