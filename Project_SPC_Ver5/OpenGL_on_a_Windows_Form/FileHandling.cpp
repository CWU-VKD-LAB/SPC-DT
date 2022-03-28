///-------------------------------------------------------------------------------------------------
// file:	FileHandling.cpp
//
// summary:	Implements the file handling class
///-------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "FileHandling.h"
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

FileHandling::FileHandling() {}

// Input: Contents of input.csv -- Opens the file and reads the values into matrix values

void FileHandling::openFile(ClassData &data)
{
	using namespace std;
	ifstream myFile;
	string line;

	myFile.open(data.fileName);											// Open the file

	if (myFile.is_open()) {												/* Valid File: Begin reading data form the file */
		vector<std::string> columns;

		while (getline(myFile, line)) {									/* While the file has more data */
			istringstream cell(line);
			string distance;
			while (getline(cell, distance, ',')) {
				columns.push_back(distance);
			}
			data.values.push_back(columns);								// Store data
			columns.clear();
		}

	}
	myFile.close();
}



void FileHandling::openParserFile(parseData &dataParsed, ClassData &data)
{
	using namespace std;
	ifstream myParserFile;
	string line;
	//int i = 0;
	std::vector<float> temp;
	std::vector<float> coordinatesPlotnumAndClassnum;
	std::vector<std::string> attributePair;
	std::map<int, std::vector<std::string>> attributePairSorter; // weird way to sort things, but it'll work
	std::vector<int> destinationVector;
	int destination;

	myParserFile.open(dataParsed.parserFileName);											// Open the file

	if (myParserFile.is_open()) {												/* Valid File: Begin reading data form the file */
		vector<std::string> columns;

		while (getline(myParserFile, line)) {									/* While the file has more data */
			istringstream cell(line);
			string distance;
			while (getline(cell, distance, ' ')) {
				columns.push_back(distance);
			}
			data.strparsedData.push_back(columns);
			dataParsed.strparsedData.push_back(columns);
			// Store data
			columns.clear();
		}

	}

	// step through each row of data
	std::set<int> seenClasses;
	for (int i = 0; i < data.strparsedData.size(); i++)
	{
		float x1 = stof(data.strparsedData[i][0]);
		float y1 = stof(data.strparsedData[i][1]);
		float x2 = stof(data.strparsedData[i][2]);
		float y2 = stof(data.strparsedData[i][3]);
		int plotNum = stoi(data.strparsedData[i][4]); // doesnt need to be float, but we're putting it in a vector with other floats
		int classNum = stoi(data.strparsedData[i][5]); // doesnt need to be float, but we're putting it in a vector with other floats
		if (seenClasses.find(classNum) == seenClasses.end()) {
			seenClasses.insert(classNum);
			data.classes.push_back(classNum);
		}
		if (classNum < 0) {
			if (!std::count(data.continueElements.begin(), data.continueElements.end(), classNum)) {
				data.continueElements.push_back(classNum);
			}
		}
		string attr1 = data.strparsedData[i][6];
		string attr2 = data.strparsedData[i][7];
        // if current parser element contains a destination, update destination map
		if (data.strparsedData[i].size() == 9) {
			int destination = stoi(data.strparsedData[i][8]);
			data.plotDestinationMap[plotNum][classNum] = destination;
			/*if (data.plotDestinationMap.find(plotNum) == data.plotDestinationMap.end()) {
                data.plotDestinationMap[plotNum][classNum] = destination;
            }
            else {
                data.plotDestinationMap[plotNum].insertnew std::map<int, int>();
                data.plotDestinationMap[plotNum][classNum] = destination;
            }*/
		}
		
		// Add coords, plotnum, classnum
		coordinatesPlotnumAndClassnum.push_back(x1);
		coordinatesPlotnumAndClassnum.push_back(y1);
		coordinatesPlotnumAndClassnum.push_back(x2);
		coordinatesPlotnumAndClassnum.push_back(y2);
		coordinatesPlotnumAndClassnum.push_back(plotNum);
		coordinatesPlotnumAndClassnum.push_back(classNum);

		// Get attribute pair names and strip commas 
		attr1.erase(std::remove(attr1.begin(), attr1.end(), ','), attr1.end());
		attr2.erase(std::remove(attr2.begin(), attr2.end(), ','), attr2.end());

		// if dataparsedAttributePairs does not contain attribute pair
		bool dataDotparsedAttributePairsContainsCurrentPair = false;
		for (int i = 0; i < data.parsedAttributePairs.size(); i++) {
			if (attr1 == data.parsedAttributePairs[i][0] &&
				attr2 == data.parsedAttributePairs[i][1] &&
				std::to_string(plotNum) == data.parsedAttributePairs[i][2])
			{
				dataDotparsedAttributePairsContainsCurrentPair = true;
				break;
			}
		}

		if (!dataDotparsedAttributePairsContainsCurrentPair) {
			attributePair.push_back(attr1);
			attributePair.push_back(attr2);
			attributePair.push_back(std::to_string(plotNum));
			attributePairSorter[plotNum] = attributePair;
		}

		//if (i < data.strparsedData.size() - 1) {
		//	data.plotDestinationList.push_back(i + 1);
		//}
		//// If row is a continue element, get its destination
		//if (data.strparsedData[i].size() > 8) {
		//	destination = stoi(data.strparsedData[i][8]);
		//	coordinatesPlotnumAndClassnum.push_back(destination);
		//	//data.plotDestinationList.push_back(destination);
		//}
		//std::vector<string> attributePair;
		//for (int j = 0; j < data.strparsedData[i].size(); j++)
		//{
		//	if (j < data.strparsedData[i].size() - 2) {
		//		temp.push_back(stof(data.strparsedData[i][j]));
		//	}
		//	else if (j == data.strparsedData.size() - 1) {
		//		string str = data.strparsedData[i][j];
		//		str.erase(std::remove(str.begin(), str.end(), ','), str.end());
		//		attributePair.push_back(str);
		//	}
		//	else {
		//		string str = data.strparsedData[i][j];
		//		str.erase(std::remove(str.begin(), str.end(), ','), str.end());
		//		attributePair.push_back(str);
		//	}
		//	//temp.push_back(stof(dataParsed.strparsedData[i][j]));
		//}

		data.parsedData.push_back(coordinatesPlotnumAndClassnum);
		dataParsed.parsedData.push_back(coordinatesPlotnumAndClassnum);

        //      // TODO: Try to remember what this does
		//bool dataParsedObjectContainsAttributePair = false;
		//for (int i = 0; i < data.parsedAttributePairs.size(); i++) {
		//	if (attributePair[0] == data.parsedAttributePairs[i][0] && attributePair[1] == data.parsedAttributePairs[i][1]) {
		//		dataParsedObjectContainsAttributePair = true;
		//	}
		//}
  //      // When would this be true?
		//if (!dataParsedObjectContainsAttributePair) {
		//	data.parsedAttributePairs.push_back(attributePair);
		//	dataParsed.parsedAttributePairs.push_back(attributePair);
		//}

		std::cout << "debug";
		coordinatesPlotnumAndClassnum.clear();
		attributePair.clear();
	}

	// very odd way to sort, but hey it works
	for (int i = 0; i < attributePairSorter.size(); i++) {
		data.parsedAttributePairs.push_back(attributePairSorter[i]);
	}

	std::cout << "debug" << &data.parsedData;
	myParserFile.close();
	std::cout << "debug2";
}

