///-------------------------------------------------------------------------------------------------
// file:	InteractiveSPC.cpp
//
// summary:	Implements the class for a combined shifted paired coordinate graph.
///-------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "InteractiveSPC.h"
#include <set>
#include <map>
#include <algorithm>


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
	newFile.sortGraphNotBasedOnParser(data);

	data.numPlots = int(data.xdata[0].size());
	for (int y1 = 0; y1 < data.plotWidth.size(); y1++)
	{

		{
			data.plotWidth[y1] = worldW / (data.numPlots * 2.5);
			data.plotHeight[y1] = worldH / 2.0;
		}

	}
						// Width size for each graph
											// Height size for each graph

	doesParserBranch = data.doesDecisionTreeBranch;
	fillPlotLocations(); // Creates starting graph positions, and fills example data for now.

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	display();
}

void InteractiveSPC::setBackgroundTransparency(float alpha) {
	this->backgroundTransparency = alpha;
}

void InteractiveSPC::setBackgroundColorLightness(float lightnessCoeff) {
	this->backgroundClassColorCoefficient = lightnessCoeff;
}

void InteractiveSPC::updatePlotLocation(double mouseX, double mouseY, int plotNum) {
	data.xPlotCoordinates[plotNum] = mouseX - data.pan_x;
	data.yPlotCoordinates[plotNum] = mouseY - data.pan_y;
	
	// update references to plot x1,y1 and x2,y2
	float plotWidth = data.x2CoordPlot[plotNum] - data.x1CoordPlot[plotNum];
	float plotHeight = data.y2CoordPlot[plotNum] - data.y1CoordPlot[plotNum];
	data.x1CoordPlot[plotNum] = data.xPlotCoordinates[plotNum] - plotWidth / 2;
	data.y1CoordPlot[plotNum] = data.yPlotCoordinates[plotNum] - plotHeight / 2;
	data.x2CoordPlot[plotNum] = data.xPlotCoordinates[plotNum] + plotWidth / 2;
	data.y2CoordPlot[plotNum] = data.yPlotCoordinates[plotNum] + plotHeight / 2;
}

// Filling Graph Locations
void InteractiveSPC::fillPlotLocations()
{
	//for (int k = 1; k <= data.xdata.size(); k++)
	//{

	int numPlots = data.numPlots;
	Node* root = data.rootNode;
	int treeDepth = root->subtreeDepth;

	// Compute spans for each depth
	std::vector<int> depthSpanList;
	std::vector<std::vector<Node*> > depthList;
	for (int i = 0; i < treeDepth; i++) {
		depthSpanList.push_back(root->getSpanAtDepth(i));
		depthList.push_back(root->getAllNodesAtDepth(i));
	}

	// update plot widths
	for (int plotNum = 0; plotNum < data.plotWidth.size(); plotNum++) {
		data.plotWidth[plotNum] = data.worldWidth / (root->subtreeDepth + 1);
	}

	for (int i = 0; i < numPlots; i++) {
		data.xPlotCoordinates.push_back(0);
		data.yPlotCoordinates.push_back(0);
		data.x1CoordPlot.push_back(0);
		data.y1CoordPlot.push_back(0);
		data.x2CoordPlot.push_back(0);
		data.y2CoordPlot.push_back(0);
	}

	// For every depth, compute plot coordinates
	for (int currentDepth = 0; currentDepth < treeDepth; currentDepth++) {
		int currentSpan = depthSpanList[currentDepth];
		// get all nodes at given depth
		std::vector<Node*> nodesAtCurrentDepth = root->getAllNodesAtDepth(currentDepth);
		for (int i = 0; i < nodesAtCurrentDepth.size(); i++) {
			plotDrawOrder.push_back(nodesAtCurrentDepth[i]->plotNum);
		}
		int plotWidth = data.plotWidth[currentDepth];
		double plotHeight = 0.0;
		if (currentDepth > 0 && nodesAtCurrentDepth.size() == 1) {
			plotHeight = data.plotHeight[0] / 5.0;
		}
		else {
			plotHeight = data.plotHeight[0] / (double)(nodesAtCurrentDepth.size() + 1); // need to figure this out
		}

		std::cout << currentSpan << plotWidth << &nodesAtCurrentDepth << plotHeight;

		float plotX1 = (plotWidth * (currentDepth + 1) + currentDepth * 10) - plotWidth / 2;
		float plotX2 = plotX1 + plotWidth;

		double screenHeight = data.plotHeight[0] * 2;
		double plotHeightAtThisDepth = screenHeight / (nodesAtCurrentDepth.size() + 1);
		if (currentDepth > 0 && nodesAtCurrentDepth.size() == 1) {
			plotHeightAtThisDepth = screenHeight / (3 + 1);
		}
		
		double plotHeightGap = 0;
		if (nodesAtCurrentDepth.size() > 1) {
			plotHeightGap = 20;
		}
		
        // iterate through each node at this depth
		for (int nodeIndex = 0; nodeIndex < nodesAtCurrentDepth.size(); nodeIndex++) {
			Node* currentNode = root;
			// data.graphheight[]
			int plotNum = nodesAtCurrentDepth[nodeIndex]->plotNum;
			data.xPlotCoordinates[plotNum] = (plotWidth * (currentDepth + 1) + currentDepth * 10);
			data.yPlotCoordinates[plotNum] = ((nodeIndex + 1) * plotHeightAtThisDepth);

			data.x1CoordPlot[plotNum] = (plotX1);
			data.x2CoordPlot[plotNum] = (plotX2);
			data.y1CoordPlot[plotNum] = (data.yPlotCoordinates[plotNum] - (plotHeightAtThisDepth / 2) + plotHeightGap / 2);
			data.y2CoordPlot[plotNum] = (data.yPlotCoordinates[plotNum] + (plotHeightAtThisDepth / 2) - plotHeightGap / 2);
			data.plotHeight[plotNum] = plotHeightAtThisDepth - plotHeightGap;
		}
		std::cout << "debug";
		std::cout << &data.xPlotCoordinates << &data.yPlotCoordinates;
		std::cout << &data.x1CoordPlot << &data.x2CoordPlot << &data.y1CoordPlot << &data.y2CoordPlot;
	}


	std::cout << &data.x1CoordPlot << &data.x2CoordPlot << &data.y1CoordPlot << &data.y2CoordPlot;



		// // constructs plots in a single line
		// for (int i = 1; i <= data.classsize; i++)
		// {

		// 	//data.xPlotCoordinates.push_back(data.graphwidth[currentDepth - 1] * currentDepth + currentDepth * 0.08 * data.graphwidth[data.graphwidth.size() - 1]);			
		// 	data.xPlotCoordinates.push_back(data.graphwidth[i - 1] * i + i * 10);
		// 	data.yPlotCoordinates.push_back(data.graphheight[i - 1]);

		// 	//set coordinates to draw rectangles
		// 	data.x1CoordGraph.push_back(data.xPlotCoordinates[i - 1] - data.graphwidth[i - 1] / 2);
		// 	data.x2CoordGraph.push_back(data.xPlotCoordinates[i - 1] + data.graphwidth[i - 1] / 2);
		// 	data.y1CoordGraph.push_back(data.yPlotCoordinates[i - 1] - data.graphheight[i - 1] / 2);
		// 	data.y2CoordGraph.push_back(data.yPlotCoordinates[i - 1] + data.graphheight[i - 1] / 2);

		// }

		// data.xclasses.push_back(data.xPlotCoordinates);
		// data.yclasses.push_back(data.yPlotCoordinates);
	//}
}

/* Draws rectangles in rectangle mode around all continue zones*/
void InteractiveSPC::drawRectanglesOnGray() {
	// iterate through each plot
	for (int plotNum = 0; plotNum < data.numPlots; plotNum++) {
		// find each zone within the plot that is not a termination zone
		for (int parsedIndex = 0; parsedIndex < dataParsed.parsedData.size(); parsedIndex++) { // TODO: if dataParsed.parsedData was a map this would be WAY faster...
			std::vector<float> parserData = dataParsed.parsedData[parsedIndex];
			if ((int)parserData[4] != plotNum || parserData[parserData.size() - 1] >= 0) {
				continue;
			}
			// TODO: There's probably a MUCH easier way to do this
			// need to accomodate various swappings
			// X/Y swap swaps parser data itself, so we need not do anything here
			// but the x and y invert buttons DONT alter data, so we need to make up for that here
			const float zoneX1 = parserData[0];
			const float zoneY1 = parserData[1];
			const float zoneX2 = parserData[2];
			const float zoneY2 = parserData[3];

			const float pltX1 = data.x1CoordPlot[plotNum];
			const float pltY1 = data.y1CoordPlot[plotNum];
			const float pltX2 = data.x2CoordPlot[plotNum];
			const float pltY2 = data.y2CoordPlot[plotNum];

			const float plotWidth = data.plotWidth[plotNum];
			const float plotHeight = data.plotHeight[plotNum];

			GLfloat zoneToDrawX1;
			GLfloat zoneToDrawX2;
			GLfloat zoneToDrawY1;
			GLfloat zoneToDrawY2;

			// old
			//zoneToCheckX1 = pltX1 + plotWidth * zoneX1 + data.pan_x;
			//zoneToCheckX2 = pltX1 + plotWidth * zoneX2 + data.pan_x;
			//zoneToCheckY2 = pltY2 - plotHeight * zoneY2 + data.pan_y;
			//zoneToCheckY1 = pltY2 - plotHeight * zoneY1 + data.pan_y;

			if (plotsWithXAxisInverted.size() != 0 && plotsWithXAxisInverted.find(plotNum) != plotsWithXAxisInverted.end()) {
				zoneToDrawX2 = pltX2 - plotWidth * zoneX1 + data.pan_x;
				zoneToDrawX1 = pltX2 - plotWidth * zoneX2 + data.pan_x;
			}
			else {
				zoneToDrawX1 = pltX1 + plotWidth * zoneX1 + data.pan_x;
				zoneToDrawX2 = pltX1 + plotWidth * zoneX2 + data.pan_x;
			}
			if (plotsWithYAxisInverted.size() != 0 && plotsWithYAxisInverted.find(plotNum) != plotsWithYAxisInverted.end()) {
				zoneToDrawY1 = pltY1 + plotHeight * zoneY2 + data.pan_y;
				zoneToDrawY2 = pltY1 + plotHeight * zoneY1 + data.pan_y;
			}
			else {
				zoneToDrawY2 = pltY2 - plotHeight * zoneY2 + data.pan_y;
				zoneToDrawY1 = pltY2 - plotHeight * zoneY1 + data.pan_y;
			}
			
			rectX1List.push_back(zoneToDrawX1);
			rectX2List.push_back(zoneToDrawX2);
			rectY1List.push_back(zoneToDrawY1);
			rectY2List.push_back(zoneToDrawY2);
		}
	}

	isRectangleMode = true;
}

