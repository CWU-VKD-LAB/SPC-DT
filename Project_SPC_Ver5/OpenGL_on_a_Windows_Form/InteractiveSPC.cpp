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
		

	int classnum = data.classNum[i] - 1;

	glPushMatrix();	// Makes a new layer

	glTranslatef(x1 + data.pan_x, y1 + data.pan_y, 0); // Translates starting position to draw
	if (data.showHideLinesVar)
	{
		glBegin(GL_LINE_STRIP);
		//data.classTransparency[1] = 70;
		glColor4ub(data.classColor[classnum][0], data.classColor[classnum][1], data.classColor[classnum][2], data.dataTransparency[i]);
		glVertex2f(x1Coord, y1Coord);
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
				glColor4ub(169, 169, 169, backgroundTransparency);
			}
			else {

				std::vector<float> hsl = RGBtoHSL(data.classColor[classNumber]);

				hsl[2] = hsl[2] * backgroundClassColorCoefficient;

				std::vector<GLubyte> rgb = HSLtoRGB(hsl[0], hsl[1], hsl[2]);
				
				glColor4ub(rgb[0], rgb[1], rgb[2], backgroundTransparency);
			}



			glRectf(data.xgraphcoordinates[dataParsed.parsedData[p][4]] - data.graphwidth[dataParsed.parsedData[p][4]] / 2 + dataParsed.parsedData[p][0] * data.graphwidth[dataParsed.parsedData[p][4]],
				data.ygraphcoordinates[dataParsed.parsedData[p][4]] + data.graphheight[dataParsed.parsedData[p][4]] / 2 - dataParsed.parsedData[p][1] * data.graphheight[dataParsed.parsedData[p][4]],
				data.xgraphcoordinates[dataParsed.parsedData[p][4]] - data.graphwidth[dataParsed.parsedData[p][4]] / 2 + dataParsed.parsedData[p][2] * data.graphwidth[dataParsed.parsedData[p][4]],
				data.ygraphcoordinates[dataParsed.parsedData[p][4]] + data.graphheight[dataParsed.parsedData[p][4]] / 2 - dataParsed.parsedData[p][3] * data.graphheight[dataParsed.parsedData[p][4]]);
		}
			
			

				
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

std::vector<GLubyte> InteractiveSPC::HSLtoRGB(float hue, float saturation, float lightness) {

	std::vector<float> rgbPrimeVector;
	std::vector<GLubyte> rgbVector;

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