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


	InteractiveSPC() {};


	InteractiveSPC(ClassData&, parseData&, double, double);


	void fillGraphLocations();

	/* Drawing data sets. */


	void drawData(float x1, float  y1, float  x2, float  y2, int i, int j);


	void display();

	// *** STUFF BELOW THIS IS FOR DRAGGING GRAPHS *** //////////////////////////////////////////

	/* DRAGGING GRAPH STUFF */
	/// <summary>	If the mouse is clicked, held and dragging. </summary>
	bool dragging = false;
	/// <summary>	Holds what graphs being clicked. </summary>
	int graphClicked = -1;
	int coordinatesClicked = -1; //Wagle

	/* This method takes the passed mouse click coordinates and finds the graph clicked on. */
	float findClickedGraph(double x, double y);
	int findClickedCoordinate(double x, double y);
	void drawCircle(int x, int y);
	void drawRectangle(float rect_x1, float rect_x2, float rect_y1, float rect_y2, float r, float g, float b);
};

