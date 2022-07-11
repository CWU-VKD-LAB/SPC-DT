///-------------------------------------------------------------------------------------------------
// file:	InteractiveSPC.h
//
///-------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "ClassData.h"
#include "FileHandling.h"
#include "UserRectangle.h"
#include "Zone.h"
#include "ColorUtils.h"
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

	//void fillPlotLocations();

	/* Drawing data sets. */

	std::vector<int> pointsToRender;

	// void drawData(float x1, float  y1, float  x2, float  y2, int i, int j);
	int drawData(float x1, float  y1, int recordNum, int plotNum);

	void display();

    // utility functions
    std::vector<int> getParserElementsWithPlotNum(int plotNum);

	// Calculates termination nodes for each datapoint
	//void calculateDataTerminationPoints();

	// *** USED FOR DRAWING LINE COLORS
	//int findBackgroundClassOfPoint(GLfloat px, GLfloat py);
	int findBackgroundClassOfPoint(GLfloat px, GLfloat py, int plotNum);
	int findBackgroundZoneIdOfPoint(GLfloat px, GLfloat py, int plotNum);
	int findPlotNumOfPoint(GLfloat px, GLfloat py);
	bool shouldPointTerminate(GLfloat px, GLfloat py);

	// *** USED FOR DRAWING RECTANGLES AND IDENTIFY WHICH LINES INTERSECT IT
	//std::vector<GLfloat> condenseRectX1List;
	//std::vector<GLfloat> condenseRectY1List;
	//std::vector<GLfloat> condenseRectX2List;
	//std::vector<GLfloat> condenseRectY2List;
    std::vector<UserRectangle> userRectangles;
	void condensePointInRectangle(float& px, float& py, int& caseClass, UserRectangle& userRect);
	void adjustPointToRectangle(float& x, float& y, int& caseClass);
	void deleteSelectedRectangle();
	void updateSelectedRectangleType(int state);
	UserRectangle* findClickedRectangle(GLfloat px, GLfloat py);
	UserRectangle* selectedRect;
	bool isSelectUserRectangleMode = false;

    //std::vector<std::vector<GLubyte>> userRectColorList;
	//GLfloat rectX1;
	//GLfloat rectY1;
	//GLfloat rectX2;
	//GLfloat rectY2;
	bool isCondenseRectangleMode = false;
	bool isUserRectangleMode = false;
	bool isBackgroundDensityColoringMode = false;
	bool drawingUserRectangleVertex1 = false;
	GLfloat userRectangleDrawGuideX;
	GLfloat userRectangleDrawGuideY;
	int isLineTrivial(bool * startPointTriviality, bool * endPointTriviality);
	//bool doPointsIntersectRectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
	bool* getPointTrivialityCode(GLfloat px, GLfloat py, GLfloat rectX1, GLfloat rectY1, GLfloat rectX2, GLfloat rectY2);

	/// *** USED FOR DETERMINING LINE TERMINATION MODE
	bool isLineTerminationMode = false; // TODO: DEBUG TRUE. in the end it will be set to false and controlled by a button
	bool isColorTerminationMode = false;

	/// *** USED FOR OVERLAP MITIGATION MODE
	bool isOverlapMitigationMode = false;
	bool isOverlapMitigationModeAll = false;
	void mitigateOverlap(float& x, float& y, int& caseNum, int& caseClass, int& plotNum, int& pointBackgroundClass, int& pointBackgroundZone);
    /// *** USED FOR ADJUSTING DECISION ZONE THRESHOLDS
	const int selectionZoneWidth = 10;
    bool isAdjustThresholdsMode = false;
    std::vector<std::vector<std::vector<float>>> thresholdEdgeSelectionZones;
    std::map<std::vector<float>, int> edgeToParserElementIndex; // map: collection of edge points -> list [ zoneId, index for (x1, y1, x2, y2) ]
    std::set<int> zoneIdThresholdEdgesRecorded;
    std::vector<int> findClickedEdge(GLfloat px, GLfloat py);
	void recomputePlotZones(int& plotNum);
	std::vector<int> clickedEdge;
	std::vector<Zone> plotZones;
	std::map<int, Zone> zoneIdMap;
	void buildZoneEdges();
	void updateZoneColors(int classNum);
	bool zonesNotBuilt = true;

	/// *** USED FOR DETERMINING ATTRIBUTE SWAP MODE
	bool swapAttributeAxesMode = false;
    void invertPlotNum(int plotNum, bool isXAxist);
	void initializePlots();
	std::set<int> swappedPlots;
	std::set<int> plotsWithXAxisInverted;
	std::set<int> plotsWithYAxisInverted;

	/// *** USED TO DETERMINE IF INCOMING PARSER BRANCHES
	bool doesParserBranch = false;
	
	/// *** USED FOR DETERMINING MISCLASSIFICATION HIGHLIGHTING MODE
	bool isHighlightMisclassficationsMode = false;
	void handleMisclassifications(float& x, float& y, int& caseClass, int& caseNum, int& pointBackgroundClass);

	/// *** USED FOR DETERMINING POINT COLORING MODE
	bool isPointColorMode = false;

	///*** USED FOR DETERMINING LINE COLORING MODE
	bool isLineColorMode = false;

	// *** STUFF BELOW THIS IS FOR DRAGGING GRAPHS *** //////////////////////////////////////////

	/* DRAGGING GRAPH STUFF */
	/// <summary>	If the mouse is clicked, held and dragging. </summary>
	bool dragging = false;

	/// <summary>	If the mouse is clicked and held on an edge, we are adjusting it
	bool thresholdBeingAdjusted = false;

	/// <summary>	Holds what graphs being clicked. </summary>
	int plotNumClicked = -1;
	int zoneIdClicked = -1;
	int coordinatesClicked = -1; //Wagle

	void updatePlotLocation(double mouseX, double mouseY, int plotNum);

    /* This is for worst performing area */
    int worstZoneNum = -1;;
    float worstZoneNumDensity = INT32_MAX;
	bool isHighlightWorstAreaMode = false;
    void drawWorstZone();

	/* This method takes the passed mouse click coordinates and finds the graph clicked on. */
	float findClickedGraph(double x, double y);
	int findClickedCoordinate(double x, double y);
	void drawCircle(int x, int y);
	//void drawCondenseRectangles();
	//void drawUserRectangles();
	void drawRectangle(UserRectangle rectToDraw);
	void drawRectangle(float rect_x1, float rect_x2, float rect_y1, float rect_y2, float r, float g, float b);
	void drawRectanglesOnGray();
	void setBackgroundTransparency(float alpha);
	void setBackgroundColorLightness(float lightness);
	//std::vector<float> RGBtoHSL(std::vector<float> classColor);
	//std::vector<GLubyte> HSLtoRGB(std::vector<float> hsl);
	bool isPointWithinRect(GLfloat px, GLfloat py, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
	//bool shouldLineBeClipped(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, bool* startPointCode, bool* endPointCode);
	//int getClassNumFromPoint(GLfloat px, GLfloat py, int currentDataIndex);

	float computeBackgroundTransparency(Zone &zone);
	void computeZoneEdges(Zone &zone);
};

