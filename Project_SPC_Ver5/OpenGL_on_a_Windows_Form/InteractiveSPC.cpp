///-------------------------------------------------------------------------------------------------
// file:	InteractiveSPC.cpp
//
// summary:	Implements the class for a combined shifted paired coordinate graph.
///-------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "InteractiveSPC.h"



InteractiveSPC::InteractiveSPC(ClassData &given, parseData &given1, double worldW, double worldH)
{
	this->data = given;
	this->dataParsed = given1;
	data.worldWidth = worldW;
	data.worldHeight = worldH;

	data.xmax = 0;
	data.ymax = 0;

	//newFile.openFile(data);
	//newFile.openParserFile(dataParsed, data);
	newFile.sortGraph(data);

	data.classsize = int(data.xdata[0].size());
	for (int y1 = 0; y1 < data.graphwidth.size(); y1++)
	{

		{
			data.graphwidth[y1] = worldW / (data.classsize * 2.5);
			data.graphheight[y1] = worldH / 2.0;
		}

	}
						// Width size for each graph
											// Height size for each graph


	fillGraphLocations(); // Creates starting graph positions, and fills example data for now.

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	display();
}

void InteractiveSPC::setBackgroundTransparency(float alpha) {
	this->backgroundTransparency = alpha;
}

void InteractiveSPC::setBackgroundColorLightness(float lightnessCoeff) {
	this->backgroundClassColorCoefficient = lightnessCoeff;
}

// Filling Graph Locations
void InteractiveSPC::fillGraphLocations()
{
	//for (int k = 1; k <= data.xdata.size(); k++)
	//{

		for (int i = 1; i <= data.classsize; i++)
		{

			//data.xgraphcoordinates.push_back(data.graphwidth[i - 1] * i + i * 0.08 * data.graphwidth[data.graphwidth.size() - 1]);			
			data.xgraphcoordinates.push_back(data.graphwidth[i - 1] * i + i * 10);
			data.ygraphcoordinates.push_back(data.graphheight[i - 1]);

			//set coordinates to draw rectangles
			data.x1CoordGraph.push_back(data.xgraphcoordinates[i - 1] - data.graphwidth[i - 1] / 2);
			data.x2CoordGraph.push_back(data.xgraphcoordinates[i - 1] + data.graphwidth[i - 1] / 2);
			data.y1CoordGraph.push_back(data.ygraphcoordinates[i - 1] - data.graphheight[i - 1] / 2);
			data.y2CoordGraph.push_back(data.ygraphcoordinates[i - 1] + data.graphheight[i - 1] / 2);

		}

		data.xclasses.push_back(data.xgraphcoordinates);
		data.yclasses.push_back(data.ygraphcoordinates);
	//}
}

/* Draws data sets. */


