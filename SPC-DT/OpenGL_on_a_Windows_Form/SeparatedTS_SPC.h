///-------------------------------------------------------------------------------------------------
// file:	SeparatedTS_SPC.h
//
// summary:	Declares the class for a separated time series shifted paired coordinate graph.
///-------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "ClassData.h"
#include "FileHandling.h"

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A separated time series shifted paired coordinate graph. </summary>
/// <author>	Mahlet Saketa			</author>
/// <editor>	Paul Collet, Allyn Vo	</editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

class SeparatedTS_SPC
{

public:
	int columns = 5;

	float linecolor[3] = { 255,0,0 }; // Connecting line color, default to red
	void setLineColor(float R, float G, float B) {
		linecolor[0] = R;
		linecolor[1] = G;
		linecolor[2] = B;
	}

	/// <summary>	The xdatatemp. </summary>
	std::vector<float> xdatatemp;
	/// <summary>	The ydatatemp. </summary>
	std::vector<float> ydatatemp;
	/// <summary>	Holds coords for the distance based time series. </summary>
	std::vector<std::vector<float> > distanceX;
	/// <summary>	The distance y coordinate. </summary>
	std::vector<std::vector<float> > distanceY;

	/// <summary>	The converted values. </summary>
	std::vector<std::vector<float> > convertedValues;

	/// <summary>	The data. </summary>
	ClassData data;
	/// <summary>	The new file. </summary>
	FileHandling newFile;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Default constructor. </summary>
	///
	/// <remarks>	Barim, 3/3/2019. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SeparatedTS_SPC() {};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constructor. </summary>
	///
	/// <remarks>	Barim, 3/3/2019. </remarks>
	///
	/// <param name="parameter1">	[in,out] The first parameter. </param>
	/// <param name="parameter2">	The second parameter. </param>
	/// <param name="parameter3">	The third parameter. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SeparatedTS_SPC(ClassData&, double, double);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Normalize data. </summary>
	///
	/// <remarks>	Barim, 3/3/2019. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void normalizeData();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sort graph 2. </summary>
	///
	/// <remarks>	Barim, 3/3/2019. </remarks>
	///
	/// <param name="parameter1">	[in,out] The first parameter. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void sortGraph2(ClassData&);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Graph by pairs. </summary>
	///
	/// <remarks>	Barim, 3/3/2019. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void graphByPairs();


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Resets the distance. </summary>
	///
	/// <remarks>	Barim, 3/3/2019. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void resetDist();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Fill graph locations. </summary>
	///
	/// <remarks>	Barim, 3/3/2019. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void fillGraphLocations();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Draw data. </summary>
	///
	/// <remarks>	Barim, 3/3/2019. </remarks>
	///
	/// <param name="x1">	   	The first x value. </param>
	/// <param name="y1">	   	The first y value. </param>
	/// <param name="index">   	Zero-based index of the. </param>
	/// <param name="curClass">	The current class. </param>
	/// <param name="j">	   	An int to process. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void drawData(float x1, float y1, int index, int curClass, int j);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Displays this object. </summary>
	///
	/// <remarks>	Barim, 3/3/2019. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void display();

	/* DRAGGING GRAPH STUFF */
	/// <summary>	If the mouse is clicked, held and dragging. </summary>
	bool dragging = false;
	/// <summary>	The graph clickedi. </summary>
	int graphClickedi = -1;
	/// <summary>	The graph clickedj. </summary>
	int graphClickedj = -1;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Searches for the first clicked graph. </summary>
	///
	/// <remarks>	Barim, 3/3/2019. </remarks>
	///
	/// <param name="x">	The x coordinate. </param>
	/// <param name="y">	The y coordinate. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void findClickedGraph(double x, double y);
};


