

#include "stdafx.h"
#include "SeparatedTS_SPC.h"

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Constructor. </summary>
/// <author>	Phillipe Shin & Allyn Vo 02/24/2019
/// <editor>	Paul Collet	</editor>
/// 
/// <remarks>	
/// This file is similar to the distance_based_time_series.
/// The following changes were made:
///		- Added the graphByPairs() method, you shouldn't need to add or make changes to it outside of the method
///		- Most of the methods have slight adjustments to them besides the sortGraph, openFile, and drawGraph
///		- Add these method calls to constructor:
///			sortDistances();
///			graphByPairs();
/// </remarks>
///
/// <param name="given"> 	[in,out] The given. </param>
/// <param name="worldW">	The world w. </param>
/// <param name="worldH">	The world h. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

SeparatedTS_SPC::SeparatedTS_SPC(ClassData &given, double worldW, double worldH)
{
	this->data = given;

	data.worldWidth = worldW;
	data.worldHeight = worldH;

	data.xmax = 0;
	data.ymax = 0;

	newFile.openFile(data);
	sortGraph2(data);

	normalizeData();
	graphByPairs();

	data.classsize = int(data.xdata[0].size());

	data.graphwidth = worldW / 6; // Width size for each graph
	data.graphheight = worldH / (data.numOfClasses + 1); // Height size for each graph


	fillGraphLocations();  // Creates starting graph positions, and fills example data for now.

	/////////////////////////////////////////////////////////////////////////////////////////////////

	display();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Normalize data. </summary>
///
/// <author>	Phillipe Shin & Allyn Vo 02/24/2019
/// <editor>	Paul Collet	</editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SeparatedTS_SPC::normalizeData() {														// Normalizes the data
	std::vector<float> minYcol;
	std::vector<float> maxYcol;
	std::vector<float> temp;

	float min = 0;
	float max = 0;
	float xMAX = 0;
	for (int j = 0; j < data.ydata[0].size(); j++) {                                        // Gets the min and max of every column
		for (int i = 0; i < data.ydata.size(); i++) {
			if (data.ydata[i][j] > max)
				max = data.ydata[i][j];
			if (data.ydata[i][j] < min)
				min = data.ydata[i][j];
		}
		minYcol.push_back(min);
		maxYcol.push_back(max);
	}

	for (int i = 0; i < data.ydata.size(); i++) {											// Normalize the data from 0 - 1
		for (int j = 0; j < data.ydata[0].size(); j++) {
			float original = data.ydata[i][j];
			float currentMin = minYcol[j];
			float currentMax = maxYcol[j];
			float converted = (original - currentMin) / (currentMax - currentMin);
			if (currentMax > xMAX) {
				xMAX = currentMax;
			}
			temp.push_back(converted);
		}
		convertedValues.push_back(temp);
		temp.clear();
	}
	data.ydata.clear();
	data.ydata = convertedValues;															// Fill ydata coordinates with normalized data

	for (int i = 0; i < data.ydata.size(); i++)												// Normalize X data
		for (int j = 0; j < data.ydata[0].size(); j++)
			data.xdata[i][j] = data.xdata[i][j] / xMAX;
	data.ymax = 1;
	data.xmax = 1;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Graph by pairs. Splits the values into x and y coords. </summary>
///
/// <author>	Phillipe Shin & Allyn Vo 02/24/2019
/// <editor>	Paul Collet	</editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SeparatedTS_SPC::graphByPairs()
{                                                                        
	float xCoord = 0;
	float yCoord = 0;

	for (int i = 0; i < data.ydata.size(); i++) {											// Columns
		for (int j = 0; j < data.ydata[0].size(); j++) {									// Rows
			if (xdatatemp.size() <= ydatatemp.size()) {										// Get X-coords
				xCoord = data.ydata[i][j];
				xdatatemp.push_back(xCoord);
				if (xCoord > data.xmax) {
					data.xmax = xCoord;
				}
			}
			else {																			// Get y coords
				yCoord = data.ydata[i][j];
				ydatatemp.push_back(yCoord);
				if (yCoord > data.ymax) {
					data.ymax = yCoord;
				}
			}
		}
		if (xdatatemp.size() != ydatatemp.size()) {											// Duplicate last pair if odd # of columns
			ydatatemp.push_back(xCoord);
		}
		distanceX.push_back(xdatatemp);														// Add line plot coords
		distanceY.push_back(ydatatemp);
		xdatatemp.clear();																	// Clear for the next plot line
		ydatatemp.clear();
	}

	resetDist();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Resets the distance. </summary>
///
/// <author>	Phillipe Shin & Allyn Vo 02/24/2019
/// <editor>	Paul Collet	</editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SeparatedTS_SPC::resetDist()
{
	data.xdata.clear();																			// Replace old graph values with new distances
	data.ydata.clear();
	data.xdata = distanceX;
	data.ydata = distanceY;
	distanceX.clear();
	distanceY.clear();
}

/* Draws data sets */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Draw data. </summary>
///
/// <author>	Phillipe Shin & Allyn Vo 02/24/2019
/// <editor>	Paul Collet	</editor>
///
/// <param name="x1">	   	The first x value. </param>
/// <param name="y1">	   	The first y value. </param>
/// <param name="index">   	Zero-based index of the. </param>
/// <param name="curClass">	The current class. </param>
/// <param name="j">	   	An int to process. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SeparatedTS_SPC::drawData(float x1, float y1, int index, int curClass, int j)
{	
	float xratio = data.graphwidth / data.xmax;													// Normalize data to the graph size
	float yratio = data.graphheight / data.ymax;

	float x2;
	float y2;

	if (j < (columns -1))
	{
		x2 = data.xclasses[(data.classNum[index] - 1)][j + 1];                                                    // Connect the CPCS
		y2 = data.yclasses[(data.classNum[index] - 1)][j + 1];
	}
	else
	{
		x2 = x1;
		y2 = y1;
	}


	y2 += (data.graphheight / 2);
	x2 -= (data.graphwidth / 2);
	x1 -= (data.graphwidth / 2);                                                                // Start x's and y's from the bottom left of the graph
	y1 += (data.graphheight / 2);

	glPushMatrix();																				// Makes a new layer
	glTranslatef(x1 + data.pan_x, y1 + data.pan_y, 0);                                          // Translates starting position to draw
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);

	//glColor3ub(0, ((curClass + 1) * 50), 100);														// Line color
	int classnum = data.classNum[index] - 1;
	glColor3ub(data.classColor[classnum][0], data.classColor[classnum][1], data.classColor[classnum][2]);
	//glColor3ub(data.classColor[(data.classNum[index] - 1)][0], data.classColor[(data.classNum[(data.classNum[index] - 1)])][1], data.classColor[(data.classNum[(data.classNum[index] - 1)])][2]);

	for (int i = 0; i < data.classsize; i++) // Draw Left Vertex
	{
		glVertex2f(xratio * data.xdata[index][i], -yratio * data.ydata[index][i]);
	}
	glEnd();

	if (j != (columns - 1)) {																				// Connect the time series graphs
		if (index < data.classNum.size() - 1)
		{
			glLineWidth(1);
			glBegin(GL_LINE_STRIP);
			glColor3ub(linecolor[0], linecolor[1], linecolor[2]);																// Line color

			glVertex2f(xratio * data.xdata[index][data.classsize - 1],
				-yratio * data.ydata[index][data.classsize - 1]);

			glVertex2f((x2 - x1) + xratio * data.xdata[index + 1][data.classsize - 1],
				(y2 - y1) - yratio * data.ydata[index + 1][data.classsize - 1]);		
			glEnd();
		}
	}

	glPointSize(4);																				// Add point to show direction
	glBegin(GL_POINTS);
	glColor3ub(200, 0, 0);

	for (int i = 0; i < data.classsize; i++) 
	{
		glVertex2f(xratio * data.xdata[index][data.classsize - 1], -yratio * data.ydata[index][data.classsize - 1]);
	}
	glEnd();
	glPopMatrix();   // Removes the layer
	
	//glPointSize(2);  
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Creates starting x and y graph coordinates </summary>
///
/// <author>	Phillipe Shin & Allyn Vo 02/24/2019
/// <editor>	Paul Collet	</editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SeparatedTS_SPC::fillGraphLocations()
{                                                               
	for (int k = 1; k <= data.xdata.size(); k++)
	{
		data.xgraphcoordinates.clear();
		data.ygraphcoordinates.clear();

		for (int i = 1; i <= columns; i++)
		{
			data.xgraphcoordinates.push_back(data.graphwidth * i + i * 5);
			data.ygraphcoordinates.push_back(data.graphheight * k + k * 10);
		}

		data.xclasses.push_back(data.xgraphcoordinates);
		data.yclasses.push_back(data.ygraphcoordinates);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Displays this window. </summary>
///
/// <author>	Phillipe Shin & Allyn Vo 02/24/2019
/// <editor>	Paul Collet	</editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SeparatedTS_SPC::display()
{																							
	glClearColor(194 / 255.0f, 206 / 255.0f, 218 / 255.0f, 0.0f);								// background color - 194, 206, 218
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* Adjust the windows aspect ratio? */
	glOrtho(data.leftWidth, data.worldWidth, data.worldHeight, data.bottomHeight, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();      // Reset the model-view matrix

	glEnable(GL_BLEND);																			// Smoothen lines drawn
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);

	glLineWidth(2);

	for (int k = 0; k < data.numOfClasses; k++)													// Draws a graph for each dimension
	{
		for (int i = 0; i < columns; i++)
			data.drawGraph(data.xclasses[k][i], data.yclasses[k][i]);
		glLoadIdentity();																		// Reset the model-view matrix
	}

	std::vector<int> linesPerClass;																// Count rows per class
	int prevClass = 0;
	int perClass = 0;

	for (int q = 0; q < data.classNum.size(); q++) {                                            // Split by classes
		int temp = data.classNum[q] - 1;
		if (temp == prevClass)
			perClass = perClass + 1;
		if (temp != prevClass) {
			linesPerClass.push_back(perClass);
			prevClass = temp;
			perClass = 1;
		}
		if (q == data.classNum.size() - 1) {
			linesPerClass.push_back(perClass);
			prevClass = temp;
			perClass = 0;
		}
	}
	int countTotal = 0;
	int previous = 0;
	for (int k = 0; k < linesPerClass.size(); k++)
	{
		int temp = linesPerClass[k];
		for (int i = 0; i < temp; i++)															// Plots for each dimension
		{
			if (previous == k)
			{
				drawData(data.xclasses[(data.classNum[countTotal] - 1)][i % columns], data.yclasses[(data.classNum[countTotal] - 1)][i % columns], countTotal, k, i % columns);
				countTotal++;
			}
		}
		previous = k + 1;
	}

	glColor3b(0, 0, 0);
	data.drawLabels();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sort graph 2. Specific to this graph.</summary>
///
/// <author>	Phillipe Shin & Allyn Vo 02/24/2019
/// <editor>	Paul Collet	</editor>
///
/// <param name="data">	[in,out] The data. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SeparatedTS_SPC::sortGraph2(ClassData& data)
{
	std::vector<float> xdatatemp;
	std::vector<float> ydatatemp;
	float yCoord = 0;
	int count = 0;

	data.getLabels();

	for (int i = 1; i < (data.values.size()); i++)
	{
		int nodeClass = stoi(data.values[i][(data.values[0].size() - 1)]);

		if (nodeClass > data.numOfClasses)
			data.numOfClasses = nodeClass;

		data.classNum.push_back(nodeClass);

		for (int j = 1; j < (data.values[i].size() - 1); j++)
		{
			yCoord = stof(data.values[i][j]);

			ydatatemp.push_back(yCoord);
			xdatatemp.push_back(count);
			if (yCoord > data.ymax)
			{
				data.ymax = yCoord;
			}
		}
		data.ydata.push_back(ydatatemp);
		data.xdata.push_back(xdatatemp);
		ydatatemp.clear();
		xdatatemp.clear();
		count = 0;
	}
}

// *** STUFF BELOW THIS IS FOR DRAGGING GRAPHS *** //////////////////////////////////////////

/* This method takes the passed mouse click coordinates and finds the graph clicked on. */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Searches for the first clicked graph. </summary>
///
/// <author>	Phillipe Shin & Allyn Vo 02/24/2019
/// <editor>	Paul Collet	</editor>
///
/// <param name="x">	The x coordinate. </param>
/// <param name="y">	The y coordinate. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SeparatedTS_SPC::findClickedGraph(double x, double y) {

	for (int i = 0; i < data.xclasses.size(); i++) {
		for (int j = 0; j < data.xclasses[0].size(); j++) {
			if (data.xclasses[i][j] + data.pan_x - (data.graphwidth / 2) <= x && x <= data.xclasses[i][j] + data.pan_x + (data.graphwidth / 2)
				&&
				data.yclasses[i][j] + data.pan_y - (data.graphheight / 2) <= y && y <= data.yclasses[i][j] + data.pan_y + (data.graphheight / 2)) {
				graphClickedi = i;
				graphClickedj = j;

			}
		}
	}
}
