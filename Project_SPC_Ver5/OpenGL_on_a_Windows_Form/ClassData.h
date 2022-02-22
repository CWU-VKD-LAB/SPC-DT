///-------------------------------------------------------------------------------------------------
// file:	ClassData.h
//
// summary:	Declares the class data class
///-------------------------------------------------------------------------------------------------

#include "stdafx.h"
 #include <Windows.h>
#include <GL/glu.h>
#include "glut.h"
#include <locale>         // std::locale, std::isalpha
#include <fstream>
#include <map>
#include <set>

#pragma once

//currently setup for EACH INDIVIDUAL CLASS


struct Node {
public:
	std::vector<Node*> destinationList;
	int plotNum;
	int subtreeSpan = -1;
	int subtreeDepth = -1;

	Node(int plotNum, int destination) {
		this->plotNum = plotNum;
		addDestination(&Node(destination));
	}
	Node(int plotNum) {
		this->plotNum = plotNum;
	}
	void addDestination(Node* node) {
		destinationList.push_back(node);
	}

	void computeSubTreeMaxSpan() {
		subtreeSpan = computeSubtreeSpan();
	}

	void computeSubtreeDepth() {
		subtreeDepth = computeDepth();
	}

	int getSpanAtDepth(int depth) {
        if (depth == 0) {
            return 1;
        }

		if (depth > subtreeDepth) {
			return 0;
		}

		return getSpanAtDepthDFS(depth);
	}

	std::vector<Node*> getAllNodesAtDepth(int depth) {
		std::vector<Node*> nodes;
        if (depth == 0) {
            nodes.push_back(this);
            return nodes;
        }
        
		if (depth > subtreeDepth) {
			return nodes;
		}

		findAllNodesAtTargetDepth(nodes, depth);
        
		return nodes;
	}

private:
	void findAllNodesAtTargetDepth(std::vector<Node*>& nodeList, int targetDepth) {
		if (targetDepth == 0) {
			nodeList.push_back(this);
			return;
		}

		for (int i = 0; i < destinationList.size(); i++) {
			std::vector<Node*> tmpList = destinationList[i]->getAllNodesAtDepth(targetDepth - 1);
			if (tmpList.size() > 0) {
				for (int j = 0; j < tmpList.size(); j++) {
					nodeList.push_back(tmpList[j]);
				}
			}
		}

		return;
	}


	int getSpanAtDepthDFS(int targetDepth) {
		if (targetDepth == 0) {
			return 1;
		}

		if (targetDepth == 1) {
			return destinationList.size();
		}

		int spanAtDepth = 0;
		for (int i = 0; i < destinationList.size(); i++) {
			spanAtDepth += destinationList[i]->getSpanAtDepth(targetDepth - 1);
			
		}

		return spanAtDepth;
	}


	int computeSubtreeSpan() {
		if (destinationList.empty()) {
			return 1;
		}

		if (subtreeSpan != -1) {
			return subtreeSpan;
		}

		int span = 0;
		for (int i = 0; i < destinationList.size(); i++) {
			destinationList[i]->computeSubTreeMaxSpan();
			span += destinationList[i]->subtreeSpan;
		}

		return span;
	}

	int computeDepth() {
		if (destinationList.empty()) {
			return 1; // TODO: look into if this should be 0
		}

		if (subtreeDepth != -1) {
			return subtreeDepth;
		}

		int maxChildDepth = 0;
		for (int i = 0; i < destinationList.size(); i++) {
			destinationList[i]->computeSubtreeDepth();
			int childDepth = destinationList[i]->subtreeDepth;
			if (childDepth > maxChildDepth) {
				maxChildDepth = childDepth;
			}
		}

		return maxChildDepth + 1;
	}
};

struct Tree {
	Tree(Node* root) {
		this->root = root;
		buildNodeList(root);
		buildNodeMap();
	}

	Node* findPlotNum(int plotNum) {
		if (nodeMap.find(plotNum) != nodeMap.end()) {
			return nodeMap[plotNum];
		}
		return nullptr;
	}

	std::map<int, Node*> getNodeMap() {
		return nodeMap;
	}

	std::vector<Node*> getNodeList() {
		return nodeList;
	}
private:
	Node* root;
	std::vector<Node*> nodeList;
	std::map<int, Node*> nodeMap;

	std::vector<Node*> buildNodeList(Node* node) {
		std::vector<Node*> localNodeList;
		localNodeList.push_back(node);
		for (int i = 0; i < node->destinationList.size(); i++) {
			localNodeList = buildNodeList(node->destinationList[i]);
			for (int j = 0; j < localNodeList.size(); j++) {
				nodeList.push_back(localNodeList[j]);
			}
		}

		return localNodeList;
	}

