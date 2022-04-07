///-------------------------------------------------------------------------------------------------
// file:	InteractiveSPC.h
//
///-------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "ClassData.h"
#include "FileHandling.h"
#pragma once

class InteractiveSPC
{

public:
	
	ClassData data;
	parseData dataParsed;
	FileHandling newFile;
	float backgroundTransparency = 100; // Default value
	float backgroundClassColorCoefficient = 1.0; // Default value
	std::vector<int> plotDrawOrder;
	std::map<float, std::map<float, std::map<int, std::vector<int>>>> overlapMap;

	InteractiveSPC() {};


	InteractiveSPC(ClassData&, parseData&, double, double);

	void fillPlotLocations();

	/* Drawing data sets. */

	std::vector<int> pointsToRender;

	void drawData(float x1, float  y1, float  x2, float  y2, int i, int j);
	int drawData(float x1, float  y1, int recordNum, int plotNum);

	void display();

	// Calculates termination nodes for each datapoint
	void calculateDataTerminationPoints();

	// *** USED FOR DRAWING LINE COLORS
	int findBackgroundClassOfPoint(GLfloat px, GLfloat py);
	int findBackgroundClassOfPoint(GLfloat px, GLfloat py, int plotNum);
	int findPlotNumOfPoint(GLfloat px, GLfloat py);
	bool shouldPointTerminate(GLfloat px, GLfloat py);

	// *** USED FOR DRAWING RECTANGLES AND IDENTIFY WHICH LINES INTERSECT IT
	std::vector<GLfloat> rectX1List;
	std::vector<GLfloat> rectY1List;
	std::vector<GLfloat> rectX2List;
	std::vector<GLfloat> rectY2List;
	GLfloat rectX1;
	GLfloat rectY1;
	GLfloat rectX2;
	GLfloat rectY2;
	bool isRectangleMode = false;
	int isLineTrivial(bool * startPointTriviality, bool * endPointTriviality);
	bool doPointsIntersectRectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
	bool* getPointTrivialityCode(GLfloat px, GLfloat py, GLfloat rectX1, GLfloat rectY1, GLfloat rectX2, GLfloat rectY2);

	/// *** USED FOR DETERMINING LINE TERMINATION MODE
	bool isLineTerminationMode = false; // TODO: DEBUG TRUE. in the end it will be set to false and controlled by a button
	bool isColorTerminationMode = false;

	/// *** USED FOR OVERLAP MITIGATION MODE
	bool isOverlapMitigationMode = false;

	/// *** USED FOR DETERMINING ATTRIBUTE SWAP MODE
	bool swapAttributeAxesMode = false;
	std::set<int> swappedPlots;
	std::set<int> plotsWithXAxisInverted;
	std::set<int> plotsWithYAxisInverted;

	/// *** USED TO DETERMINE IF INCOMING PARSER BRANCHES
	bool doesParserBranch = false;
	
	/// *** USED FOR DETERMINING MISCLASSIFICATION HIGHLIGHTING MODE
	bool isHighlightMisclassficationsMode = false;

	/// *** USED FOR DETERMINING POINT COLORING MODE
	bool isPointColorMode = false;

	///*** USED FOR DETERMINING LINE COLORING MODE
	bool isLineColorMode = false;

	// *** STUFF BELOW THIS IS FOR DRAGGING GRAPHS *** //////////////////////////////////////////

	/* DRAGGING GRAPH STUFF */
	/// <summary>	If the mouse is clicked, held and dragging. </summary>
	bool dragging = false;
	/// <summary>	Holds what graphs being clicked. </summary>
	int plotNumClicked = -1;
	int coordinatesClicked = -1; //Wagle

	void updatePlotLocation(double mouseX, double mouseY, int plotNum);

	/* This method takes the passed mouse click coordinates and finds the graph clicked on. */
	float findClickedGraph(double x, double y);
	int findClickedCoordinate(double x, double y);
	void drawCircle(int x, int y);
	void drawRectangle();
	void drawRectangle(float rect_x1, float rect_x2, float rect_y1, float rect_y2, float r, float g, float b);
	void drawRectanglesOnGray();
	void setBackgroundTransparency(float alpha);
	void setBackgroundColorLightness(float lightness);
	std::vector<float> RGBtoHSL(std::vector<float> classColor);
	std::vector<GLubyte> HSLtoRGB(std::vector<float> hsl);
	bool isPointWithinRect(GLfloat px, GLfloat py, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
	bool shouldLineBeClipped(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, bool* startPointCode, bool* endPointCode);
	int getClassNumFromPoint(GLfloat px, GLfloat py, int currentDataIndex);
};