void InteractiveSPC::drawData(float x1, float y1, float x2, float y2, int i, int j)
{
	
	float xratio = data.graphwidth[j] / data.xmax; // Normalize data to the graph size
	float yratio = data.graphheight[j] / data.ymax;	
	x1 -= (data.graphwidth[j] / 2);
	x2 -= (data.graphwidth[j + 1] / 2);
	y1 += (data.graphheight[j] / 2);
	y2 += (data.graphheight[j + 1] / 2);
	float x1Coord = data.graphwidth[j] * data.xdata[i][j];
	float x2Coord = (x2 - x1) + data.graphwidth[j + 1] * data.xdata[i][j + 1];
	float y1Coord = -data.graphheight[j] * data.ydata[i][j]; //height of graph is constant = 328.5
	float y2Coord = (y2 - y1) - data.graphheight[j+1] * data.ydata[i][j + 1];
	float x1CoordTrans = x1Coord + (x1 + data.pan_x);
	float x2CoordTrans = x2Coord + (x1 + data.pan_x);
	float y1CoordTrans = y1Coord + (y1 + data.pan_y);
	float y2CoordTrans = y2Coord + (y1 + data.pan_y);


	int classnum = data.classNum[i] - 1;

	
	// If in drawn rectangle mode, check if the points make a line that intersects the rectangle
	GLfloat middleX;
	GLfloat middleY;
	bool drawMiddleVertex = false;

	if (isRectangleMode && doPointsIntersectRectangle(x1CoordTrans, y1CoordTrans, x2CoordTrans, y2CoordTrans)) {
		// draw point within rectangle based on how many classes there are
		// TODO: later could add a selection based on class
		GLfloat deltaX = abs(rectX1 - rectX2);
		GLfloat deltaY = abs(rectY1 - rectY2);
		GLfloat highX = max(rectX1, rectX2);
		GLfloat lowX = min(rectX1, rectX2);
		GLfloat highY = max(rectY1, rectY2);
		GLfloat lowY = min(rectY1, rectY2);

//		middleX = lowX + ((highX - lowX) / 5) + ((deltaX / data.numOfClasses) * classnum) - (x1 + data.pan_x);
//		middleY = lowY + ((highY - lowY) / 5) + ((deltaY / data.numOfClasses) * classnum) - (y1 + data.pan_y);

		middleX = lowX + ((highX - lowX) / 2) - (x1 + data.pan_x);
		middleY = lowY + ((highY - lowY) / data.numOfClasses + 1) * classnum - (y1 + data.pan_y);

		drawMiddleVertex = true;

		//debug
		glColor4ub(255, 0, 0, 255);
		glPointSize(8.0);
		glBegin(GL_POINTS);
		glVertex2f(x1CoordTrans, y1CoordTrans);
		glVertex2f(x2CoordTrans, y2CoordTrans);
		glPointSize(4.0);
		glEnd();
	}

	glPushMatrix();	// Makes a new layer

	glTranslatef(x1 + data.pan_x, y1 + data.pan_y, 0); // Translates starting position to draw

	if (data.showHideLinesVar)
	{
		glBegin(GL_LINE_STRIP);
		//data.classTransparency[1] = 70;
		glColor4ub(data.classColor[classnum][0], data.classColor[classnum][1], data.classColor[classnum][2], data.dataTransparency[i]);
		glVertex2f(x1Coord, y1Coord);
		if (drawMiddleVertex) {
			glVertex2f(middleX, middleY);
		}
		if(data.classsize!=1)
			glVertex2f(x2Coord, y2Coord); // ending vertex
		glEnd();
	}
	
	glColor4ub(0, 0, 0, data.dataTransparency[i]);
	if (j == 0) {
		glPointSize(4);
	}
	else {
		glPointSize(4);
	}
	glBegin(GL_POINTS);
	
	//glVertex2f(0 + xratio * data.xdata[i][j], 0 - yratio * data.ydata[i][j]);                                     // starting vertex
	glVertex2f(x1Coord, y1Coord);
	if (drawMiddleVertex) {
		glVertex2f(middleX, middleY);
	}
	glEnd();

	glPointSize(4);
	glBegin(GL_POINTS);
	//glVertex2f((x2 - x1) + xratio * data.xdata[i][j + 1], (y2 - y1) - yratio * data.ydata[i][j + 1]);                         // ending vertex
	if (data.classsize != 1)
		glVertex2f(x2Coord, y2Coord);	
	glEnd();
	if (j == 0)
	{
		glColor4ub(data.classColor[classnum][0], data.classColor[classnum][1], data.classColor[classnum][2], data.dataTransparency[i]);
		drawCircle(x1Coord, y1Coord);
	}

	glPopMatrix();
	
}

/* DISPLAY */