	void buildNodeMap() {
		std::map<int, Node*> localNodeMap;
		for (int i = 0; i < nodeList.size(); i++) {
			localNodeMap.insert(std::pair<int, Node*>(nodeList[i]->plotNum, nodeList[i]));
		}
	}
};

class ClassData // copy class before changing
{
public: 
	std::vector<std::vector<float> > classColor;
	std::vector<float>  classTransparency;
	int classToDisplayOnTop;
	bool doesDecisionTreeBranch = false;
	Node* rootNode;

	void setClassColors() {
		for (int i = 0; i < numOfClasses; i++) {
			classColor.push_back({});
			//0, (data.classNum[i] * 50) + 1, 100
			classColor[i].push_back(0);
			classColor[i].push_back((i + 1) * 50 + 1);
			classColor[i].push_back(100);
			classTransparency.push_back(255);
		}
	}
	void setClassColor(float R, float G, float B, int classnum) {
		//if(!classColor[classnum - 1].empty())
		{
			classColor[classnum-1].clear();
			classColor[classnum - 1].push_back(R);
			classColor[classnum - 1].push_back(G);
			classColor[classnum - 1].push_back(B);
		}
	}
	void setClassTransparency(float alpha, int classNum) {
		
		//int size = dataTransparency.size();
		if (classNum < 0) { // Sets transparency of all data
			int size = classTransparencies.size();
			for (int i = 0; i < classTransparencies.size(); i++) {
				//dataTransparency[i] = alpha;
				classTransparencies[i] = alpha;
			}
		}
		else {
			
			classTransparencies[classNum] = alpha;
			
		}
	}

	/// <summary>	Filename of the file. </summary>
	std::string fileName;
	
	/// <summary>	The values. </summary>
	std::vector<std::vector<std::string> > values;

	/// <summary>	these are actually in OpenGL.h. </summary>
	double worldWidth;
	/// <summary>	Height of the world. </summary>
	double worldHeight;

	/// <summary>	The class number. </summary>
	std::vector<int> classNum;
	/// <summary>	Number of classes. </summary>
	int numOfClasses = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 	Holds class size, it is fixed at 4 for now, but should be changed when reading actual
	/// 	data.
	/// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	int classsize;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 	Holds the x max for all data. Used normalize data to the max for graphing purposes.
	/// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	double xmax;


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 	Holds the y max for all data. Used normalize data to the max for graphing purposes.
	/// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	double ymax;

	/// <summary>	Width size for each graph. </summary>
	//double plotWidth;
	std::vector<double> plotWidth; 
	/// <summary>	Height size for each graph. </summary>
	//double plotHeight;
	std::vector<double> plotHeight;
	//for storing orthogonal information for X and Y axes for each graph 
	std::vector<double> nonOrthoX1;
	std::vector<double> nonOrthoY1;
	std::vector<double> nonOrthoX2;
	std::vector<double> nonOrthoY2;

	std::vector<std::vector<float>> parsedData;
	std::vector<std::vector<std::string>> strparsedData;
	std::vector<std::vector<std::string>> parsedAttributePairs;
	//std::vector<int> plotDestinationList;
	std::map<int, std::map<int, int> > plotDestinationMap; // maps plotnum to a map of classnum -> destinationplot 
	double temprx2;
	double temprectAndCoordy2;
	//bool seeLabels;
	bool showHideLinesVar = true; 
	bool drawAndRect = false;
	bool drawOrRect = false;
	std::string xlabels = "X-Axes: ";
	std::string ylabels = "Y-Axes: ";

	/// <summary>	Holds different dimensions of x data. </summary>
	std::vector<std::vector<float> > xdata;
	std::vector<double> x1CoordPlot, x2CoordPlot, y1CoordPlot, y2CoordPlot;
	std::vector<float> dataTransparency;
	std::vector<float> classTransparencies;
	bool hdDisplay; //to display high dimension data
	int numOfPlotsPerRowHdDisplay;
	std::vector< std::vector<float>> originalXData;
	std::vector< std::vector<float>> originalYData;
	/// <summary>	Holds different dimensions of y data. </summary>
	std::vector<std::vector<float> > ydata;

	/// <summary> Holds information for when a point ends
	std::vector<int> dataTerminationIndex;
	//std::vector<ClassData> classes;

	/// <summary>	The initial xclasses. </summary>
	std::vector<std::vector<float> > initialXclasses;
	/// <summary>	The initial yclasses. </summary>
	std::vector<std::vector<float> > initialYclasses;