/* Draws data sets. */
std::set<int> debugSet;
int InteractiveSPC::drawData(float x1, float y1, int caseNum, int plotNum) {
	// Don't draw data past its termination point
	/*if (isLineTerminationMode && plotNum > data.dataTerminationIndex[recordNum] - 1) {
		if (plotNum != 0) return;
	}*/

	int caseClass = data.classNum[caseNum] - 1;

	if (plotNum == 0 && caseClass == 0) {
		std::cout << "debug";
	}

	float plt1X1 = data.x1CoordPlot[plotNum];
	float plt1Y1 = data.y1CoordPlot[plotNum];
	float plt1X2 = data.x2CoordPlot[plotNum];
	float plt1Y2 = data.y2CoordPlot[plotNum];

	float plotHeight = data.plotHeight[plotNum];
	float plotWidth = data.plotWidth[plotNum];

	// Get x point
	if (plotsWithXAxisInverted.find(plotNum) != plotsWithXAxisInverted.end()) {
		x1 = plt1X2 - plotWidth * x1 + data.pan_x;
	}
	else {
		x1 = plt1X1 + plotWidth * x1 + data.pan_x;
	}
	if (plotsWithYAxisInverted.find(plotNum) != plotsWithYAxisInverted.end()) {
		y1 = plt1Y1 + plotHeight * y1 + data.pan_y;
	}
	else {
		y1 = plt1Y2 - plotHeight * y1 + data.pan_y;
	}

	// debug
	//glColor3ub(255, 0, 0);
	//glBegin(GL_POINTS);
	//glVertex2f(plt1X1, plt1Y1);
	//glColor3ub(0, 0, 255);
	//glVertex2f(plt1X2, plt1Y2);
	//glEnd();
	//end debug
    // old code
	// x1 = plt1X1 + (plt1X2 - plt1X1) * x1 + data.pan_x;
	// y1 = plt1Y2 - (plt1Y2 - plt1Y1) * y1 + data.pan_y;

	int point1BackgroundClass = findBackgroundClassOfPoint(x1, y1, plotNum);
    int point1BackgroundZone = findBackgroundZoneIdOfPoint(x1, y1, plotNum);
    std::set<int> * point1BackgroundZoneTotalCaseList = &data.plotNumZoneTotalCases[plotNum][point1BackgroundZone];
    std::set<int> * point1BackgroundZoneMisclassifiedCaseList = &data.plotNumZoneTotalMisclassifiedCases[plotNum][point1BackgroundZone];
    if (point1BackgroundZoneTotalCaseList->find(caseNum) == point1BackgroundZoneTotalCaseList->end()) {
        point1BackgroundZoneTotalCaseList->insert(caseNum);
    }
    if (point1BackgroundClass != caseClass) {
        if (point1BackgroundZoneMisclassifiedCaseList->find(caseNum) == point1BackgroundZoneMisclassifiedCaseList->end()) {
            point1BackgroundZoneMisclassifiedCaseList->insert(caseNum);
        }
    }

    if (point1BackgroundZoneTotalCaseList->size() > data.maxCasesPerPlotZone) {
        data.maxCasesPerPlotZone = point1BackgroundZoneTotalCaseList->size();
    }

	// asses overlap
	std::vector<int>* overlapList = &overlapMap[x1][y1][caseClass];
	if (std::find(overlapList->begin(), overlapList->end(), caseNum) == overlapList->end()) {
		overlapList->push_back(caseNum);
	}

	float shiftAmount = 0;
	float classShiftAmount = 0;
	const float shiftConstant = 10;
	const float classShift = 10;

	std::map<int, std::vector<int>> caseClassMap = overlapMap[x1][y1];
	caseClassMap = overlapMap[x1][y1];
	overlapList = &overlapMap[x1][y1][caseClass];
	if (isOverlapMitigationModeAll && point1BackgroundClass >= 0) {
		std::vector<int> caseList;
		for (int i = 0; i < data.classes.size(); i++) {
			int classNum = data.classes[i];
			if (classNum < 0) continue;
			for (int j = 0; j < caseClassMap[classNum].size(); j++) {
				caseList.push_back(caseClassMap[classNum][j]);
			}
		}

		int totalCases = caseList.size();
		auto caseIndexIterator = std::find(caseList.begin(), caseList.end(), caseNum);
		int index = -1;
		if (caseIndexIterator != caseList.end()) {
			index = caseIndexIterator - caseList.begin();
		}
		if (index == -1) {
			std::cout << "something is wrong";
		}
		if (totalCases != 0) {
			shiftAmount = ((float)index / (float)totalCases) * shiftConstant;;
		}
		x1 += shiftAmount + classShiftAmount;
		y1 += shiftAmount;
	}
	else if (isOverlapMitigationMode && caseClassMap.size() > 1) {
		if (point1BackgroundClass >= 0) {
			std::vector<int> overlappingCases;
			// count how many cases there are that are overlapping
			int nonNegClassCount = 0;
			int totalCases = 0;
			for (int i = 0; i < data.classes.size(); i++) {
				int classNum = data.classes[i];
				if (classNum < 0) continue;
				nonNegClassCount++;
				if (caseClassMap.find(classNum) != caseClassMap.end()) {
					for (int j = 0; j < caseClassMap[classNum].size(); j++) {
						overlappingCases.push_back(caseClassMap[classNum][j]);
						totalCases++;
					}
				}
			}

			// determine which index position the current case is
			auto caseIndexIterator = std::find(overlappingCases.begin(), overlappingCases.end(), caseNum);
			int index = -1;
			if (caseIndexIterator != overlappingCases.end()) {
				index = caseIndexIterator - overlappingCases.begin();
			}
			if (index == -1) {
				std::cout << "something is wrong";
			}

			// shuffle function
			// x1' = x1 + (index / totalCases) * b
			// y1' = y1 + (index / totalCases) * b
			//find num of non-negative classes
			if (totalCases != 0) {
				shiftAmount = ((float)index / (float)totalCases) * shiftConstant;;
			}
			if (nonNegClassCount != 0) {
				classShiftAmount = (float)caseClass / (float)nonNegClassCount;
			}
			x1 += shiftAmount + classShiftAmount;
			y1 += shiftAmount;

			std::cout << "debug";
		}
	}

	if (plotNum == 0 && caseClass != 0 && point1BackgroundClass == 0) {
		std::cout << "debug";
	}

	// Debug
	int point1BackgroundClassDebug = findBackgroundClassOfPoint(x1, y1, plotNum);

	if (isRectangleMode) {
		for (int i = 0; i < rectX1List.size(); i++) {
			GLfloat highX = max(rectX1List[i], rectX2List[i]);
			GLfloat lowX = min(rectX1List[i], rectX2List[i]);
			GLfloat highY = max(rectY1List[i], rectY2List[i]);
			GLfloat lowY = min(rectY1List[i], rectY2List[i]);
			if (isPointWithinRect(x1, y1, lowX, highY, highX, lowY)) {
				//if (point1BackgroundClass >= 0) {
					// compute new location
				GLfloat deltaX = abs(rectX2List[i] - rectX1List[i]);
				GLfloat deltaY = abs(rectY2List[i] - rectY1List[i]);
				GLfloat newX = lowX + deltaX / 2;
				GLfloat newY = highY - (deltaY / (data.numOfClasses + 2)) * (caseClass + 1);
				x1 = newX;
				y1 = newY;
				//}
			}
		}
	}



	// debug
	//glPointSize(6.0);
	//if (point1BackgroundClass >= 0) {
	//	glColor3f(data.classColor[point1BackgroundClass][0], data.classColor[point1BackgroundClass][1], data.classColor[point1BackgroundClass][2]);
	//}
	//else if (point1BackgroundClass != INT_MIN){
	//	glColor3ub(data.continueClassColor[point1BackgroundClass][0], data.continueClassColor[point1BackgroundClass][1], data.continueClassColor[point1BackgroundClass][2]);
	//}
	//else {
	//	glColor3ub(255, 0, 0);
	//}
	//glBegin(GL_POINTS);
	//glVertex2f(x1, y1);
	//glEnd();
	//glPointSize(4.0);
	// end debug

	int pointWasCorrectlyClassified = false;

	GLubyte classTransparency = data.classTransparencies[caseClass];
	
	if (point1BackgroundClass >= 0 && point1BackgroundClass != caseClass) {
		// if we're in here, this is a misclassification
		std::vector<int>* misclassifiedClassList = &data.misclassifiedCases[caseClass][point1BackgroundClass];
		if (std::find(misclassifiedClassList->begin(), misclassifiedClassList->end(), caseNum) == misclassifiedClassList->end()) {
			misclassifiedClassList->push_back(caseNum);
			data.classMisclassifiedCaseCount[caseClass]++;
		}

		// determine if we should be highlighting misclassified points
		if (isHighlightMisclassficationsMode) {
			glColor4ub(255, 0, 0, classTransparency);
			glPointSize(8.0);
			glBegin(GL_POINTS);
			glVertex2f(x1, y1);
			glEnd();
			glPointSize(4.0);
		}
	}

	
    // draw regular point
    // draw point one
    glPointSize(4.0);
    glColor4ub(0, 0, 0, classTransparency);
    if (isPointColorMode) {
        if (point1BackgroundClass >= 0) {
            GLubyte r = data.classColor[caseClass][0];
            GLubyte g = data.classColor[caseClass][1];
            GLubyte b = data.classColor[caseClass][2];
            GLubyte a = classTransparency;
            glColor4ub(r, g, b, a);
        }
    }


    // debug
    /*GLubyte r = data.classColor[recordClass][0];
    GLubyte g = data.classColor[recordClass][1];
    GLubyte b = data.classColor[recordClass][2];
    glColor4ub(r, g, b, classTransparency);*/
    // endDebug

    //debug
    /*std::vector<float> color;
    if (point1BackgroundClass >= 0) {
        color = data.classColor[point1BackgroundClass];
    }
    else {
        color = data.continueClassColor[point1BackgroundClass];
    }
    glColor4ub(color[0], color[1], color[2], classTransparency);*/
    // end debug

    glBegin(GL_POINTS);
    glVertex2f(x1, y1);
    glEnd();
    // end point one draw

	if (point1BackgroundClass >= 0) {
		if (point1BackgroundClass == caseClass) {
			// point correctly classified
			pointWasCorrectlyClassified = true;
			//if (isLineTerminationMode) {
			//	return;
			//}
			//else {
			//	// if not line termination mode, continue to render down the tree. If not, do nothing
			//	// again, how would we know which path to take?
			//	return; // return for now. TODO
			//}
		}
		else {
			// point incorrectly classified
			// either way, continue drawing
			// oh wait how would this work? 
			// if we're using decision tree, we don't know what the next tree should be unless we land on a continue zone

			// idea: option that says : "trace misclassifications" which has a drop down for which misclassification to view.
			//		then the user can select one and it will render the point on each plot.
			//		i.e. : render selected record on all paths through tree to leaves, while highlighting the incorrectly classified zones
			// for now return
		}
		//data.dataTerminationIndex[recordNum] = plotNum;
		return -1;
	}
	else if (point1BackgroundClass == INT_MIN) {
		// if there is no zone behind us, I don't think we're even supposed to draw the point but we'll see.
		//data.dataTerminationIndex[recordNum] = plotNum;
		return -1;
	}
	// continue drawing based on what's in the continue zone's destination param
	
	// get next point
	// we have: record number, next plot number, record class. get point from record with attributes present in next plot
	int nextPlotNum = data.plotDestinationMap[plotNum][point1BackgroundClass];
	std::vector<std::string> destinationPlotAttributes = data.parsedAttributePairs[nextPlotNum];
	int destAttr1Index = data.attributeNameToDataIndex[destinationPlotAttributes[0]];
	int destAttr2Index = data.attributeNameToDataIndex[destinationPlotAttributes[1]];
	float x2 = data.normalizedValues[caseNum][destAttr1Index];
	float y2 = data.normalizedValues[caseNum][destAttr2Index];

	float plt2X1 = data.x1CoordPlot[nextPlotNum];
	float plt2Y1 = data.y1CoordPlot[nextPlotNum];
	float plt2X2 = data.x2CoordPlot[nextPlotNum];
	float plt2Y2 = data.y2CoordPlot[nextPlotNum];

	plotHeight = data.plotHeight[nextPlotNum];
	plotWidth = data.plotWidth[nextPlotNum];


	if (plotsWithXAxisInverted.find(nextPlotNum) != plotsWithXAxisInverted.end()) {
		x2 = plt2X2 - plotWidth * x2 + data.pan_x;
	}
	else {
		x2 = plt2X1 + plotWidth * x2 + data.pan_x;
	}

	if (plotsWithYAxisInverted.find(nextPlotNum) != plotsWithYAxisInverted.end()) {
		y2 = plt2Y1 + plotHeight * y2 + data.pan_y;
	}
	else {
		y2 = plt2Y2 - plotHeight * y2 + data.pan_y;
	}
    
	// x2 = plt2X1 + (plt2X2 - plt2X1) * x2 + data.pan_x;
	// y2 = plt2Y2 - (plt2Y2 - plt2Y1) * y2 + data.pan_y;

	// set line color
	glColor4ub(128, 128, 128, classTransparency);
	int point2BackgroundClass = findBackgroundClassOfPoint(x2, y2, nextPlotNum);

	caseClassMap = overlapMap[x2][y2];

    // check for incorrect classifications
    bool doesCaseClassMapContainMisclassifiedCases = false;
    for (int i = 0; i < data.classes.size(); i++) {
		int curClass = data.classes[i];
		if (curClass == caseClass) continue;
		if (caseClassMap.find(curClass) != caseClassMap.end()) {
			doesCaseClassMapContainMisclassifiedCases = true;
			break;
		}
    }

	overlapList = &overlapMap[x2][y2][caseClass];
	if (isOverlapMitigationModeAll && point2BackgroundClass >= 0) {
		std::vector<int> caseList;
		for (int i = 0; i < data.classes.size(); i++) {
			int classNum = data.classes[i];
			if (classNum < 0) continue;
			for (int j = 0; j < caseClassMap[classNum].size(); j++) {
				caseList.push_back(caseClassMap[classNum][j]);
			}
		}

		int totalCases = caseList.size();
		auto caseIndexIterator = std::find(caseList.begin(), caseList.end(), caseNum);
		int index = -1;
		if (caseIndexIterator != caseList.end()) {
			index = caseIndexIterator - caseList.begin();
		}
		if (index == -1) {
			std::cout << "something is wrong";
		}
		if (totalCases != 0) {
			shiftAmount = ((float)index / (float)totalCases) * shiftConstant;;
		}
		x2 += shiftAmount + classShiftAmount;
		y2 += shiftAmount;
	} else if (isOverlapMitigationMode && (caseClassMap.size() > 1 || doesCaseClassMapContainMisclassifiedCases)) {
		if (point2BackgroundClass >= 0) {
			std::vector<int> overlappingCases;
			// count how many cases there are that are overlapping
			int nonNegClassCount = 0;
			int totalCases = 0;
			for (int i = 0; i < data.classes.size(); i++) {
				int classNum = data.classes[i];
				if (classNum < 0) continue;
				nonNegClassCount++;
				if (caseClassMap.find(classNum) != caseClassMap.end()) {
					for (int j = 0; j < caseClassMap[classNum].size(); j++) {
						overlappingCases.push_back(caseClassMap[classNum][j]);
						totalCases++;
					}
				}
			}

			// determine which index position the current case is
			auto caseIndexIterator = std::find(overlappingCases.begin(), overlappingCases.end(), caseNum);
			int index = -1;
			if (caseIndexIterator != overlappingCases.end()) {
				index = caseIndexIterator - overlappingCases.begin();
			}
			if (index == -1) {
				std::cout << "something is wrong";
			}

			// shuffle function
			// x1' = x1 + (index / totalCases) * b
			// y1' = y1 + (index / totalCases) * b
			//find num of non-negative classes
			if (totalCases != 0) {
				shiftAmount = ((float)index / (float)totalCases) * shiftConstant;;
			}
			if (nonNegClassCount != 0) {
				classShiftAmount = (float)caseClass / (float)nonNegClassCount;
			}
			x2 += shiftAmount + classShiftAmount;
			y2 += shiftAmount;

			std::cout << "debug";
		}
	}

	if (isRectangleMode) {
		for (int i = 0; i < rectX1List.size(); i++) {
			GLfloat highX = max(rectX1List[i], rectX2List[i]);
			GLfloat lowX = min(rectX1List[i], rectX2List[i]);
			GLfloat highY = max(rectY1List[i], rectY2List[i]);
			GLfloat lowY = min(rectY1List[i], rectY2List[i]);
			if (isPointWithinRect(x2, y2, lowX, highY, highX, lowY)) {
				// compute new location
				GLfloat deltaX = abs(rectX2List[i] - rectX1List[i]);
				GLfloat deltaY = abs(rectY2List[i] - rectY1List[i]);
				GLfloat newX = lowX + deltaX / 2;
				GLfloat newY = highY - (deltaY / (data.numOfClasses + 2)) * (caseClass + 1);
				x2 = newX;
				y2 = newY;
			}
		}
	}

	if (isLineColorMode && point2BackgroundClass >= 0) {
		GLubyte r = data.classColor[caseClass][0];
		GLubyte g = data.classColor[caseClass][1];
		GLubyte b = data.classColor[caseClass][2];
		GLubyte a = classTransparency;
		glColor4ub(r, g, b, a);
	}

	// draw line from point 1 to point 2
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();

	// only draw line, not the point. point will be covered when point2 becomes point1 on future iterations

	// debug
	if (point1BackgroundClass == INT_MIN || point1BackgroundClass >= 0) {
		std::cout << "yay we can stop drawing if we've got line mode on!";
	}
	else if (point1BackgroundClass < 0) {
		std::cout << "debug less than zerp" << point1BackgroundClass;
	}
	else {
		std::cout << "hmmm this shouldnt be hit ever...";
	}

	return nextPlotNum;
}