void InteractiveSPC::display() {

	glClearColor(255 / 255.0f, 255 / 255.0f, 254 / 255.0f, 0.0f);			//194, 206, 218. VisCanvas background color.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* Adjust the windows aspect ratio? */
	glOrtho(data.leftWidth, data.worldWidth, data.worldHeight, data.bottomHeight, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();														// Reset the model-view matrix

	/* Draws a graph for each dimension */
	for (int i = 0; i < data.classsize; i++)
	{
		data.drawGraph(data.xgraphcoordinates[i], data.ygraphcoordinates[i], data.graphwidth[i], data.graphheight[i], i);
	}

	//comment to see only coordinates
	glLoadIdentity();													// Reset the model-view matrix
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_ALWAYS, 1, 1);
	//glStencilOp(GL_KEEP, GL_ZERO, GL_REPLACE);
	glEnable(GL_LINE_SMOOTH);

	//if (dataParsed.parserFileName.empty() && data.parsedData.size() == 0)
	//{
	//	FileHandling newFile;
	//	dataParsed.parserFileName = "D:\\MyDocs\\CWU\\Fall2020\\Data Mining\\Projects\\parserBC.txt";
	//	newFile.openParserFile(dataParsed, data);
	//}

	if (dataParsed.parsedData.size() > 0)
	{
		for (int p = 0; p < dataParsed.parsedData.size(); p++)
		{
			int classNumber = dataParsed.parsedData[p][5];
			// Old background color assignment
			/*if (dataParsed.parsedData[p][5] == 0)
			{
				glColor4ub(255, 0, 0, backgroundTransparency);
			}
			else if (dataParsed.parsedData[p][5] == 1)
			{
				glColor4ub(0, 255, 0, backgroundTransparency);
			}
			else if (dataParsed.parsedData[p][5] == 2)
			{
				glColor4ub(0, 0, 255, backgroundTransparency);
			}
			else if (dataParsed.parsedData[p][5] == -1)
			{
				glColor4ub(169, 169, 169, backgroundTransparency);
			}*/

			if (classNumber == -1) {
				// if class is -1, then it is the background;
				glColor4ub(169, 169, 169, backgroundTransparency);
			}
			else {
				// we need to adjust the lightness of the background color
				std::vector<float> hsl = RGBtoHSL(data.classColor[classNumber]);

				hsl[2] = hsl[2] * backgroundClassColorCoefficient;

				std::vector<GLubyte> rgb = HSLtoRGB(hsl);

				glColor4ub(rgb[0], rgb[1], rgb[2], backgroundTransparency);
			}

			const GLfloat x1 = data.xgraphcoordinates[dataParsed.parsedData[p][4]] - data.graphwidth[dataParsed.parsedData[p][4]] / 2 + dataParsed.parsedData[p][0] * data.graphwidth[dataParsed.parsedData[p][4]];
			const GLfloat y1 = data.ygraphcoordinates[dataParsed.parsedData[p][4]] + data.graphheight[dataParsed.parsedData[p][4]] / 2 - dataParsed.parsedData[p][1] * data.graphheight[dataParsed.parsedData[p][4]];
			const GLfloat x2 = data.xgraphcoordinates[dataParsed.parsedData[p][4]] - data.graphwidth[dataParsed.parsedData[p][4]] / 2 + dataParsed.parsedData[p][2] * data.graphwidth[dataParsed.parsedData[p][4]];
			const GLfloat y2 = data.ygraphcoordinates[dataParsed.parsedData[p][4]] + data.graphheight[dataParsed.parsedData[p][4]] / 2 - dataParsed.parsedData[p][3] * data.graphheight[dataParsed.parsedData[p][4]];

			glRectf(x1, y1, x2, y2);

			// We need to draw a dotted line from x1, y1 to x1, y2 and another one from x1, y1, to x2, y1
			// Due to potential incompatibilities with dictating line width, we can use thin polygones as a workaround
			GLfloat lineThickness = 3.0f;
			GLfloat dashSpace = 10.0f;
			GLfloat currentColor[4];
			glGetFloatv(GL_CURRENT_COLOR, currentColor);
			glColor4f(currentColor[0], currentColor[1], currentColor[2], 1.0f);

			// Left dashed line, from top left
			bool drawLeftColor = true;
			const GLfloat x1Backup = x1;
			for (GLfloat y = y1; y >= y2; y -= dashSpace) {
				if (y < y2) {
					y = y2;
				}

				// We need to sample points on either side of the line and only draw the dashed line if the two points are different colors
				// this is going to be super inefficient
				int pointClass = getClassNumFromPoint(x1, y, p);

				// Check if of the same class
				if (pointClass == -2 || pointClass == classNumber) {
					// don't draw if our test point is not a member of any background rectangle.
					// this means that we are on the left or the right edge
					// don't draw if the adaject class is the same as our current class
					continue;
				}

				if (drawLeftColor) {
					if (pointClass == -1) {
						glColor4ub(0, 0, 0, 255);
					}
					else {
						std::vector<float> pointClassColor = { data.classColor[pointClass][0], data.classColor[pointClass][1], data.classColor[pointClass][2] };
						std::vector<float> pointClassColorHSL = RGBtoHSL(pointClassColor);
						pointClassColorHSL[2] *= backgroundClassColorCoefficient;
						std::vector<GLubyte> pointClassColorRGBModified = HSLtoRGB(pointClassColorHSL);
						glColor4ub(pointClassColorRGBModified[0], pointClassColorRGBModified[1], pointClassColorRGBModified[2], 255);

					}
				}
				else {
					if (classNumber == -1) {
						glColor4ub(0, 0, 0, 255);
					}
					else {
						glColor3f(currentColor[0], currentColor[1], currentColor[2]);
					}
				}

				drawLeftColor = !drawLeftColor;

				glBegin(GL_POLYGON);
				glVertex2f(x1 + lineThickness / 2, y);
				glVertex2f(x1 - lineThickness / 2, y);
				if (y - dashSpace <= y2) {
					glVertex2f(x1 - lineThickness / 2, y2);
					glVertex2f(x1 + lineThickness / 2, y2);
				}
				else {
					glVertex2f(x1 - lineThickness / 2, y - dashSpace);
					glVertex2f(x1 + lineThickness / 2, y - dashSpace);
				}
				glEnd();
			}

			bool drawTopColor = true;
			// Draw bottom line
			for (GLfloat x = x1; x <= x2; x += dashSpace) {
				if (x > x2) {
					x = x2;
				}

				// We need to sample points on either side of the line and only draw the dashed line if the two points are different colors
				// this is going to be super inefficient
				int pointClass = getClassNumFromPoint(x, y2, p);

				// Check if of the same class
				if (pointClass == -2 || pointClass == classNumber) {
					// don't draw if our test point is not a member of any background rectangle.
					// this means that we are on the left or the right edge
					// don't draw if the adaject class is the same as our current class
					continue;
				}

				if (drawTopColor) {
					if (classNumber == -1) {
						glColor4ub(0, 0, 0, 255);
					} else {
						glColor3f(currentColor[0], currentColor[1], currentColor[2]);
					}
				}
				else {
					if (pointClass == -1) {
						glColor4ub(0, 0, 0, 255);
					}
					else {
						std::vector<float> pointClassColor = { data.classColor[pointClass][0], data.classColor[pointClass][1], data.classColor[pointClass][2] };
						std::vector<float> pointClassColorHSL = RGBtoHSL(pointClassColor);
						pointClassColorHSL[2] *= backgroundClassColorCoefficient;
						std::vector<GLubyte> pointClassColorRGBModified = HSLtoRGB(pointClassColorHSL);
						glColor4ub(pointClassColorRGBModified[0], pointClassColorRGBModified[1], pointClassColorRGBModified[2], 255);
					}
				}

				drawTopColor = !drawTopColor;

				glBegin(GL_POLYGON);
				glVertex2f(x, y2 + lineThickness / 2);
				glVertex2f(x, y2 - lineThickness / 2);
				if (x + dashSpace >= x2) {
					glVertex2f(x2, y2 - lineThickness / 2);
					glVertex2f(x2, y2 + lineThickness / 2); 
				}
				else {
					glVertex2f(x + dashSpace, y2 - lineThickness / 2);
					glVertex2f(x + dashSpace, y2 + lineThickness / 2);
				}

				glEnd();
			}
		}
	}

	// Draws rectangle if rectangle mode is enabled
	if (isRectangleMode) {
		drawRectangle();
	}


	/* Plots the data. Outer loop for each dimension. Inner loop for data across each graph.  */
	if (data.classsize > 1)
	{
		int g;
		for (int i = 0; i < data.xdata.size(); i++) 
		{
			g =  1;
			for (int j = 0; j < data.classsize - 1; j++)
			{
				if (data.classNum[i] != (data.classToDisplayOnTop ))
				{
					drawData(data.xgraphcoordinates[j], data.ygraphcoordinates[j],
						data.xgraphcoordinates[j + 1], data.ygraphcoordinates[j + 1], i, j); //((1 + i) % data.xdata.size());
				}


			}

		}

		for (int i = 0; i < data.xdata.size(); i++)
		{
			g = 1;
			for (int j = 0; j < data.classsize - 1; j++)
			{
				if (data.classNum[i] == (data.classToDisplayOnTop) )
				{
					drawData(data.xgraphcoordinates[j], data.ygraphcoordinates[j],
						data.xgraphcoordinates[j + 1], data.ygraphcoordinates[j + 1], i, j); //((1 + i) % data.xdata.size());
				}

			}

		}
	}
	else
	{
		for (int i = 0; i < data.xdata.size(); i++) {

			for (int j = 0; j < data.classsize; j++)
			{
				drawData(data.xgraphcoordinates[data.classsize -1-j], data.ygraphcoordinates[j],
					data.xgraphcoordinates[j+1], data.ygraphcoordinates[j+1], i, j);
			}
		}
	}


	data.drawLabels();
	
}