// Input: Contents of input.csv -- Splits the values into x and y coords

void FileHandling::sortGraphBasedOnParser(ClassData& data) {
	std::vector<float> xdatatemp;
	std::vector<float> ydatatemp;
	float xCoord = 0;
	float yCoord = 0;

	data.getLabelsFromParser();

	for (int i = 0; i < data.values[0].size(); i++) {
		data.attributeNameToDataIndex[data.values[0][i]] = i;
	}

	// step through each row of data, except for first which is attribute labels
	for (int i = 1; i < data.values.size(); i++) {
		// Get point class
		int nodeClass = stoi(data.values[i][data.values[0].size() - 1]);
		if (nodeClass > data.numOfClasses) {
			data.numOfClasses = nodeClass;
		}
		if (nodeClass > data.classTransparencies.size()) {
			data.classTransparencies.push_back(255); // init class transparencies
		}
		data.classNum.push_back(nodeClass);

		std::map<std::string, float> attributeValueMap;

		// Step through each column and add them to map
		for (int j = 0; j < data.values[i].size(); j++) {
			std::string attributeName = data.values[0][j];
			float attributeValue = stof(data.values[i][j]);
			if (attributeValue == 0.0f) {
				std::cout << "debug";
			}
			attributeValueMap[attributeName] = attributeValue;
		}

		int size = attributeValueMap.size();
		std::cout << "debug" << size;

		// step through each attribute pair and plot their coordinates
		for (int j = 0; j < data.parsedAttributePairs.size(); j++) {
			std::string attr1 = data.parsedAttributePairs[j][0];
			std::string attr2 = data.parsedAttributePairs[j][1];

			if (attr1 == "sepal-length" || attr2 == "sepal-length") {
				std::cout << "debug";
			}


			size = attributeValueMap.size();
			std::cout << "debug" << size;

			xCoord = attributeValueMap[attr1];
			yCoord = attributeValueMap[attr2];

			size = attributeValueMap.size();
			std::cout << "debug" << size;

			// init min/max attribute map
			if (data.attributeMinMax.find(attr1) == data.attributeMinMax.end()) {
				data.attributeMinMax[attr1].push_back(xCoord);
				data.attributeMinMax[attr1].push_back(xCoord);
			}
			if (data.attributeMinMax.find(attr2) == data.attributeMinMax.end()) {
				data.attributeMinMax[attr2].push_back(yCoord);
				data.attributeMinMax[attr2].push_back(yCoord);
			}

			if (xCoord > data.attributeMinMax[attr1][1]) {
				data.attributeMinMax[attr1][1] = xCoord;
			}

			if(xCoord < data.attributeMinMax[attr1][0]) {
				data.attributeMinMax[attr1][0] = xCoord;
			}

			if (yCoord > data.attributeMinMax[attr2][1]) {
				data.attributeMinMax[attr2][1] = yCoord;
			}

			if (yCoord < data.attributeMinMax[attr2][0]) {
				data.attributeMinMax[attr2][0] = yCoord;
			}

			if (xCoord > data.xmax) {
				data.xmax = xCoord;
			}
			if (yCoord > data.ymax) {
				data.ymax = yCoord;
			}

			xdatatemp.push_back(xCoord);
			ydatatemp.push_back(yCoord);
		}

		data.xdata.push_back(xdatatemp);
		data.dataTransparency.push_back(255);
		data.originalXData.push_back(xdatatemp); // Add line plot coords
		data.ydata.push_back(ydatatemp);
		data.originalYData.push_back(ydatatemp);

		//data.dataTerminationIndex.push_back(xdatatemp.size()); // why was i doing this?

		xdatatemp.clear();                                              // Clear for the next plot line
		ydatatemp.clear();
	}
	std::cout << "debug";
}