// NOT USED ANYMORE
//std::set<int> backgroundClassSet; // Debug
//std::set<int> pointClassSet; // Debug
//void InteractiveSPC::drawData(float x1, float y1, float x2, float y2, int i, int j)
//{
//	// Don't draw data past its termination point
//	if (isLineTerminationMode && j > data.dataTerminationIndex[i] - 1) {
//		if (j != 0) return;
//	}
//	
//	// this is all for getting plot coordinates
//	float xratio = data.plotWidth[j] / data.xmax; // Normalize data to the graph size
//	float yratio = data.plotHeight[j] / data.ymax;	
//	x1 -= (data.plotWidth[j] / 2);
//	x2 -= (data.plotWidth[j + 1] / 2);
//	y1 += (data.plotHeight[j] / 2);
//	y2 += (data.plotHeight[j + 1] / 2);
//	float x1Coord = data.plotWidth[j] * data.xdata[i][j];
//	float x2Coord = (x2 - x1) + data.plotWidth[j + 1] * data.xdata[i][j + 1]; // this is where actual data gathered
//	float y1Coord = -data.plotHeight[j] * data.ydata[i][j]; //height of plot is constant = 328.5
//	float y2Coord = (y2 - y1) - data.plotHeight[j+1] * data.ydata[i][j + 1]; // this is where actual data is gathered
//	float x1CoordTrans = x1Coord + (x1 + data.pan_x);
//	float x2CoordTrans = x2Coord + (x1 + data.pan_x);
//	float y1CoordTrans = y1Coord + (y1 + data.pan_y);
//	float y2CoordTrans = y2Coord + (y1 + data.pan_y);
//
//	int classnum = data.classNum[i] - 1;
//	pointClassSet.insert(classnum);
//
//	
//	// If in drawn rectangle mode, check if the points make a line that intersects the rectangle
//	GLfloat middleX;
//	GLfloat middleY;
//	GLfloat middleXTerminating;
//	GLfloat middleYTerminating;
//
//	bool drawMiddleVertex = false;
//	bool drawVertex1 = true;
//	bool drawVertex2 = true;
//
//	if (isLineTerminationMode && data.dataTerminationIndex[i] == 0) {
//		drawVertex2 = false;
//	}
//
//	if (isRectangleMode) { // TODO : need to adapt for branches
//		if (doPointsIntersectRectangle(x1CoordTrans, y1CoordTrans, x2CoordTrans, y2CoordTrans)) {
//			// draw point within rectangle based on how many classes there are
//			// TODO: later could add a selection based on class
//			GLfloat deltaX = abs(rectX1 - rectX2);
//			GLfloat deltaY = abs(rectY1 - rectY2);
//			GLfloat highX = max(rectX1, rectX2);
//			GLfloat lowX = min(rectX1, rectX2);
//			GLfloat highY = max(rectY1, rectY2);
//			GLfloat lowY = min(rectY1, rectY2);
//
//			middleX = lowX + 2 * ((highX - lowX) / 3) - (x1 + data.pan_x);
//			middleY = lowY + (((highY - lowY) / (data.numOfClasses + 2)) * (classnum + 1)) - (y1 + data.pan_y);
//			middleXTerminating = lowX + ((highX - lowX) / 3) - (x1 + data.pan_x);
//			middleYTerminating = lowY + (((highY - lowY) / (data.numOfClasses + 2)) * (classnum + 1)) - (y1 + data.pan_y);
//
//			if (isPointWithinRect(x1CoordTrans, y1CoordTrans, rectX1, rectY2, rectX2, rectY1)) {
//				drawVertex1 = false;
//			}
//			if (isPointWithinRect(x2CoordTrans, y2CoordTrans, rectX1, rectY2, rectX2, rectY1)) {
//				drawVertex2 = false;
//			}
//
//			if (data.dataTerminationIndex[i] == j + 1) { // Need to check if we should be checking point 1 or point2
//				//x1Coord = middleXTerminating;
//				//y1Coord = middleYTerminating;
//				x2Coord = middleXTerminating;
//				y2Coord = middleYTerminating;
//			}
//
//			if (findBackgroundClassOfPoint(x1Coord, y1Coord) != classnum) { // do something special if point class doesnt match background zone
//				// Do nothing for now
//			}
//
//			drawMiddleVertex = true;
//		}
//		else {
//			data.calculateTerminationPoint(i);
//		}
//	}
//
//	//debug
//	//glColor4ub(255, 0, 0, 255);
//	//glPointSize(8.0);
//	//glBegin(GL_POINTS);
//	//glVertex2f(x1CoordTrans, y1CoordTrans);
//	//glColor4ub(0, 0, 255, 255);
//	//glVertex2f(x2CoordTrans, y2CoordTrans);
//	//glPointSize(4.0);
//	//glEnd();
//	
//	int backgroundClassP1 = findBackgroundClassOfPoint(x1CoordTrans, y1CoordTrans);
//	int backgroundClassP2 = findBackgroundClassOfPoint(x2CoordTrans, y2CoordTrans);
//	backgroundClassSet.insert(backgroundClassP1);
//
//	if (isHighlightMisclassficationsMode && (backgroundClassP1 != -1 && backgroundClassP1 != classnum)) {
//		glColor4ub(255, 0, 0, data.classTransparencies[classnum]);
//		glPointSize(8.0);
//		glBegin(GL_POINTS);
//		glVertex2f(x1CoordTrans, y1CoordTrans);
//		glEnd();
//		glPointSize(4.0);
//	}
//
//	glPushMatrix();	// Makes a new layer
//
//	glTranslatef(x1 + data.pan_x, y1 + data.pan_y, 0); // Translates starting position to draw
//
//	// If point terminates on the next point, color it
//	// COLORING FOR LINES
//	if (isPointColorMode || (isColorTerminationMode && data.dataTerminationIndex[i] != j + 1)) {
//		glColor4ub(128, 128, 128, data.classTransparencies[classnum]);
//	}
//	else {
//		glColor4ub(data.classColor[classnum][0], data.classColor[classnum][1], data.classColor[classnum][2], data.classTransparencies[classnum]);
//	}
//
//	if (data.showHideLinesVar)
//	{
//		glBegin(GL_LINE_STRIP); 
//		//data.classTransparency[1] = 70;
//		//glColor4ub(data.classColor[classnum][0], data.classColor[classnum][1], data.classColor[classnum][2], data.dataTransparency[currentDepth]);
//		//glColor4ub(data.classColor[classnum][0], data.classColor[classnum][1], data.classColor[classnum][2], data.classTransparencies[classnum]);
//		if (drawVertex1) {
//			glVertex2f(x1Coord, y1Coord);
//		}
//		if (drawMiddleVertex) {
//			if (drawVertex2) {
//				GLfloat currentColor[4];
//				glGetFloatv(GL_CURRENT_COLOR, currentColor);
//				glColor4ub(128, 128, 128, data.classTransparencies[classnum]);
//				glVertex2f(middleX, middleY);
//				glColor4ub(currentColor[0], currentColor[1], currentColor[2], currentColor[3]);
//			}
//			else {
//				glVertex2f(middleX, middleY);
//			}
//		}
//		if(drawVertex2 && data.numPlots!=1)
//			glVertex2f(x2Coord, y2Coord); // ending vertex
//		glEnd();
//	}
//	
//	// POINT 1 COLOR
//	if (isPointColorMode && backgroundClassP1 != -1) {
//		glColor4ub(data.classColor[classnum][0], data.classColor[classnum][1], data.classColor[classnum][2], data.classTransparencies[classnum]);
//	}
//	else {
//		glColor4ub(0, 0, 0, data.classTransparencies[classnum]);
//	}
//	
//	glBegin(GL_POINTS);
//	
//	//glVertex2f(0 + xratio * data.xdata[currentDepth][nodeIndex], 0 - yratio * data.ydata[currentDepth][nodeIndex]);                                     // starting vertex
//	if (drawVertex1) {
//		glVertex2f(x1Coord, y1Coord);
//	}
//	if (drawMiddleVertex) {
//		//glColor4ub(0, 255, 0, 255); // debug
//		glVertex2f(middleX, middleY);
//	}
//	glEnd();
//
//	// POINT 2 COLOR
//	if (isPointColorMode && backgroundClassP2 != -1) {
//		glColor4ub(data.classColor[classnum][0], data.classColor[classnum][1], data.classColor[classnum][2], data.classTransparencies[classnum]);
//	}
//	else {
//		glColor4ub(0, 0, 0, data.classTransparencies[classnum]);
//	}
//
//	glPointSize(4);
//	glBegin(GL_POINTS);
//	//glVertex2f((x2 - x1) + xratio * data.xdata[currentDepth][nodeIndex + 1], (y2 - y1) - yratio * data.ydata[currentDepth][nodeIndex + 1]);                         // ending vertex
//	if (drawVertex2 && data.numPlots != 1)
//		//glColor4ub(0, 0, 255, 255); // debug
//		glVertex2f(x2Coord, y2Coord);	
//	glEnd();
//	if (j == 0)
//	{
//		//glColor4ub(data.classColor[classnum][0], data.classColor[classnum][1], data.classColor[classnum][2], data.classTransparencies[classnum]);
//		if (drawVertex1) {
//			glVertex2f(x1Coord, y1Coord);
//		}
//	}
//
//	glPopMatrix();
//	
//}

