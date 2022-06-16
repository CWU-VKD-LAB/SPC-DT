///-------------------------------------------------------------------------------------------------
// file:	Cont_Parallel_Coordinates.h
//
// summary:	Declares the container parallel coordinates class
///-------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "ClassData.h"
#include "FileHandling.h"

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A container parallel coordinates. </summary>
///
/// <author>	Mahlet Saketa			</author>
/// <editor>	Paul Collet, Allyn Vo	</editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

class Cont_Parallel_Coordinates
{
public:
	/// <summary>	The colors. </summary>
	/// <summary>	The colors. </summary>
	float colors;
	/// <summary>	The red. </summary>
	std::vector<float> red;
	/// <summary>	The green. </summary>
	std::vector<float> green;
	/// <summary>	The blue. </summary>
	std::vector<float> blue;
	/// <summary>	The temp[ 30]. </summary>
	float temp[30];
	/// <summary>	The xdatatemp. </summary>
	std::vector<float> xdatatemp;
	/// <summary>	The ydatatemp. </summary>
	std::vector<float> ydatatemp;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Default constructor. </summary>
	///
	/// <author>	Mahlet Saketa			</author>
	/// <editor>	Paul Collet, Allyn Vo	</editor>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Cont_Parallel_Coordinates();

	/// <summary>	The data. </summary>
	ClassData data;
	/// <summary>	The new file. </summary>
	FileHandling newFile;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Fill graph locations. </summary>
	///
	/// <author>	Mahlet Saketa			</author>
	/// <editor>	Paul Collet, Allyn Vo	</editor>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void fillGraphLocations();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Draw data. </summary>
	///
	/// <author>	Mahlet Saketa			</author>
	/// <editor>	Paul Collet, Allyn Vo	</editor>
	///
	/// <param name="x1">	The first x value. </param>
	/// <param name="y1">	The first y value. </param>
	/// <param name="i"> 	Zero-based index of the. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void drawData(float x1, float y1, int i);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Displays this object. </summary>
	///
	/// <author>	Mahlet Saketa			</author>
	/// <editor>	Paul Collet, Allyn Vo	</editor>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void display();


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Clears the fields. </summary>
	///
	/// <author>	Mahlet Saketa			</author>
	/// <editor>	Paul Collet, Allyn Vo	</editor>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void clearFields();
	void sortGraph(ClassData& data);

	void normalizeData();

};

	