void FileHandling::sortGraphNotBasedOnParser(ClassData &data)
{
	std::vector<float> xdatatemp;
	std::vector<float> ydatatemp;
	float xCoord = 0;
	float yCoord = 0;
	
	data.getLabels();

	std::cout << "debug";

	for (int i = 1; i < (data.values.size()); i++)					// OLD: for (int i = 1; i < (data.values.size() - 1); i++)
	{																	// Columns
		int nodeClass = 
			stoi(data.values[i][(data.values[0].size() - 1)]);          // Get the class of the node
		if (nodeClass > data.numOfClasses)                              // Get the highest class number
			data.numOfClasses = nodeClass;
		if (nodeClass > data.classTransparencies.size()) {
			data.classTransparencies.push_back(255); // init class transparencies
		}
		data.classNum.push_back(nodeClass);                             // Add to vector of class numbers

		int counter = 0;
		int xcounter = 0;
		int ycounter = 0;
		for (int j = 1; j < (data.values[i].size() - 1); j++)			// Rows
		{               
			counter++;
			if (xdatatemp.size() <= ydatatemp.size())					// Get X-coords
			{            
				xcounter++;
				xCoord = stof(data.values[i][j]);
				xdatatemp.push_back(xCoord);
				if (xCoord > data.xmax)
					data.xmax = xCoord;
			}
			else
			{                                                            // Get y coords
				ycounter++;
				yCoord = stof(data.values[i][j]);
				ydatatemp.push_back(yCoord);
				if (yCoord > data.ymax)
					data.ymax = yCoord;
			}
		}

		std::cout << "debug" << counter << xcounter << ycounter;

		if (xdatatemp.size() != ydatatemp.size())						// Duplicate last pair if odd # of columns
		{                                            
			ydatatemp.push_back(xCoord);
			if (yCoord > data.ymax)  // Get y-max
				data.ymax = yCoord;
		}
		data.xdata.push_back(xdatatemp); 
		data.dataTransparency.push_back(255);
		data.originalXData.push_back(xdatatemp); // Add line plot coords
		data.ydata.push_back(ydatatemp);
		data.originalYData.push_back(ydatatemp);

		//data.dataTerminationIndex.push_back(xdatatemp.size()); // no don't do this 

		xdatatemp.clear();                                              // Clear for the next plot line
		ydatatemp.clear();
	}
}