/* DISPLAY */

//debug list
std::set<GLubyte> backgroundLightnessSet;
void InteractiveSPC::display() {
	glClearColor(255 / 255.0f, 255 / 255.0f, 254 / 255.0f, 0.0f);			//194, 206, 218. VisCanvas background color.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* Adjust the windows aspect ratio? */
	glOrtho(data.leftWidth, data.worldWidth, data.worldHeight, data.bottomHeight, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();														// Reset the model-view matrix

	/* Draws a plot for each dimension */
	// ahhh but we want to draw in the order that appears in the tree! Not just in order
	for (int i = 0; i < plotDrawOrder.size(); i++) {
		int plotToDraw = plotDrawOrder[i];
		data.drawPlot(data.xPlotCoordinates[plotToDraw], data.yPlotCoordinates[plotToDraw], data.plotWidth[plotToDraw], data.plotHeight[plotToDraw], plotToDraw);
		updatePlotLocation(data.xPlotCoordinates[plotToDraw], data.yPlotCoordinates[plotToDraw], plotToDraw);
	}


	/*for (int i = 0; i < data.numPlots; i++)
	{
		data.drawPlot(data.xPlotCoordinates[i], data.yPlotCoordinates[i], data.plotWidth[i], data.plotHeight[i], i);
		updatePlotLocation(data.xPlotCoordinates[i], data.yPlotCoordinates[i], i);
	}*/

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

            int plot = dataParsed.parsedData[p][4];

			// point(xc, yc) denotes center of plot
			// x1,y1 should be upper left visually
			// x2,y2 should be lower right visually
			// coord x1 = xc - (half the plot width) + (fraction of the plotWidth that the current rect x1 is at)
			// coord x2 = xc + (half the plot width) + (fraction of the plotWidth that the current rect x2 is at)
			// coord y1 = yc - (half the plot width <will be visually lower part of graph>)  - (fraction of plot height that rect y1 should be at) 
			// coord y2 = yc + (half the plot width <will be visually lower part of graph>)  - (fraction of plot height that rect y1 should be at)
			GLfloat y1 = 0;
			GLfloat x2 = 0;
			GLfloat y2 = 0;
			GLfloat x1 = 0;

			// get plot coords

			// calculate x components
			if (plotsWithXAxisInverted.find(plot) != plotsWithXAxisInverted.end()) {
				x1 = data.x2CoordPlot[plot] - dataParsed.parsedData[p][0] * data.plotWidth[plot];
				x2 = data.x2CoordPlot[plot] - dataParsed.parsedData[p][2] * data.plotWidth[plot];
			}
			else {
				x1 = data.x1CoordPlot[plot] + dataParsed.parsedData[p][0] * data.plotWidth[plot];
				x2 = data.x1CoordPlot[plot] + dataParsed.parsedData[p][2] * data.plotWidth[plot];
			}
			
			// calculate y components
			if (plotsWithYAxisInverted.find(plot) != plotsWithYAxisInverted.end()) {
				y2 = data.y1CoordPlot[plot] + dataParsed.parsedData[p][1] * data.plotHeight[plot];
				y1 = data.y1CoordPlot[plot] + dataParsed.parsedData[p][3] * data.plotHeight[plot];
			}
			else {
				y2 = data.y2CoordPlot[plot] - dataParsed.parsedData[p][1] * data.plotHeight[plot];
				y1 = data.y2CoordPlot[plot] - dataParsed.parsedData[p][3] * data.plotHeight[plot];
			}

			// debug
			if (plot == 0) {
				std::cout << "test test debug debug :)";
			}


			float maxX = max(x1, x2);
			float maxY = max(y1, y2);
			float minX = min(x1, x2);
			float minY = min(y1, y2);

            /*float px = minX + maxX / 2;
            float py = minY + maxY / 2;*/
			float px = minX + (maxX - minX) / 2;
			float py = minY + (maxY - minY) / 2;
            int zone = findBackgroundZoneIdOfPoint(px, py, plot);
			int zoneClass = findBackgroundClassOfPoint(px, py, plot);
			// check if plot has similar attributes
			// 
			bool isSingleAttributePlot = false;
			if (data.parsedAttributePairs[plot][0] == data.parsedAttributePairs[plot][1]) {
				isSingleAttributePlot = true;
			}

			//debug
			if (zone < 0) {
				std::cout << "debug message!";
			}

            // make zone color depdendant on accuracy, density
            // TODO:
			GLubyte backgroundTransparencyCopy = (GLubyte)backgroundTransparency;
			const GLubyte maxVal = 245;
			if (isBackgroundDensityColoringMode && zone >= 0 && zoneClass >= 0) {
				if (isSingleAttributePlot) {
					int singleAttributePlotSum = data.plotNumToTotalCases[plot].size();
					backgroundTransparencyCopy = min(((float)singleAttributePlotSum / (float)data.maxCasesPerPlotZone) * maxVal +backgroundTransparency, maxVal);
				}
				else if (zoneClass >= 0 && data.maxCasesPerPlotZone != 0) {
					backgroundTransparencyCopy = min(((float)data.plotNumZoneTotalCases[plot][zone].size() / (float)data.maxCasesPerPlotZone) * maxVal + backgroundTransparency, maxVal);
					if (backgroundLightnessSet.find(backgroundTransparencyCopy) == backgroundLightnessSet.end()) {
						backgroundLightnessSet.insert(backgroundTransparencyCopy);
					}
				}
			}

			// TODO: handle multiple decision classes
			std::vector<float> hsl;
			if (classNumber < 0) {
				hsl = RGBtoHSL(data.continueClassColor[classNumber]);
			}
			else
			{
				// we need to adjust the lightness of the background color
				hsl = RGBtoHSL(data.classColor[classNumber]);
			}
			// we need to adjust the lightness of the background color

			hsl[2] = hsl[2] * backgroundClassColorCoefficient;

			std::vector<GLubyte> rgb = HSLtoRGB(hsl);

			glColor4ub(rgb[0], rgb[1], rgb[2], backgroundTransparencyCopy);





			glRectf(x1, y1, x2, y2);


			// We need to draw a dotted line from x1, y1 to x1, y2 and another one from x1, y1, to x2, y1
			// Due to potential incompatibilities with dictating line width, we can use thin polygones as a workaround
			/*GLfloat lineThickness = 3.0f;
			GLfloat dashSpace = 10.0f;
			GLfloat currentColor[4];
			glGetFloatv(GL_CURRENT_COLOR, currentColor);
			glColor4f(currentColor[0], currentColor[1], currentColor[2], 1.0f);*/

			//// Left dashed line, from top left
			//bool drawLeftColor = true;
			//const GLfloat x1Backup = x1;

			//// todo: built for single line plots
			//for (GLfloat y = y1; y >= y2; y -= dashSpace) {
			//	if (y < y2) {
			//		y = y2;
			//	}

			//	// We need to sample points on either side of the line and only draw the dashed line if the two points are different colors
			//	// this is going to be super inefficient
			//	int pointClass = getClassNumFromPoint(x1, y, p);

			//	// Check if of the same class
			//	if (pointClass == -200 || pointClass == classNumber) { // todo: adjust. there could be 200 decision zones per plot
			//		// don't draw if our test point is not a member of any background rectangle.
			//		// this means that we are on the left or the right edge
			//		// don't draw if the adaject class is the same as our current class
			//		continue;
			//	}

			//	if (drawLeftColor) {
			//		if (pointClass < 0) {
			//			glColor4ub(0, 0, 0, 255);
			//		}
			//		else {
			//			std::vector<float> pointClassColor = { data.classColor[pointClass][0], data.classColor[pointClass][1], data.classColor[pointClass][2] };
			//			std::vector<float> pointClassColorHSL = RGBtoHSL(pointClassColor);
			//			pointClassColorHSL[2] *= backgroundClassColorCoefficient;
			//			std::vector<GLubyte> pointClassColorRGBModified = HSLtoRGB(pointClassColorHSL);
			//			glColor4ub(pointClassColorRGBModified[0], pointClassColorRGBModified[1], pointClassColorRGBModified[2], 255);

			//		}
			//	}
			//	else {
			//		if (classNumber < 0) {
			//			glColor4ub(0, 0, 0, 255);
			//		}
			//		else {
			//			glColor3f(currentColor[0], currentColor[1], currentColor[2]);
			//		}
			//	}

			//	drawLeftColor = !drawLeftColor;

			//	glBegin(GL_POLYGON);
			//	glVertex2f(x1 + lineThickness / 2, y);
			//	glVertex2f(x1 - lineThickness / 2, y);
			//	if (y - dashSpace <= y2) {
			//		glVertex2f(x1 - lineThickness / 2, y2);
			//		glVertex2f(x1 + lineThickness / 2, y2);
			//	}
			//	else {
			//		glVertex2f(x1 - lineThickness / 2, y - dashSpace);
			//		glVertex2f(x1 + lineThickness / 2, y - dashSpace);
			//	}
			//	glEnd();
			//}

			//bool drawTopColor = true;
			//// Draw bottom line
			//// todo: built for single line plots
			//for (GLfloat x = x1; x <= x2; x += dashSpace) {
			//	if (x > x2) {
			//		x = x2;
			//	}

			//	// We need to sample points on either side of the line and only draw the dashed line if the two points are different colors
			//	// this is going to be super inefficient
			//	int pointClass = getClassNumFromPoint(x, y2, p);

			//	// Check if of the same class
			//	if (pointClass == -200 || pointClass == classNumber) {
			//		// don't draw if our test point is not a member of any background rectangle.
			//		// this means that we are on the left or the right edge
			//		// don't draw if the adaject class is the same as our current class
			//		continue;
			//	}

			//	if (drawTopColor) {
			//		if (classNumber < 0) {
			//			glColor4ub(0, 0, 0, 255);
			//		} else {
			//			glColor3f(currentColor[0], currentColor[1], currentColor[2]);
			//		}
			//	}
			//	else {
			//		if (pointClass < 0) {
			//			glColor4ub(0, 0, 0, 255);
			//		}
			//		else {
			//			std::vector<float> pointClassColor = { data.classColor[pointClass][0], data.classColor[pointClass][1], data.classColor[pointClass][2] };
			//			std::vector<float> pointClassColorHSL = RGBtoHSL(pointClassColor);
			//			pointClassColorHSL[2] *= backgroundClassColorCoefficient;
			//			std::vector<GLubyte> pointClassColorRGBModified = HSLtoRGB(pointClassColorHSL);
			//			glColor4ub(pointClassColorRGBModified[0], pointClassColorRGBModified[1], pointClassColorRGBModified[2], 255);
			//		}
			//	}

			//	drawTopColor = !drawTopColor;

			//	glBegin(GL_POLYGON);
			//	glVertex2f(x, y2 + lineThickness / 2);
			//	glVertex2f(x, y2 - lineThickness / 2);
			//	if (x + dashSpace >= x2) {
			//		glVertex2f(x2, y2 - lineThickness / 2);
			//		glVertex2f(x2, y2 + lineThickness / 2); 
			//	}
			//	else {
			//		glVertex2f(x + dashSpace, y2 - lineThickness / 2);
			//		glVertex2f(x + dashSpace, y2 + lineThickness / 2);
			//	}

			//	glEnd();
			//}
		}
	}

	// Draws rectangle if rectangle mode is enabled
	if (isRectangleMode) {
		drawRectangle();
	}

	// TODO: We need to shake up the draw order!!!! Draw relative to pairs, not columns!!!!!
	// for every point, we need to draw it. What constitutes a point? 
	// Well that's in the attribute pairs map which is in the data class
	
	//debug
	//for (int i = 0; i < data.numPlots; i++) {
	//	drawData(.355, 0.5, 0, i);
	//}



	for (int recordNum = 0; recordNum < data.normalizedValues.size(); recordNum++) {
		int plotToDrawNext = 0;
		while (plotToDrawNext != -1) {
			int plotNum = plotToDrawNext;
			std::vector<std::string> attributePairAndPlotNum = data.parsedAttributePairs[plotNum];
			std::string attr1 = attributePairAndPlotNum[0];
			std::string attr2 = attributePairAndPlotNum[1];

			if (attr1 == attr2) {
				std::cout << "debug";
			}

			int attr1Index = data.attributeNameToDataIndex[attr1];
			int attr2Index = data.attributeNameToDataIndex[attr2];

			/*float x = data.xdata[recordNum][attr1Index];
			float y = data.ydata[recordNum][attr2Index];*/
			float x = data.normalizedValues[recordNum][attr1Index];
			float y = data.normalizedValues[recordNum][attr2Index];

			//if (attr1Index == attr2Index) {
			//	// this means that the two attributes are the same so we are on a single attribute plot
			//	y = x;
			//}

			plotToDrawNext = drawData(x, y, recordNum, plotNum);
		}

		//for (int i = 0; i < plotDrawOrder.size(); i++) {
		//	int plotNum = plotDrawOrder[i];
		//	if (plotNum > data.dataTerminationIndex[recordNum]) {
		//		break; // hmm problem. plots arent really ordered in a way that's meaningful to the decision tree
		//		// well, technically they are??? So then why arent we rendering the last point
		//	}
		//	std::vector<std::string> attributePairAndPlotNum = data.parsedAttributePairs[plotNum];
		//	std::string attr1 = attributePairAndPlotNum[0];
		//	std::string attr2 = attributePairAndPlotNum[1];
		//	int attr1Index = data.attributeNameToDataIndex[attr1];
		//	int attr2Index = data.attributeNameToDataIndex[attr2];

		//	float x = data.xdata[recordNum][attr1Index];
		//	float y = data.ydata[recordNum][attr2Index];

		//	int plotToDrawNext = drawData(x, y, recordNum, plotNum);
		//}
		//for (int plotNum = 0; plotNum < data.parsedAttributePairs.size(); plotNum++) {
		//	if (plotNum > data.dataTerminationIndex[recordNum]) {
		//		continue; // hmm problem. plots arent really ordered in a way that's meaningful to the decision tree
		//		// well, technically they are??? So then why arent we rendering the last point
		//	}
		//	std::vector<std::string> attributePairAndPlotNum = data.parsedAttributePairs[plotNum];
		//	std::string attr1 = attributePairAndPlotNum[0];
		//	std::string attr2 = attributePairAndPlotNum[1];
		//	int attr1Index = data.attributeNameToDataIndex[attr1];
		//	int attr2Index = data.attributeNameToDataIndex[attr2];

		//	float x = data.xdata[recordNum][attr1Index];
		//	float y = data.ydata[recordNum][attr2Index];

		//	drawData(x, y, recordNum, plotNum);
		//}
	}


	//for (int recordNum = 0; recordNum < data.xdata.size(); recordNum++) {
	//	for (int plotNum = 0; plotNum < data.parsedAttributePairs.size(); plotNum++) {
	//		std::vector<std::string> attributePairAndPlotNum = data.parsedAttributePairs[plotNum];
	//		std::string attr1 = attributePairAndPlotNum[0];
	//		std::string attr2 = attributePairAndPlotNum[1];
	//		int attr1Index = data.attributeNameToDataIndex[attr1];
	//		int attr2Index = data.attributeNameToDataIndex[attr2];

	//		float x = data.xdata[recordNum][attr1Index];
	//		float y = data.ydata[recordNum][attr2Index];

	//		drawData(x, y, recordNum, plotNum);
	//	}
	//}

	//data.drawLabels();

	return;

	// OLD CODE FOR OLD DRAWING METHOD. WILL PROBABLY MAKE TOGGLEABLE
	// TODO

	/* Plots the data. Outer loop for each dimension. Inner loop for data across each graph.  */
	//if (data.numPlots > 1)
	//{
	//	int g;
	//	for (int i = 0; i < data.xdata.size(); i++) 
	//	{
	//		g =  1;
	//		for (int j = 0; j < data.numPlots - 1; j++)
	//		{
	//			if (data.classNum[i] != (data.classToDisplayOnTop ))
	//			{
	//				// gove current point and next point to draw function
	//				drawData(data.xPlotCoordinates[j], data.yPlotCoordinates[j],
	//					data.xPlotCoordinates[j + 1], data.yPlotCoordinates[j + 1], i, j); //((1 + currentDepth) % data.xdata.size());
	//			}


	//		}

	//	}

	//	for (int i = 0; i < data.xdata.size(); i++)
	//	{
	//		g = 1;
	//		for (int j = 0; j < data.numPlots - 1; j++)
	//		{
	//			if (data.classNum[i] == (data.classToDisplayOnTop) )
	//			{
	//				drawData(data.xPlotCoordinates[j], data.yPlotCoordinates[j],
	//					data.xPlotCoordinates[j + 1], data.yPlotCoordinates[j + 1], i, j); //((1 + currentDepth) % data.xdata.size());
	//			}

	//		}

	//	}
	//}
	//else
	//{
	//	for (int i = 0; i < data.xdata.size(); i++) {

	//		for (int j = 0; j < data.numPlots; j++)
	//		{
	//			drawData(data.xPlotCoordinates[data.numPlots -1-j], data.yPlotCoordinates[j],
	//				data.xPlotCoordinates[j+1], data.yPlotCoordinates[j+1], i, j);
	//		}
	//	}
	//}


	//data.drawLabels();
	
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
		result = shouldLineBeClipped(x1, y1, x2, y2, startPointCode, endPointCode);
		break;
	default:
		break;
	}

	return result;
}