bool InteractiveSPC::doPointsIntersectRectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{	
	GLfloat boundingBoxLeft = min(rectX1, rectX2);
	GLfloat boundingBoxTop = max(rectY1, rectY2);
	GLfloat boundingBoxRight = max(rectX1, rectX2);
	GLfloat boundingBoxBottom = min(rectY1, rectY2);
	bool* startPointCode = getPointTrivialityCode(x1, y1, rectX1, rectY1, rectX2, rectY2);
	bool* endPointCode = getPointTrivialityCode(x2, y2, rectX1, rectY1, rectX2, rectY2);
	bool result = false;

	int lineTriviality = isLineTrivial(startPointCode, endPointCode);

	switch (lineTriviality) {
	case 0:
		// Line is completely within rectangle
		result =  true;
		break;
	case 1:
		// trivial reject
		break;
	case 2:
		// Line intersects rectangle
		result = true;
		break;
	default:
		break;
	}

	return result;
}

int InteractiveSPC::isLineTrivial(bool* startPointTriviality, bool* endPointTriviality) {
	bool startPointCode = !startPointTriviality[0] &&
		!startPointTriviality[1] &&
		!startPointTriviality[2] &&
		!startPointTriviality[3];

	bool endPointCode = !endPointTriviality[0] &&
		!endPointTriviality[1] &&
		!endPointTriviality[2] &&
		!endPointTriviality[3];

	if (startPointCode && endPointCode) {
		return 0;
	}

	for (int i = 0; i < 4; i++) {
		if (startPointTriviality[i] && endPointTriviality[i]) {
			return 1;
		}
	}

	return 2;
}

