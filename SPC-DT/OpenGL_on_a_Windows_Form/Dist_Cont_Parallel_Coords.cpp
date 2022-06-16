////////////////////////////////////////////////////////////////////////////////////////////////////
// <author>Mahlet Saketa</author>
// <editor>Allyn Vo</editor>
// <date>3/8/2019</date>
// <summary>Implements the distance container parallel coordinates class</summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <cmath>
#include "Dist_Cont_Parallel_Coords.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Initializes a new instance of the Dist_Cont_Parallel_Coords class. </summary>
///
// <author>Mahlet Saketa</author>
// <editor>Allyn Vo</editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

Dist_Cont_Parallel_Coords::Dist_Cont_Parallel_Coords()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Clears the fields. </summary>
///
// <author>Mahlet Saketa</author>
// <editor>Allyn Vo</editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void Dist_Cont_Parallel_Coords::clearFields() {
	xdatatemp.clear();
	ydatatemp.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Normalize data. </summary>
///
// <author>Mahlet Saketa</author>
// <editor>Allyn Vo</editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void Dist_Cont_Parallel_Coords::normalizeData() {													// Normalizes the data
	std::vector<float> minYcol;
	std::vector<float> maxYcol;
	std::vector<float> temp;
	std::vector<std::vector<float> > convertedValues;

	float min = 0;
	float max = 0;

	for (int j = 0; j < data.ydata[0].size(); j++) {												// Gets the min and max of every column
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
	for (int i = 0; i < data.ydata.size(); i++) {													// Normalize the data from 0 - 1
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
	data.xmax = data.ydata[0].size();																// Change xMax and yMax to normalized data
	data.ymax = 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Calculates the distances between each value. </summary>
///
// <author>Mahlet Saketa</author>
// <editor>Allyn Vo</editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void Dist_Cont_Parallel_Coords::sortDistances() {                                                                       
	float previous = 0;
	float current;
	float coord = 0;
	int count = 0;
	std::vector<float> xdatatemp;
	std::vector<float> ydatatemp;
	std::vector<std::vector<float> > distanceX;														// Holds coords for the distance based time series
	std::vector<std::vector<float> > distanceY;

	for (int i = 0; i < data.ydata[0].size()/*ydata.size()*/; i++) {
		for (int j = 1; j < data.ydata.size()/*ydata[0].size()*/; j++) {                            // Find abs(difference) between each index
			current = data.ydata[j][i]/*ydata[i][j]*/;                                              // Get current value at index
			coord = float(current - previous);
			if (coord > data.ymax) {
				data.ymax = coord;
			}
			ydatatemp.push_back(coord);
			xdatatemp.push_back(count);
			previous = data.ydata[j][i]/*ydata[i][j]*/;												// Store the previous value
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


	std::vector<float> tempx;
	std::vector<float> tempy;
	std::vector<std::vector<float>> temp;

	for (int i = 0; i < data.ydata.size(); i++) {
		for (int j = 0; j < data.ydata[0].size(); j++) {
			tempy.push_back(data.ydata[i][j]);
		}
		temp.push_back(tempy);
		tempy.clear();
	}
	data.ydata = temp;
	//std::cout << ydata.size();
	data.xmax = data.ydata.size();

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Draw data. </summary>
///
// <author>Mahlet Saketa</author>
// <editor>Allyn Vo</editor>
///
/// <param name="x1">   	The first x value. </param>
/// <param name="y1">   	The first y value. </param>
/// <param name="index">	Zero-based index of the. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void Dist_Cont_Parallel_Coords::drawData(float x1, float y1, int index) {                                              
	float xratio = data.graphwidth / data.xmax;															// Normalize data to the graph size
	float yratio = data.graphheight / data.ymax;

	x1 -= (data.graphwidth / 2);																		// Start x's and y's from the bottom left of the graph
	y1 -= (data.graphheight / 2);

	glPushMatrix();																						// Makes a new layer
	glTranslatef(x1 + data.pan_x, y1 + data.pan_y, 0);                                                  // Translates starting position to draw
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < data.ydata[0].size()/*  xdata[0].size()*/; i++) {        

		int classnum = data.classNum[i] - 1;
		glColor3ub(data.classColor[classnum][0], data.classColor[classnum][1], data.classColor[classnum][2]);

		for (int j = 0; j < data.ydata.size(); j++) {/*new loop*/

			if (j % (data.classNum.size()) == 0) {
				glColor3f(0.8f, 0.8f, 0.8f);
				glVertex2f(xratio * i, yratio * data.ydata[j][i]);
				glColor3ub(data.classColor[classnum][0], data.classColor[classnum][1], data.classColor[classnum][2]);
				glVertex2f(xratio * i, yratio * data.ydata[j][i]);
			}
			else {
				glVertex2f(xratio * i, yratio * data.ydata[j][i]);
			}

		}
	}
	glEnd();


	for (int i = 0; i < data.ydata[0].size(); i++) { 
		glPointSize(6);
		for (int j = 0; j < data.ydata.size(); j++) {/*new loop*/

			glBegin(GL_POINTS);

			glColor3ub(128 + (j * 30), 0, 0);

			glVertex2f(xratio * i,
				yratio * data.ydata[j][i]   /*    ydata[index][i]   */);
			glEnd();
			glPointSize(3);
		}
	}
	
	glPopMatrix();																						// Removes the layer
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Creates starting x and y graph coordinates. </summary>
///
// <author>Mahlet Saketa</author>
// <editor>Allyn Vo</editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void Dist_Cont_Parallel_Coords::fillGraphLocations() {                                                                  
	for (int i = 1; i <= data.xdata.size(); i++) {
		data.xgraphcoordinates.push_back(data.worldWidth / 2);
		data.ygraphcoordinates.push_back(data.worldHeight / 2);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Displays this object. </summary>
///
// <author>Mahlet Saketa</author>
// <editor>Allyn Vo</editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

void Dist_Cont_Parallel_Coords::display() {                                                                             
	glClearColor(194 / 255.0f, 206 / 255.0f, 218 / 255.0f, 0.0f);										// VisCanvas background color - 194, 206, 218
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(data.leftWidth, data.worldWidth, data.worldHeight, data.bottomHeight, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();																					// Reset the model-view matrix
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2);


	data.drawGraph(data.xgraphcoordinates[0], data.ygraphcoordinates[0]);

	glLoadIdentity();																					// Reset the model-view matrix

	drawData(data.xgraphcoordinates[0], data.ygraphcoordinates[0], 0);

	glColor3d(0, 0, 0);
	data.drawLabels();
                                                                 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Splits the values into x and y coords. </summary>
///
// <author>Mahlet Saketa</author>
// <editor>Allyn Vo</editor>
///
/// <param name="data">	[in,out] The data. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void Dist_Cont_Parallel_Coords::sortGraph(ClassData& data) {                                    
	std::vector<float> xdatatemp;
	std::vector<float> ydatatemp;
	float yCoord = 0;
	int count = 0;

	data.getLabels();

	for (int i = 1; i < (data.values.size() ); i++) {                                                   // Columns
		int nodeClass = stoi(data.values[i][(data.values[0].size() - 1)]);                              // Get the class of the node
		if (nodeClass > data.numOfClasses)																// Get the highest class number
			data.numOfClasses = nodeClass;
		data.classNum.push_back(nodeClass);																// Add to vector of class numbers
		for (int j = 1; j < (data.values[i].size() - 1); j++) {                                         // Rows
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
	data.xmax = 1  /*  xdata[0].size()  */;
}