bool InteractiveSPC::shouldLineBeClipped(GLfloat startX, GLfloat startY, GLfloat endX, GLfloat endY, bool* startPointCode, bool* endPointCode) {
	GLfloat boundingBoxTop = max(rectY1, rectY2);
	GLfloat boundingBoxBottom = min(rectY1, rectY2);
	GLfloat boundingBoxLeft = min(rectX1, rectX2);
	GLfloat boundingBoxRight = max(rectX1, rectX2);

	bool* outCodeStart  = getPointTrivialityCode(startX, startY, rectX1, rectY1, rectX2, rectY2);
	bool* outCodeEnd = getPointTrivialityCode(endX, endY, rectX1, rectY1, rectX2, rectY2);

	bool isAccepted = false;

	while (true) {
		bool outCodeStartInRect = !outCodeStart[0] && !outCodeStart[1] && !outCodeStart[2] && !outCodeStart[3];
		bool outCodeEndInRect = !outCodeEnd[0] && !outCodeEnd[1] && !outCodeEnd[2] && !outCodeEnd[3];

		bool doPointsShareAZone = false;

		for (int i = 0; i < 4; i++) {
			if (outCodeStart[i] && outCodeEnd[i]) {
				doPointsShareAZone = true;
				break;
			}
		}

		if (outCodeStartInRect && outCodeEndInRect) {
			isAccepted = true;
			break;
		}
		else if (doPointsShareAZone) {
			break;
		}
		else {
			double x, y;

			bool* outCodeOut = outCodeStart;
			bool pointChosen = false;
			for (int i = 0; i < 4; i++) {
				if (outCodeStart[i] && !outCodeEnd[i]) {
					break;
				}
				if (!outCodeStart[i] && outCodeEnd[i]) {
					outCodeOut = outCodeEnd;
					break;
				}
			}

			for (int i = 0; i < 4; i++) {
				if (outCodeOut[i]) {
					switch (i) {
					case 0: // left
						y = startY + (endY - startY) * (boundingBoxLeft - startX) / (endX - startX);
						x = boundingBoxLeft;
						break;
					case 1: // top
						x = startX + (endX - startX) * (boundingBoxTop - startY) / (endY - startY);
						y = boundingBoxTop;
						break;
					case 2: // right
						y = startY + (endY - startY) * (boundingBoxRight - startX) / (endX - startX);
						x = boundingBoxRight;
						break;
					case 3: // bottom
						x = startX + (endX - startX) * (boundingBoxBottom - startY) / (startY - endY);
						y = boundingBoxBottom;
						break;
					default:
						break;
					}
					break;
				}
			}

			bool doesOutCodeOutEqualOutCodeStart = true;
			for (int i = 0; i < 4; i++) {
				if (outCodeOut[i] != outCodeStart[i]) {
					doesOutCodeOutEqualOutCodeStart = false;
					break;
				}
			}

			if (doesOutCodeOutEqualOutCodeStart) {
				startX = x;
				startY = y;
				outCodeStart = getPointTrivialityCode(startX, startY, rectX1, rectY1, rectX2, rectY2);
			}
			else {
				endX = x;
				endY = y;
				outCodeEnd = getPointTrivialityCode(endX, endY, rectX1, rectY1, rectX2, rectY2);
			}
		}
	}

	// Debug
	/*glColor4ub(0, 255, 0, 255);
	glPointSize(8.0);
	glBegin(GL_POINTS);
	glVertex2f(startX, startY);
	glColor4ub(255, 255, 0, 255);
	glVertex2f(endX, endY);
	glEnd();
	glPointSize(4.0);

	glBegin(GL_LINES);
	glVertex2f(startX, startY);
	glVertex2f(endX, endY);
	glEnd();

	glColor4ub(255, 0, 0, 255);
	glBegin(GL_LINES);
	glVertex2f(0.0f, boundingBoxBottom);
	glVertex2f(1000, boundingBoxBottom);
	glColor4ub(0, 255, 0, 255);
	glVertex2f(0.0f, boundingBoxTop);
	glVertex2f(1000, boundingBoxTop);
	glColor4ub(0, 0, 255, 255);
	glVertex2f(boundingBoxLeft, 0.0f);
	glVertex2f(boundingBoxLeft, 1000);
	glColor4ub(255, 0, 255, 255);
	glVertex2f(boundingBoxRight, 0.0f);
	glVertex2f(boundingBoxRight, 1000);
	glEnd();*/

	return isAccepted;
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
	else if (px > boundingBoxRight) {
		triviality[2] = true;
	}
	if (py > boundingBoxTop) {
		triviality[1] = true;
	} 
	else if (py < boundingBoxBottom) {
		triviality[3] = true;
	}

	return triviality;
}

