
#include "stdafx.h"
#include "TimeSeries_Dist.h"

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Constructor. </summary>
///
/// <author> Phillipe Shin & Allyn Vo 02/25/2019 </author>
/// <editor> Paul Collet </editor>
///
/// <param name="given"> 	[in,out] The given. </param>
/// <param name="worldW">	The world w. </param>
/// <param name="worldH">	The world h. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

TimeSeries_Dist::TimeSeries_Dist(ClassData &given, double worldW, double worldH)
{
	this->data = given;

	data.worldWidth = worldW;
	data.worldHeight = worldH;

	data.xmax = 0;
	data.ymax = 0;

	newFile.openFile(data);
	sortGraph2();
	sortDistances(); //THE DIFFERENCE
	normalizeData();

	data.classsize = int(data.xdata[0].size());

	data.graphwidth = worldW * 0.35; // Width size for each graph
	data.graphheight = worldH / (data.numOfClasses + 1); // Height size for each graph

	fillGraphLocations();  // Creates starting graph positions, and fills example data for now.

	////////////////////////////////////////////////////////////////////////////////////////////////

	display();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Normalize data. </summary>
///
/// <author> Phillipe Shin & Allyn Vo 02/25/2019 </author>
/// <editor> Paul Collet </editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void TimeSeries_Dist::normalizeData()
{
	std::vector<float> minYcol;
	std::vector<float> maxYcol;
	std::vector<float> temp;

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
	}
	minYcol.push_back(min);
	maxYcol.push_back(max);
	max = 0;
	min = 0;

	for (int i = 0; i < data.ydata.size(); i++) {                                                  // Normalize the data from 0 - 1
		for (int j = 0; j < data.ydata[0].size(); j++) {
			float original = data.ydata[i][j];
			float currentMin = minYcol[0];
			float currentMax = maxYcol[0];
			float converted = (original - currentMin) / (currentMax - currentMin);

			temp.push_back(converted);
		}
		convertedValues.push_back(temp);
		temp.clear();
	}
	data.ydata.clear();
	data.ydata = convertedValues;																	// Fill ydata coordinates with normalized data  
	convertedValues.clear();
	data.xmax = data.ydata[0].size();																// Change xMax and yMax to normalized data
	data.ymax = 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Calculates the distances between each value. </summary>
///
/// <author> Phillipe Shin & Allyn Vo 02/25/2019 </author>
/// <editor> Paul Collet </editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void TimeSeries_Dist::sortDistances()
{                                                                        
	float previous = 0;
	float current;
	float coord = 0;
	int count = 0;
	std::vector<float> xdatatemp;
	std::vector<float> ydatatemp;
	std::vector<std::vector<float> > distanceX;														// Holds coords for the distance based time series
	std::vector<std::vector<float> > distanceY;

	for (int i = 0; i < data.ydata.size(); i++) {
		for (int j = 1; j < data.ydata[0].size(); j++) {											// Find abs(difference) between each index
			current = data.ydata[i][j];																// Get current value at index
			coord = float(current - previous);
			if (coord > data.ymax) {
				data.ymax = coord;
			}
			ydatatemp.push_back(coord);
			xdatatemp.push_back(count);
			previous = data.ydata[i][j];															// Store the previous value
			count++;
		}
		distanceY.push_back(ydatatemp);																// Push onto new
		distanceX.push_back(xdatatemp);
		xdatatemp.clear();																			// Reset for next graph
		ydatatemp.clear();
		count = 0;
	}
	data.xmax = distanceX[0].size();																// Set the size of x -axis
	data.ymax++;
	data.xdata.clear();																				// Replace old graph values with new distances
	data.ydata.clear();
	data.xdata = distanceX;
	data.ydata = distanceY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Fill graph locations. </summary>
///
/// <author> Phillipe Shin & Allyn Vo 02/25/2019 </author>
/// <editor> Paul Collet </editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void TimeSeries_Dist::fillGraphLocations() {
	for (int i = 1; i <= data.xdata.size(); i++) {
		data.xgraphcoordinates.push_back(data.worldWidth / 2);                                                   
		data.ygraphcoordinates.push_back(data.graphheight * i + i * 10);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Draw data. </summary>
///
/// <author> Phillipe Shin & Allyn Vo 02/25/2019 </author>
/// <editor> Paul Collet </editor>
///
/// <param name="x1">   	The first x value. </param>
/// <param name="y1">   	The first y value. </param>
/// <param name="index">	Zero-based index of the. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void TimeSeries_Dist::drawData(float x1, float y1, int index)
{
	float xratio = data.graphwidth / data.xmax;														// Normalize data to the graph size
	float yratio = data.graphheight / data.ymax;

	x1 -= (data.graphwidth / 2);                                                                    // Start x's and y's from the bottom left of the graph
	y1 += (data.graphheight / 2);

	glPushMatrix();																					// Makes a new layer
	glTranslatef(x1 + data.pan_x, y1 + data.pan_y, 0);                                              // Translates starting position to draw
	glBegin(GL_LINE_STRIP);

	//glColor3ub(0, (index * 50), 100);
	//glColor3ub(0, (data.classNum[index] * 50) + 1, 100);																// Line color
	int classnum = data.classNum[index] - 1;
	glColor3ub(data.classColor[classnum][0], data.classColor[classnum][1], data.classColor[classnum][2]);

	for (int i = 0; i < data.xdata[0].size(); i++)
	{
		glVertex2f(xratio * data.xdata[index][i], -yratio * data.ydata[index][i]);
	}
	glEnd();
	//glPointSize(4);
	//glBegin(GL_POINTS);
	glColor3ub(0, 0, 0);

	for (int i = 0; i < data.ydata[0].size(); i++)
	{                                         
		if (i == 0) {
			glPointSize(4);
		} // (3) Make a circle of the first point large than for the remaining  points in all visualizations (decrease other circles).
		else {
			glPointSize(2);
		}
		glBegin(GL_POINTS);

		glVertex2f(xratio * (i), -yratio * data.ydata[index][i]);

		glEnd();
	}
	
	glPopMatrix();																					// Removes the layer
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Displays this object. </summary>
///
/// <author> Phillipe Shin & Allyn Vo 02/25/2019 </author>
/// <editor> Paul Collet </editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void TimeSeries_Dist::display()
{
	glClearColor(194 / 255.0f, 206 / 255.0f, 218 / 255.0f, 0.0f);									// background color - 194, 206, 218
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(data.leftWidth, data.worldWidth, data.worldHeight, data.bottomHeight, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();      // Reset the model-view matrix

	glEnable(GL_BLEND);																				// Smoothen lines drawn
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);

	glLineWidth(2);

	for (int i = 0; i < data.numOfClasses; i++)														// Draw each class graph
		data.drawGraph(data.xgraphcoordinates[i], data.ygraphcoordinates[i]);
	glLoadIdentity();																				// Reset the model-view matrix

	for (int i = 0; i < data.classNum.size(); i++) {
		int curClass = (data.classNum[i]) - 1;
		drawData(data.xgraphcoordinates[curClass], data.ygraphcoordinates[curClass], i);
	}

	data.drawLabels();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sort graph 2. Class specific sorter.</summary>
///
/// <author>	Phillipe Shin & Allyn Vo 02/25/2019 </author>
/// <editor>	Paul Collet </editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void TimeSeries_Dist::sortGraph2()
{
	std::vector<float> xdatatemp;
	std::vector<float> ydatatemp;
	float yCoord = 0;
	int count = 0;

	data.getLabels();

	for (int i = 1; i < (data.values.size()); i++) {                                                // Columns
		int nodeClass = stoi(data.values[i][(data.values[0].size() - 1)]);                          // Get the class of the node
		if (nodeClass > data.numOfClasses)                                                          // Get the highest class number
			data.numOfClasses = nodeClass;
		data.classNum.push_back(nodeClass);                                                         // Add to vector of class numbers
		for (int j = 1; j < (data.values[i].size() - 1); j++) {                                     // Rows
			yCoord = stof(data.values[i][j]);
			ydatatemp.push_back(yCoord);
			xdatatemp.push_back(count);
			if (yCoord > data.ymax) {
				data.ymax = yCoord;
			}
			++count;
		}
		data.ydata.push_back(ydatatemp);
		data.xdata.push_back(xdatatemp);
		ydatatemp.clear();
		xdatatemp.clear();
		count = 0;
	}
	data.xmax = data.xdata[0].size();
}