	/// <summary>	The xclasses. </summary>
	std::vector<std::vector<float> > xclasses;
	/// <summary>	The yclasses. </summary>
	std::vector<std::vector<float> > yclasses;

	// These hold the ORIGINAL x & y coordinates for each graph position.
	// Since we're using ClassData, this might get replaced
	/// <summary>	The originalxgraphcoordinates. </summary>
	std::vector<float> originalXPlotCoordinates;
	/// <summary>	The originalYPlotCoordinates. </summary>
	std::vector<float> originalYPlotCoordinates;

	/// <summary>	These hold x &amp; y coordinates for each graph position. </summary>
	std::vector<float> xPlotCoordinates;
	bool fOpen;
	/// <summary>	The yPlotCoordinates. </summary>
	std::vector<float> yPlotCoordinates;


	// for zooming and panning ////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	The pan x coordinate. </summary>
	double pan_x = 0.0;
	/// <summary>	The pan y coordinate. </summary>
	double pan_y = 0.0;
	/// <summary>	The scale. </summary>
	double scale = 1.2;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 	These are starting width and height. The world width and world height is the width after
	/// 	these starting values.
	/// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	double leftWidth = 0;
	/// <summary>	Height of the bottom. </summary>
	double bottomHeight = 0;
	/// <summary>	starting pan amount because zooming would need a different pan amount. </summary>
	int panamount = 22;


	void resetSomeVars() {
		numOfClasses = 0;
		classsize = 0;
		xmax = 0;
		ymax = 0;
		classColor.clear();
		leftWidth = 0;
		bottomHeight = 0;
	}



	ClassData() {};



	ClassData(std::string name)
	{
		fileName = name;
		//fOpen = false;
	};

	~ClassData()
	{
		// Delete as necessary.
		values.clear();
		xdata.clear();
		ydata.clear();
		initialXclasses.clear();
		initialYclasses.clear();
		xclasses.clear();
		yclasses.clear();
		originalXPlotCoordinates.clear();
		originalYPlotCoordinates.clear();
		xPlotCoordinates.clear();
		yPlotCoordinates.clear();
		classNum.clear();


	}

	/* Input: Position of graph -- Creates a new empty graph */

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Draw graph. </summary>
	///
	/// <remarks>	Moved from classes. </remarks>
	///
	/// <param name="x">	The x coordinate. </param>
	/// <param name="y">	The y coordinate. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void drawPlot(float x, float y, float plotW, float plotH, int i) 
	{

			glPushMatrix(); // Makes a new layer
			glTranslatef(x + pan_x, y + pan_y, 0.0f);	
			glScalef((plotW / 2), (plotH / 2), 0.0f);
			//glRotatef(45, 1, 1, 1);
			glBegin(GL_LINE_STRIP);
			glColor3ub(0, 0, 0); // Line color
			glVertex2f(nonOrthoX1[i], nonOrthoY1[i]); //defines Y axis -1 for 90 degrees, change the first element to change angle	
			glVertex2f(-1, 1);
			glVertex2f(nonOrthoX2[i], nonOrthoY2[i]); //defines X axis, change the second element to change angle	
			glEnd();
			glPopMatrix(); // Removes the layer
			//if (!fOpen)
			//{
			//	for (int p = 0; p < parsedData.size(); p++)
			//	{
			//		glColor4ub(255, 0, 0, 1);
			//		glRectf(xPlotCoordinates[parsedData[p][4]] - graphwidth[parsedData[p][4]] / 2 + parsedData[p][0] * graphwidth[parsedData[p][4]],
			//			    yPlotCoordinates[parsedData[p][4]] + graphheight[parsedData[p][4]] / 2 - parsedData[p][1] * graphheight[parsedData[p][4]],
			//			    xPlotCoordinates[parsedData[p][4]] - graphwidth[parsedData[p][4]] / 2 + parsedData[p][2] * graphwidth[parsedData[p][4]],
			//			    yPlotCoordinates[parsedData[p][4]] + graphheight[parsedData[p][4]] / 2 - parsedData[p][3] * graphheight[parsedData[p][4]]);
			//	}
			//	glColor4ub(255, 0, 0, 1);
			//	glRectf(xPlotCoordinates[0] - graphwidth[0] / 2, yPlotCoordinates[0] + graphheight[0] / 2,
			//		xPlotCoordinates[0] - graphwidth[0] / 2 + 0.255 * graphwidth[0], yPlotCoordinates[0] - graphheight[0] / 2);
			//	/*glRectf(data.xPlotCoordinates[1] - data.graphwidth[1] / 2, data.yPlotCoordinates[1] + data.graphheight[1] / 2,
			//		data.xPlotCoordinates[1] - data.graphwidth[1] / 2 + 0.458 * data.graphwidth[1], data.yPlotCoordinates[0] + data.graphheight[1] / 2 - 0.505 * data.graphheight[1]);
			//	glColor4ub(0, 255, 0, 1);
			//	glRectf(data.xPlotCoordinates[1] - data.graphwidth[1] / 2 + 0.458 * data.graphwidth[1], data.yPlotCoordinates[1] + data.graphheight[1] / 2,
			//		data.xPlotCoordinates[1] - data.graphwidth[1] / 2 + 1 * data.graphwidth[1], data.yPlotCoordinates[0] + data.graphheight[1] / 2 - 0.505 * data.graphheight[1]);
			//	glRectf(data.xPlotCoordinates[1] - data.graphwidth[1] / 2 + 0 * data.graphwidth[1], data.yPlotCoordinates[1] + data.graphheight[1] / 2 - 0.505 * data.graphheight[1],
			//		data.xPlotCoordinates[1] - data.graphwidth[1] / 2 + 1 * data.graphwidth[1], data.yPlotCoordinates[0] + data.graphheight[1] / 2 - 1 * data.graphheight[1]);
			//	glColor4ub(169, 169, 169, 6);
			//	glRectf(data.xPlotCoordinates[0] - data.graphwidth[0] / 2 + 0.255 * data.graphwidth[0], data.yPlotCoordinates[0] + data.graphheight[0] / 2 - 0 * data.graphheight[1],
			//		data.xPlotCoordinates[0] - data.graphwidth[0] / 2 + 1 * data.graphwidth[0], data.yPlotCoordinates[0] + data.graphheight[0] / 2 - 1 * data.graphheight[1]);*/
			//}
	}