void InteractiveSPC::calculateDataTerminationPoints() {
	for (int i = 0; i < data.dataTerminationIndex.size(); i++) {
		int classnum = data.classNum[i] - 1;
		for (int j = 0; j < data.dataTerminationIndex[i]; i++) {
			float px = data.xPlotCoordinates[j];
			float py = data.yPlotCoordinates[j];
			px -= (data.plotWidth[j] / 2);
			py += (data.plotHeight[j] / 2);
			float x1Coord = data.plotWidth[j] * data.xdata[i][j];
			float y1Coord = -data.plotHeight[j] * data.ydata[i][j]; //height of graph is constant = 328.5
			float x1CoordTrans = x1Coord + (px + data.pan_x);
			float y1CoordTrans = y1Coord + (py + data.pan_y);

			int backgroundClass = findBackgroundClassOfPoint(x1CoordTrans, y1CoordTrans);


			if (backgroundClass != -1) {
				if (backgroundClass != classnum) { // do something special if point class doesnt match background zone
				// TODO
				// debug
					//glColor4ub(255, 0, 0, 255);
					//glPointSize(8.0);
					//glBegin(GL_POINTS);
					//glVertex2f(x1CoordTrans, y1CoordTrans);
					//glEnd();
					//glPointSize(4.0);
				}
				else {
					data.dataTerminationIndex[i] = j;
					// debug
					/*glColor4ub(0, 0, 255, 255);
					glPointSize(8.0);
					glBegin(GL_POINTS);
					glVertex2f(x1CoordTrans, y1CoordTrans);
					glEnd();
					glPointSize(4.0);*/
				}
			}
		}
	}
}