bool* InteractiveSPC::getPointTrivialityCode(GLfloat px, GLfloat py, GLfloat rectX1, GLfloat rectY1, GLfloat rectX2, GLfloat rectY2) {
	bool* triviality = new bool[] {false, false, false, false};
	GLfloat boundingBoxLeft = min(rectX1, rectX2);
	GLfloat boundingBoxTop = max(rectY1, rectY2);
	GLfloat boundingBoxRight = max(rectX1, rectX2);
	GLfloat boundingBoxBottom = min(rectY1, rectY2);

	if (px < boundingBoxLeft) {
		triviality[0] = true;
	}
	if (py > boundingBoxTop) {
		triviality[1] = true;
	}
	if (px > boundingBoxRight) {
		triviality[2] = true;
	}
	if (py < boundingBoxBottom) {
		triviality[3] = true;
	}

	return triviality;
}


// Dragging Graphs
float InteractiveSPC::findClickedGraph(double x, double y) 
{
	for (int i = 0; i < data.classsize; i++) 
	{
		//for (int i1 = 0; i1 < data.graphwidth.size(); i1++)
		//{
			if (data.xgraphcoordinates[i] + data.pan_x - (data.graphwidth[i] / 2) <= x &&
				data.xgraphcoordinates[i] + data.pan_x + (data.graphwidth[i] / 2) >= x &&
				data.ygraphcoordinates[i] + data.pan_y - (data.graphheight[i] / 2) <= y &&
				data.ygraphcoordinates[i] + data.pan_y + (data.graphheight[i] / 2) >= y)
			{

				return i;
			}
		//}

		//original code
		
	} return -1;
}

