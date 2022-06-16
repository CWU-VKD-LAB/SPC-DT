///-------------------------------------------------------------------------------------------------
// file:	SeparatedCPC.h
//
// summary:	Declares the separated cpc class
///-------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "ClassData.h"
#include "FileHandling.h"

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A separated cpc. </summary>
///
/// <author> Phillipe Shin & Mahlet Saketa 02/21/2019 </author>
/// <editor> Paul Collet </editor>
////////////////////////////////////////////////////////////////////////////////////////////////////

class SeparatedCPC
{

public:
	/// <summary>	Class data. </summary>
	ClassData data;
	/// <summary>	The new file. </summary>
	FileHandling newFile;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Default constructor. </summary>
	///
	/// <author> Phillipe Shin & Mahlet Saketa 02/21/2019 </author>
	/// <editor> Paul Collet </editor>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SeparatedCPC() {};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constructor. </summary>
	///
	/// <author> Phillipe Shin & Mahlet Saketa 02/21/2019 </author>
	/// <editor> Paul Collet </editor>
	///
	/// <param name="parameter1">	[in,out] The first parameter. </param>
	/// <param name="parameter2">	The second parameter. </param>
	/// <param name="parameter3">	The third parameter. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SeparatedCPC(ClassData&, double, double);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Fill graph locations. </summary>
	///
	/// <author> Phillipe Shin & Mahlet Saketa 02/21/2019 </author>
	/// <editor> Paul Collet </editor>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void fillGraphLocations();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Draw data. </summary>
	///
	/// <author> Phillipe Shin & Mahlet Saketa 02/21/2019 </author>
	/// <editor> Paul Collet </editor>
	///
	/// <param name="x1">   	The first x value. </param>
	/// <param name="y1">   	The first y value. </param>
	/// <param name="index">	Zero-based index of the. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void drawData(float x1, float y1, int index, int curClass);
	void display();

};

