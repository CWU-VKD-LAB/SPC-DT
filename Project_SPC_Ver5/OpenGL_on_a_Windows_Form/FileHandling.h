// File Handler

#include "stdafx.h"
#include "ClassData.h"
#pragma once


class FileHandling
{
public:

	FileHandling();

	void openFile(ClassData&);
	void openParserFile(parseData&, ClassData&);
	void sortGraphNotBasedOnParser(ClassData&);
	void sortGraphBasedOnParser(ClassData&);
	void normalizeData(ClassData &data);

};