int InteractiveSPC::findClickedCoordinate(double x, double y)
{
	for (int i = 0; i < data.classsize; i++)
	{

			if (x <= (data.ygraphcoordinates[i] - 150) && x>= (data.ygraphcoordinates[i]-500))
			{
				return 0;
			}

			else if (data.xgraphcoordinates[i] - 25 <= x &&
				data.xgraphcoordinates[i] + 25 >= x)
			{
				return 1;
			}

		
	} return -1;
}

void InteractiveSPC::drawCircle(int x, int y)
{
	//Drawing circles to mark begining of the dataset
	GLUquadricObj* g_normalObject = NULL;
	g_normalObject = gluNewQuadric();
	glMateriali(GL_FRONT_AND_BACK, GL_SMOOTH, rand() % 128);
	//glColor3f(1, 0, 0);
	glTranslatef(x, y, 0);
	//glRotatef(10.0f, 0.0, 1.0, 0.0);
	gluDisk(g_normalObject, 5, 6, 10, 1);
	
	//for (int i = 0; i < numOfClasses; i++) {
	//	classColor.push_back({});
	//	//0, (data.classNum[i] * 50) + 1, 100
	//	classColor[i].push_back(0);
	//	classColor[i].push_back((i + 1) * 50 + 1);
	//	classColor[i].push_back(100);
	//	classTransparency.push_back(255);
	//}

}

void InteractiveSPC::drawRectangle(float rect_x1, float rect_x2, float rect_y1, float rect_y2, float r, float g, float b)
{
	glBegin(GL_LINE_STRIP);
	//glLineWidth(10);

	
	glColor3ub(r, g, b);
   // glVertex2f(); // origin of the line
    glVertex2f(rect_x1, rect_y1);
	glVertex2f(rect_x2, rect_y1);	
	glVertex2f(rect_x2, rect_y2); // origin of the line
	glVertex2f(rect_x1, rect_y2);
	glVertex2f(rect_x1, rect_y1);
	glEnd();

}

void InteractiveSPC::drawRectangle() {
	drawRectangle(rectX1, rectX2, rectY1, rectY2, 0.0f, 0.0f, 0.0f);
}

/* Display Utilities*/

std::vector<float> InteractiveSPC::RGBtoHSL(std::vector<float> classColor) {
	std::vector<float> hsl;
	float rprime = classColor[0] / 255.0;
	float gprime = classColor[1] / 255.0;
	float bprime = classColor[2] / 255.0;
	float cMax = max(max(rprime, gprime), bprime);
	float cMin = min(min(rprime, gprime), bprime);
	float cDelta = cMax - cMin;
	float saturation, lightness = 0;
	int hue = 0;

	// get hue
	if (cDelta == 0) {
		hue = 0;
	}else if (cMax == rprime) {
		hue = 60 * fmod(((gprime - bprime) / cDelta), 6);
	}
	else if (cMax == gprime) {
		hue = 60 * ((bprime - rprime) / cDelta + 2);
	}
	else if (cMax == bprime) {
		hue = 60 * ((rprime - gprime) / cDelta + 4);
	}

	// get lightness
	lightness = (cMax + cMin) / 2;

	// get saturation
	if (cDelta == 0) {
		saturation = 0;
	} else {
		saturation = cDelta / (1 - abs(2 * lightness - 1));
	}

	if (saturation > 1.0f) {
		saturation = 1.0;
	}

	hsl.push_back(hue);
	hsl.push_back(saturation);
	hsl.push_back(lightness);
	return hsl;
}