// Dragging Graphs
float InteractiveSPC::findClickedGraph(double x, double y) 
{
	for (int i = 0; i < data.numPlots; i++) 
	{
		//for (int i1 = 0; i1 < data.graphwidth.size(); i1++)
		//{
			if (data.xPlotCoordinates[i] + data.pan_x - (data.plotWidth[i] / 2) <= x &&
				data.xPlotCoordinates[i] + data.pan_x + (data.plotWidth[i] / 2) >= x &&
				data.yPlotCoordinates[i] + data.pan_y - (data.plotHeight[i] / 2) <= y &&
				data.yPlotCoordinates[i] + data.pan_y + (data.plotHeight[i] / 2) >= y)
			{

				return i;
			}
		//}

		//original code
		
	} return -1;
}

int InteractiveSPC::findClickedCoordinate(double x, double y)
{
	for (int i = 0; i < data.numPlots; i++)
	{

			if (x <= (data.yPlotCoordinates[i] - 150) && x>= (data.yPlotCoordinates[i]-500))
			{
				return 0;
			}

			else if (data.xPlotCoordinates[i] - 25 <= x &&
				data.xPlotCoordinates[i] + 25 >= x)
			{
				return 1;
			}

		
	} return -1;
}

int InteractiveSPC::findBackgroundClassOfPoint(GLfloat px, GLfloat py) {
	// TODO
	// get plot num
	int plotNum = findPlotNumOfPoint(px,py);
	// check all rects inside plotnum
	for (int parsedIndex = 0; parsedIndex < dataParsed.parsedData.size(); parsedIndex++) {
		std::vector<float> parserData = dataParsed.parsedData[parsedIndex];
		if (parserData[4] != plotNum) continue;
		const GLfloat x1 = data.xPlotCoordinates[parserData[4]] - data.plotWidth[parserData[4]] / 2 + parserData[0] * data.plotWidth[parserData[4]];
		const GLfloat y1 = data.yPlotCoordinates[parserData[4]] + data.plotHeight[parserData[4]] / 2 - parserData[1] * data.plotHeight[parserData[4]];
		const GLfloat x2 = data.xPlotCoordinates[parserData[4]] - data.plotWidth[parserData[4]] / 2 + parserData[2] * data.plotWidth[parserData[4]];
		const GLfloat y2 = data.yPlotCoordinates[parserData[4]] + data.plotHeight[parserData[4]] / 2 - parserData[3] * data.plotHeight[parserData[4]];
		if (isPointWithinRect(px, py, x1, y1, x2, y2)) {
			return parserData[5];
		}
	}
	return INT_MIN;

 //   int resultClass = -1;
	//for (int p = 0; p < dataParsed.parsedData.size(); p++) { // Will we need to state which graph we are looking at?
 //       int classNumber = dataParsed.parsedData[p][5];
	//	const GLfloat x1 = data.xPlotCoordinates[dataParsed.parsedData[p][4]] - data.plotWidth[dataParsed.parsedData[p][4]] / 2 + dataParsed.parsedData[p][0] * data.plotWidth[dataParsed.parsedData[p][4]];
	//	const GLfloat y1 = data.yPlotCoordinates[dataParsed.parsedData[p][4]] + data.plotHeight[dataParsed.parsedData[p][4]] / 2 - dataParsed.parsedData[p][1] * data.plotHeight[dataParsed.parsedData[p][4]];
	//	const GLfloat x2 = data.xPlotCoordinates[dataParsed.parsedData[p][4]] - data.plotWidth[dataParsed.parsedData[p][4]] / 2 + dataParsed.parsedData[p][2] * data.plotWidth[dataParsed.parsedData[p][4]];
	//	const GLfloat y2 = data.yPlotCoordinates[dataParsed.parsedData[p][4]] + data.plotHeight[dataParsed.parsedData[p][4]] / 2 - dataParsed.parsedData[p][3] * data.plotHeight[dataParsed.parsedData[p][4]];
 //       if (isPointWithinRect(px, py, x1, y1, x2, y2)) {
 //           resultClass = classNumber;
	//		break;
 //       } // Check to see if these need to be rearranged.
	//	std::cout << "debug" << x1 << x2 << y1 << y2;
	//}

	//if (resultClass != -1) {
	//	std::cout << "debug";
	//}

 //   return resultClass;
}