	bool is_digits(const std::string &str)
	{
		return str.find_first_not_of("0123456789") == std::string::npos;
	}
	//late entry: separate function for adding x,y labels
	void getLabelsFromParser() {
		xlabels = "";
		ylabels = "";
		for (int i = 0; i < parsedAttributePairs.size(); i++) {
			if (i > 0) {
				xlabels += ", ";
				ylabels += ", ";
			}
			xlabels += parsedAttributePairs[i][0];
			ylabels += parsedAttributePairs[i][1];
		}
	}

	void getLabels()
	{
		//stores x axis labels, but skips first and last cell
		for (int label = 1; label < values[0].size() - 1; label++)
		{
			if (label > 1) xlabels += ",";
			if (!is_digits(values[0][label]))
				xlabels += values[0][label];
			else break;
		}

		//stores y axis labels
		for (int label = 1; label < values.size(); label++)
		{
			if (label > 1) ylabels += ",";
			if (!is_digits(values[label][0]))
				ylabels += values[label][0];
			else break;
		}
	}

	void drawBitmapText(const char *string, float x, float y)
	{
		const char *c;
		glRasterPos2f(x, y);

		int i = 0;
		for (c = string; *c != ' '; c++)
		{
			if (i < strlen(string))
			{
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
			}
			else
			{
				break;
			}
			i++;
		}
	}

	void drawLabels()
	{
		glColor3f(0, 0, 1);
		drawBitmapText(xlabels.c_str(), .2*worldWidth, .98*worldHeight);
		drawBitmapText(ylabels.c_str(), .05*worldWidth, .05*worldHeight);
	}

	void calculateTerminationPoints() {
		//int currentPlot = 0;
		//while (currentPlot < dataTerminationIndex[currentPlot]) {

		//	int backgroundClass = -1;
		//	if (backgroundClass >= 0) {
		//		currentPlot += 1;
		//	}
		//	else {
		//		
		//	}
		//}




		for (int i = 0; i < dataTerminationIndex.size(); i++) {
			int classnum = classNum[i] - 1;
			for (int j = 0; j < dataTerminationIndex[i]; j++) {
				float px = xPlotCoordinates[j];
				float py = yPlotCoordinates[j];
				px -= (plotWidth[j] / 2);
				py += (plotHeight[j] / 2);
				float x1Coord = plotWidth[j] * xdata[i][j];
				float y1Coord = -plotHeight[j] * ydata[i][j]; //height of plot is constant = 328.5
				float x1CoordTrans = x1Coord + (px + pan_x);
				float y1CoordTrans = y1Coord + (py + pan_y);

				int backgroundClass = findBackgroundClassOfPoint(x1CoordTrans, y1CoordTrans);

				if (backgroundClass >= 0) {
					std::cout << "debug";
					if (backgroundClass == classnum) {
						dataTerminationIndex[i] = j;
					}
				}
				else {

				}
			}
		}
	}