bool InteractiveSPC::isPointWithinRect(GLfloat px, GLfloat py, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
	bool result = false;
	if (px <= x2 && px >= x1) {
		if (py <= y1 && py >= y2) {
			result = true;
		}
	}
	return result;
}

int InteractiveSPC::getClassNumFromPoint(GLfloat px, GLfloat py, int currentDataIndex) {
	int dataIndex = -1;

	for (int i = 0; i < dataParsed.parsedData.size(); i++) {
		if (i == currentDataIndex) continue;
		GLfloat testRectx1 = data.xgraphcoordinates[dataParsed.parsedData[i][4]] - data.graphwidth[dataParsed.parsedData[i][4]] / 2 + dataParsed.parsedData[i][0] * data.graphwidth[dataParsed.parsedData[i][4]];
		GLfloat testRecty1 = data.ygraphcoordinates[dataParsed.parsedData[i][4]] + data.graphheight[dataParsed.parsedData[i][4]] / 2 - dataParsed.parsedData[i][1] * data.graphheight[dataParsed.parsedData[i][4]];
		GLfloat testRectx2 = data.xgraphcoordinates[dataParsed.parsedData[i][4]] - data.graphwidth[dataParsed.parsedData[i][4]] / 2 + dataParsed.parsedData[i][2] * data.graphwidth[dataParsed.parsedData[i][4]];
		GLfloat testRecty2 = data.ygraphcoordinates[dataParsed.parsedData[i][4]] + data.graphheight[dataParsed.parsedData[i][4]] / 2 - dataParsed.parsedData[i][3] * data.graphheight[dataParsed.parsedData[i][4]];

		if (isPointWithinRect(px, py, testRectx1, testRecty1, testRectx2, testRecty2)) {
			dataIndex = i;
			break;
		}
	}

	if (dataIndex == -1) {
		return -2;
	}
	else {
		return dataParsed.parsedData[dataIndex][5];
	}
}

std::vector<GLubyte> InteractiveSPC::HSLtoRGB(std::vector<float> hsl) {

	std::vector<float> rgbPrimeVector;
	std::vector<GLubyte> rgbVector;
	float hue = hsl[0];
	float saturation = hsl[1];
	float lightness = hsl[2];

	float c = (1 - abs(2 * lightness - 1)) * saturation;
	float x = c * (1 - abs(fmod((hue / 60.0), 2) - 1));
	float m = lightness - (c / 2.0);

	if (hue >= 0 && hue < 60) {
		rgbPrimeVector.push_back(c);
		rgbPrimeVector.push_back(x);
		rgbPrimeVector.push_back(0);
	}
	else if (hue >= 60 && hue < 120) {
		rgbPrimeVector.push_back(x);
		rgbPrimeVector.push_back(c);
		rgbPrimeVector.push_back(0);
	} else if (hue >= 120 && hue < 180) {
		rgbPrimeVector.push_back(0);
		rgbPrimeVector.push_back(c);
		rgbPrimeVector.push_back(x);
	} else if (hue >= 180 && hue < 240) {
		rgbPrimeVector.push_back(0);
		rgbPrimeVector.push_back(x);
		rgbPrimeVector.push_back(c);
	} else if (hue >= 240 && hue < 300) {
		rgbPrimeVector.push_back(x);
		rgbPrimeVector.push_back(0);
		rgbPrimeVector.push_back(c);
	} else if (hue >= 300 && hue < 360) {
		rgbPrimeVector.push_back(c);
		rgbPrimeVector.push_back(0);
		rgbPrimeVector.push_back(x);
	}

	rgbVector.push_back((rgbPrimeVector[0] + m) * 255);
	rgbVector.push_back((rgbPrimeVector[1] + m) * 255);
	rgbVector.push_back((rgbPrimeVector[2] + m) * 255);

	std::cout << "debug" << std::endl;

	return rgbVector;
}