int InteractiveSPC::findBackgroundZoneIdOfPoint(GLfloat px, GLfloat py, int plotNum) {
    // TODO
	// get plot num
	//int plotNum = findPlotNumOfPoint(px, py);
	// check all rects inside plotnum
	for (int parsedIndex = 0; parsedIndex < dataParsed.parsedData.size(); parsedIndex++) {
		std::vector<float> parserData = dataParsed.parsedData[parsedIndex];
		if ((int)parserData[4] != plotNum) {
			std::cout << "debug: I wonder if there's some sort of float / int comparison issue";
			continue;
		}
		// TODO: There's probably a MUCH easier way to do this
		// need to accomodate various swappings
		// X/Y swap swaps parser data itself, so we need not do anything here
		// but the x and y invert buttons DONT alter data, so we need to make up for that here
		const float zoneX1 = parserData[0]; 
		const float zoneY1 = parserData[1]; 
		const float zoneX2 = parserData[2]; 
		const float zoneY2 = parserData[3]; 

		const float pltX1 = data.x1CoordPlot[plotNum];
		const float pltY1 = data.y1CoordPlot[plotNum];
		const float pltX2 = data.x2CoordPlot[plotNum];
		const float pltY2 = data.y2CoordPlot[plotNum];

		const float plotWidth = data.plotWidth[plotNum];
		const float plotHeight = data.plotHeight[plotNum];

		GLfloat zoneToCheckX1;
		GLfloat zoneToCheckX2;
		GLfloat zoneToCheckY1;
		GLfloat zoneToCheckY2;

		// old
		//zoneToCheckX1 = pltX1 + plotWidth * zoneX1 + data.pan_x;
		//zoneToCheckX2 = pltX1 + plotWidth * zoneX2 + data.pan_x;
		//zoneToCheckY2 = pltY2 - plotHeight * zoneY2 + data.pan_y;
		//zoneToCheckY1 = pltY2 - plotHeight * zoneY1 + data.pan_y;

		if (plotsWithXAxisInverted.size() != 0 && plotsWithXAxisInverted.find(plotNum) != plotsWithXAxisInverted.end()) {
			zoneToCheckX2 = pltX2 - plotWidth * zoneX1 + data.pan_x;
			zoneToCheckX1 = pltX2 - plotWidth * zoneX2 + data.pan_x;
		}
		else {
			zoneToCheckX1 = pltX1 + plotWidth * zoneX1 + data.pan_x;
			zoneToCheckX2 = pltX1 + plotWidth * zoneX2 + data.pan_x;
		}
		if (plotsWithYAxisInverted.size() != 0 && plotsWithYAxisInverted.find(plotNum) != plotsWithYAxisInverted.end()) {
			zoneToCheckY1 = pltY1 + plotHeight * zoneY2 + data.pan_y;
			zoneToCheckY2 = pltY1 + plotHeight * zoneY1 + data.pan_y;
		}
		else {
			zoneToCheckY2 = pltY2 - plotHeight * zoneY2 + data.pan_y;
			zoneToCheckY1 = pltY2 - plotHeight * zoneY1 + data.pan_y;
		}

		// debug
		//glBegin(GL_POINTS);
		//glColor4ub(255, 0, 0, 255);
		//glVertex2f(zoneToCheckX1, zoneToCheckY1);
		//glColor4ub(0, 255, 0, 255);
		//glVertex2f(zoneToCheckX2, zoneToCheckY2);
		//glEnd();
		// end debug

		bool withinRect = isPointWithinRect(px, py, zoneToCheckX1, zoneToCheckY1, zoneToCheckX2, zoneToCheckY2);

		if (withinRect) {
			return parsedIndex;
		}
	}

	return INT_MIN;
}

int InteractiveSPC::findBackgroundClassOfPoint(GLfloat px, GLfloat py, int plotNum) {
	// TODO
	// get plot num
	//int plotNum = findPlotNumOfPoint(px, py);
	// check all rects inside plotnum
	for (int parsedIndex = 0; parsedIndex < dataParsed.parsedData.size(); parsedIndex++) {
		std::vector<float> parserData = dataParsed.parsedData[parsedIndex];
		if ((int)parserData[4] != plotNum) {
			std::cout << "debug: I wonder if there's some sort of float / int comparison issue";
			continue;
		}
		// TODO: There's probably a MUCH easier way to do this
		// need to accomodate various swappings
		// X/Y swap swaps parser data itself, so we need not do anything here
		// but the x and y invert buttons DONT alter data, so we need to make up for that here
		const float zoneX1 = parserData[0]; 
		const float zoneY1 = parserData[1]; 
		const float zoneX2 = parserData[2]; 
		const float zoneY2 = parserData[3]; 

		const float pltX1 = data.x1CoordPlot[plotNum];
		const float pltY1 = data.y1CoordPlot[plotNum];
		const float pltX2 = data.x2CoordPlot[plotNum];
		const float pltY2 = data.y2CoordPlot[plotNum];

		const float plotWidth = data.plotWidth[plotNum];
		const float plotHeight = data.plotHeight[plotNum];

		GLfloat zoneToCheckX1;
		GLfloat zoneToCheckX2;
		GLfloat zoneToCheckY1;
		GLfloat zoneToCheckY2;

		// old
		//zoneToCheckX1 = pltX1 + plotWidth * zoneX1 + data.pan_x;
		//zoneToCheckX2 = pltX1 + plotWidth * zoneX2 + data.pan_x;
		//zoneToCheckY2 = pltY2 - plotHeight * zoneY2 + data.pan_y;
		//zoneToCheckY1 = pltY2 - plotHeight * zoneY1 + data.pan_y;

		if (plotsWithXAxisInverted.size() != 0 && plotsWithXAxisInverted.find(plotNum) != plotsWithXAxisInverted.end()) {
			zoneToCheckX2 = pltX2 - plotWidth * zoneX1 + data.pan_x;
			zoneToCheckX1 = pltX2 - plotWidth * zoneX2 + data.pan_x;
		}
		else {
			zoneToCheckX1 = pltX1 + plotWidth * zoneX1 + data.pan_x;
			zoneToCheckX2 = pltX1 + plotWidth * zoneX2 + data.pan_x;
		}
		if (plotsWithYAxisInverted.size() != 0 && plotsWithYAxisInverted.find(plotNum) != plotsWithYAxisInverted.end()) {
			zoneToCheckY1 = pltY1 + plotHeight * zoneY2 + data.pan_y;
			zoneToCheckY2 = pltY1 + plotHeight * zoneY1 + data.pan_y;
		}
		else {
			zoneToCheckY2 = pltY2 - plotHeight * zoneY2 + data.pan_y;
			zoneToCheckY1 = pltY2 - plotHeight * zoneY1 + data.pan_y;
		}

		// debug
		//glBegin(GL_POINTS);
		//glColor4ub(255, 0, 0, 255);
		//glVertex2f(zoneToCheckX1, zoneToCheckY1);
		//glColor4ub(0, 255, 0, 255);
		//glVertex2f(zoneToCheckX2, zoneToCheckY2);
		//glEnd();
		// end debug

		bool withinRect = isPointWithinRect(px, py, zoneToCheckX1, zoneToCheckY1, zoneToCheckX2, zoneToCheckY2);

		if (withinRect) {
			return parserData[5];
		}
	}

	return INT_MIN;
}



int InteractiveSPC::findPlotNumOfPoint(GLfloat px, GLfloat py) {
	for (int i = 0; i < data.numPlots; i++) {
		float x1 = data.x1CoordPlot[i];
		float y1 = data.y1CoordPlot[i];
		float x2 = data.x2CoordPlot[i];
		float y2 = data.y2CoordPlot[i];

		if (px >= x1 && px <= x2) {
			if (py >= y1 && py <= y2) {
				return i;
			}
		}
	}

	return -1;
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
	
	//for (int currentDepth = 0; currentDepth < numOfClasses; currentDepth++) {
	//	classColor.push_back({});
	//	//0, (data.classNum[currentDepth] * 50) + 1, 100
	//	classColor[currentDepth].push_back(0);
	//	classColor[currentDepth].push_back((currentDepth + 1) * 50 + 1);
	//	classColor[currentDepth].push_back(100);
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
	for (int i = 0; i < rectX1List.size(); i++) {
		drawRectangle(rectX1List[i], rectX2List[i], rectY1List[i], rectY2List[i], 0.0f, 0.0f, 0.0f);
	}
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
	hsl.push_back(hue % 360);
	hsl.push_back(saturation);
	hsl.push_back(lightness);
	return hsl;
}

bool InteractiveSPC::isPointWithinRect(GLfloat px, GLfloat py, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
	if (px <= x2 && px >= x1) {
		if (py <= y1 && py >= y2) {
			return true;
		}
	}
	return false;
}

int InteractiveSPC::getClassNumFromPoint(GLfloat px, GLfloat py, int currentDataIndex) {
	int dataIndex = -100;

	// check for termination classes
	for (int i = 0; i < dataParsed.parsedData.size(); i++) {
		if (i == currentDataIndex) continue;
		GLfloat testRectx1 = data.xPlotCoordinates[dataParsed.parsedData[i][4]] - data.plotWidth[dataParsed.parsedData[i][4]] / 2 + dataParsed.parsedData[i][0] * data.plotWidth[dataParsed.parsedData[i][4]];
		GLfloat testRecty1 = data.yPlotCoordinates[dataParsed.parsedData[i][4]] + data.plotHeight[dataParsed.parsedData[i][4]] / 2 - dataParsed.parsedData[i][1] * data.plotHeight[dataParsed.parsedData[i][4]];
		GLfloat testRectx2 = data.xPlotCoordinates[dataParsed.parsedData[i][4]] - data.plotWidth[dataParsed.parsedData[i][4]] / 2 + dataParsed.parsedData[i][2] * data.plotWidth[dataParsed.parsedData[i][4]];
		GLfloat testRecty2 = data.yPlotCoordinates[dataParsed.parsedData[i][4]] + data.plotHeight[dataParsed.parsedData[i][4]] / 2 - dataParsed.parsedData[i][3] * data.plotHeight[dataParsed.parsedData[i][4]];

		if (isPointWithinRect(px, py, testRectx1, testRecty1, testRectx2, testRecty2)) {
			dataIndex = i;
			break;
		}
	}

	// check for continue classes





	if (dataIndex < 0) {
		return -200;
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