void FileHandling::normalizeData(ClassData &data)
{
	//// Y Normalization
	//std::vector<float> minYcol;
	//std::vector<float> maxYcol;
	//std::vector<float> temp;

	//std::vector<std::vector<float> > convertedValues; // NEW


	//float min = 0;
	//float max = 0;

	//for (int j = 0; j < data.ydata[0].size(); j++) {                                               // Gets the min and max of every column
	//	min = data.ydata[0][j];
	//	for (int i = 0; i < data.ydata.size(); i++) {
	//		
	//		if (data.ydata[i][j] > max) {
	//			max = data.ydata[i][j];
	//		}
	//		if (data.ydata[i][j] < min) {
	//			min = data.ydata[i][j];
	//		}
	//	}
	//	if (min == max)   // This is used when entire column is of same values, the normalized value is 0.5 (chosen arbitrarily)
	//		min = 0;
	//	minYcol.push_back(min);
	//	maxYcol.push_back(max);
	//	max = 0;
	//
	//}

	// determine if data classes are zero indexed
	int minClass = INT_MAX;
	int maxClass = INT_MIN;
	for (int i = 1; i < data.values.size(); i++) {
		int curClass = stoi(data.values[i][data.values[0].size() - 1]);
		if (curClass > maxClass) {
			maxClass = curClass;
		}
		if (curClass < minClass) {
			minClass = curClass;
		}
	}
	if (minClass == 0) {
		data.isDatasetClassesZeroIndexed = true;
	}
	else {
		data.minClassNum = minClass;
	}

	// get min/max for each column
	std::vector<float> maxPerCol;
	std::vector<float> minPerCol;
	for (int i = 0; i < data.values[0].size() - 1; i++) {
		maxPerCol.push_back(INT_MIN);
		minPerCol.push_back(INT_MAX);
		for (int j = 1; j < data.values.size(); j++) {
			float val = stof(data.values[j][i]);
			if (val > maxPerCol[i]) {
				maxPerCol[i] = val;
			}
			if (val < minPerCol[i]) {
				minPerCol[i] = val;
			}
		}
	}


	for (int i = 1; i < data.values.size(); i++) {
		if (i == 99) {
			std::cout << "debug!!!";
		}
		std::vector<std::string> row = data.values[i];
		std::vector<float> normalizedRow;
		for (int j = 0; j < row.size() - 1; j++) {
			float val = stof(row[j]);
			if (maxPerCol[j] - minPerCol[j] == 0) {
				normalizedRow.push_back(0);
			}
			else {
				float min = minPerCol[j];
				float max = maxPerCol[j];
				normalizedRow.push_back((val - min) / (max - min));
			}
		}

		normalizedRow.push_back(stof(row[row.size() - 1]));

		// check
		if (stof(row[2]) > 2.45 && normalizedRow[2] < 0.355) {
			std::cout << "debug!";
		}

		data.normalizedValues.push_back(normalizedRow);
	}

	//for (int i = 0; i < data.ydata.size(); i++) {                                                  // Normalize the data from 0 - 1
	//	for (int j = 0; j < data.ydata[0].size(); j++) {
	//		float original = data.ydata[i][j];
	//		float currentMin = minYcol[j];
	//		float currentMax = maxYcol[j];
	//		float converted = (original - currentMin) / (currentMax - currentMin);

	//		temp.push_back(converted);
	//	}
	//	convertedValues.push_back(temp);
	//	temp.clear();
	//}
	//data.ydata.clear();
	//data.ydata = convertedValues;																	// Fill ydata coordinates with normalized data  
	//data.originalYData = convertedValues;

	//convertedValues.clear();
	////data.xmax = data.ydata[0].size();																// Change xMax and yMax to normalized data
	//data.ymax = 1;

	//// X Normalization after this point
	//std::vector<float> minXcol;
	//std::vector<float> maxXcol;
	//temp.clear();

	//convertedValues.clear();


	//min = 0;
	//max = 0;

	//for (int j = 0; j < data.xdata[0].size(); j++) {                                               // Gets the min and max of every column
	//	min = data.xdata[0][j];
	//	for (int i = 0; i < data.xdata.size(); i++) {
	//		if (data.xdata[i][j] > max) {
	//			max = data.xdata[i][j];
	//		}
	//		if (data.xdata[i][j] < min) {
	//			min = data.xdata[i][j];
	//		}
	//	}
	//	if (min == max)   //This is used when entire column is of same values, the normalized value is 0.5 (chosen arbitrarily)
	//		min =0;
	//	minXcol.push_back(min);
	//	maxXcol.push_back(max);
	//	max = 0;
	//}


	//for (int i = 0; i < data.xdata.size(); i++) {                                                  // Normalize the data from 0 - 1
	//	for (int j = 0; j < data.xdata[0].size(); j++) {
	//		float original = data.xdata[i][j];
	//		float currentMin = minXcol[j];
	//		float currentMax = maxXcol[j];
	//		float converted = (original - currentMin) / (currentMax - currentMin);

	//		temp.push_back(converted);
	//	}
	//	convertedValues.push_back(temp);
	//	temp.clear();
	//}
	//data.xdata.clear();
	//data.xdata = convertedValues;																	// Fill ydata coordinates with normalized data  
	//data.originalXData = convertedValues;
	//convertedValues.clear();
	////data.xmax = data.ydata[0].size();																// Change xMax and yMax to normalized data
	//data.xmax = 1;
	
}