	void computeDecisionTreeBranches() {
		std::map<int, std::vector<int>> plotSourceDestinationMap;
		std::map<int, Node*> nodeMap;
		std::vector<Node*> nodeList;
		std::set<int> constructedNodes;

		// Construct nodes
		for (int i = 0; i < strparsedData.size(); i++) {
			// plot num
			int plotNum = stoi(strparsedData[i][4]);
			if (constructedNodes.empty() || constructedNodes.find(plotNum) == constructedNodes.end()) {
				Node * n = new Node(plotNum);
				nodeList.push_back(n);
				nodeMap[plotNum] = n;
				constructedNodes.insert(plotNum);
			}
		}

		// Construct tree of nodes
		// TODO: Optimization: parser generation places all continue elements at the top. Could save time to not cycle the whole list
		for (int i = 0; i < strparsedData.size(); i++) {
			if (strparsedData[i].size() == 9) {
				int plotNum = stoi(strparsedData[i][4]);
				int dest = stoi(strparsedData[i][8]);
				nodeMap[plotNum]->addDestination(nodeMap[dest]);
			}
		}

		rootNode = nodeMap[0]; // TODO: investigate why this is necessary
		
		rootNode->computeSubtreeDepth();
		rootNode->computeSubTreeMaxSpan();

		int rootDepth = rootNode->subtreeDepth;
		int rootSpan = rootNode->subtreeSpan;

		if (rootSpan > 1) {
			doesDecisionTreeBranch = true;
		}

		std::cout << "debug treespan" << rootSpan;
		std::cout << "debug treedepth" << rootDepth;
	}

	void calculateTerminationPoint(int i) {
		int classnum = classNum[i] - 1;
		for (int j = 0; j < dataTerminationIndex[i]; j++) {
			float px = xPlotCoordinates[j];
			float py = yPlotCoordinates[j];
			px -= (plotWidth[j] / 2);
			py += (plotHeight[j] / 2);
			float x1Coord = plotWidth[j] * xdata[i][j];
			float y1Coord = -plotHeight[j] * ydata[i][j]; //height of plot is constant = 328.5
			float x1CoordTrans = x1Coord + (px + pan_x);
			float y1CoordTrans = y1Coord + (py + pan_y);

			int backgroundClass = findBackgroundClassOfPoint(x1CoordTrans, y1CoordTrans);

			if (backgroundClass >= 0) {
				std::cout << "debug";
				if (backgroundClass == classnum) {
					if (j < 2) {
						dataTerminationIndex[i] = 2; // might need to be 2
					}
					else {
						dataTerminationIndex[i] = j;
					}
				}
			}
			else {
				// TODO
			}
		}
	}

	int findBackgroundClassOfPoint(GLfloat px, GLfloat py) {
		// TODO
		int resultClass;
		for (int p = 0; p < parsedData.size(); p++) { // Will we need to state which plot we are looking at?
			int classNumber = parsedData[p][5];
			int plot = parsedData[p][4];
			GLfloat x1 = xPlotCoordinates[plot] - plotWidth[plot] / 2 + parsedData[p][0] * plotWidth[plot];
			GLfloat y1 = yPlotCoordinates[plot] + plotHeight[plot] / 2 - parsedData[p][1] * plotHeight[plot];
			GLfloat x2 = xPlotCoordinates[plot] - plotWidth[plot] / 2 + parsedData[p][2] * plotWidth[plot];
			GLfloat y2 = yPlotCoordinates[plot] + plotHeight[plot] / 2 - parsedData[p][3] * plotHeight[plot];
			if (isPointWithinRect(px, py, x1, y1, x2, y2)) {
				resultClass = classNumber;
			} // Check to see if these need to be rearranged.
			std::cout << "debug" << x1 << x2 << y1 << y2;
		}

		if (resultClass != -1) {
			std::cout << "debug";
		}

		return resultClass;
	}

	bool isPointWithinRect(GLfloat px, GLfloat py, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
		bool result = false;
		if (px <= x2 && px >= x1) {
			if (py <= y1 && py >= y2) {
				result = true;
			}
		}
		return result;
	}
};




class parseData // copy class before changing
{
public:
	std::string parserFileName;
	std::vector<std::vector<float>> parsedData;
	std::vector<std::vector<std::string>> strparsedData;
	std::vector<std::vector<std::string>> parsedAttributePairs;
	bool parserFileOpen;

	parseData() {};
	parseData(std::string name)
	{
		parserFileName = name;
		//bool parserFileOpen = false;
	};
};

