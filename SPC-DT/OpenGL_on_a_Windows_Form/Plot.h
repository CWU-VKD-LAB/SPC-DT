#include "stdafx.h"
#include "Zone.h"
#include "UserRectangle.h"
#include "DrawingUtils.h"
#include "Classification.h"
#pragma once

struct Plot {
	int plotNum;
	GLfloat centerX, centerY, width, height;
	std::vector<Zone> zones;
    std::vector<UserRectangle> userRects;
	std::string attribute1Name, attribute2Name;
	std::map<std::string, std::vector<float>>* attributeMinMax;
	std::vector<std::vector<std::string>> *parsedAttributePairs;
	std::map<std::string, int> *attributeNameToDataIndex;
	std::vector<std::vector<float>> *normalizedValues;
	std::vector<Plot> *plots;
	std::vector<float> *classTransparencies;
	std::vector<float>* caseColor;
	std::set<int> classifiedCases;
	std::set<Classification> misclassifiedCases;
	std::set<Classification> correctlyClassifiedCases;
	bool isXInverted, isYInverted, isXYSwapped, isSingleAttributePlot;

	Plot(int id, std::map<std::string, std::vector<float>>& attributeMinMax, std::string &attribute1Name, std::string &attribute2Name) {
		plotNum = id;
		this->attributeMinMax = &attributeMinMax;
		this->attribute1Name = attribute1Name;
		this->attribute2Name = attribute2Name;
		isXInverted = false;
		isYInverted = false;
		isXYSwapped = false;
		isSingleAttributePlot = false;
		if (attribute1Name == attribute2Name) {
			isSingleAttributePlot = true;
		}
	}

	Plot(GLfloat centerX, GLfloat centerY, GLfloat width, GLfloat height) {
		this->centerX = centerX;
		this->centerY = centerY;
		this->width = width;
		this->height = height;
	}

	void draw() {
		// draw plot
		drawPlot();
		// draw zones
		drawZones();

		drawUserRectangles();
	}

	void drawUserRectangles() {
		for (UserRectangle userRect : userRects) {
			userRect.draw();
		}
	}
	
	void drawRectsOnGray(RectangleType type) {
		for (int i = 0; i < zones.size(); i++) {
			Zone* z = &zones[i];
			if (z->type == type) {
				addUserRectangle(*z->x1, *z->y1, *z->x2, *z->y2, type);
			}
		}
	}

	void drawPlot() {
		// tbd

		// used for panning
		GLfloat pan_x = 0;
		GLfloat pan_y = 0;
		glPushMatrix(); // Makes a new layer
		glTranslatef(centerX + pan_x, centerY + pan_y, 0.0f);
		glScalef((width / 2), (height / 2), 0.0f);
		//glRotatef(45, 1, 1, 1);
		glBegin(GL_LINE_STRIP);
		glColor3ub(0, 0, 0); // Line color
		glVertex2f(-1, -1); //defines Y axis -1 for 90 degrees, change the first element to change angle	
		glVertex2f(-1, 1);
		glVertex2f(1, 1); //defines X axis, change the second element to change angle	
		glEnd();
		glPopMatrix(); // Removes the layer

		/*float labelsDistanceFromPlot = 10;
		float xLabelLocation = labelsDistanceFromPlot + 10;*/
		float lineSeparation = 18;
		float xLabelVerticalOffset = 17;
		float yLabelVerticalOffset = xLabelVerticalOffset + lineSeparation;
		std::string xLabelStr = "x:" + attribute1Name + " ";
		std::string yLabelStr = "y:" + attribute2Name + " ";
		std::string labelsStr = xLabelStr + yLabelStr;
		char* xLabelArr = &xLabelStr[0];
		char* yLabelArr = &yLabelStr[0];
		/*float plotHeight = nonOrthoY2[i] - nonOrthoY1[i];
		float width = nonOrthoX2[i] - nonOrthoX1[i];*/
		float fractionOfWidth = width / 3;
		drawBitmapText(xLabelArr, getX1() + fractionOfWidth, getY2() + xLabelVerticalOffset);
		drawBitmapText(yLabelArr, getX1() + fractionOfWidth, getY2() + yLabelVerticalOffset);


		int precision = 4;

		// draw axes ranges
		std::string attr1 = attribute1Name;
		std::string attr2 = attribute2Name;
		//std::string xLowRange = std::to_string((int)attributeMinMax->at(attr1)[0]);
		std::string xLowRange = std::to_string(attributeMinMax->at(attr1)[0]).substr(0, precision);
		std::string xHighRange = std::to_string(attributeMinMax->at(attr1)[1]).substr(0, precision);
		//std::string yLowRange = std::to_string((int)attributeMinMax->at(attr2)[0]);
		std::string yLowRange = std::to_string(attributeMinMax->at(attr2)[0]).substr(0, precision);
		std::string yHighRange = std::to_string(attributeMinMax->at(attr2)[1]).substr(0, precision);


		// TODO: needs some work
		if (isXInverted) {
			std::string tmp = xLowRange;
			xLowRange = xHighRange;
			xHighRange = tmp;
		}

		if (isYInverted) {
			std::string tmp = yLowRange;
			yLowRange = yHighRange;
			yHighRange = tmp;
		}

		if (attr1 == attr2) {
			std::cout << "debug";
		}

		drawBitmapText(&xLowRange[0], getX1(), getY2() + 1.5 * lineSeparation);
		drawBitmapText(&xHighRange[0], getX2() - xHighRange.length() / 2, getY2() + 1.5 * lineSeparation);
		drawBitmapText(&yLowRange[0], getX1() - ((float)precision / 1.5f) * lineSeparation, getY2());
		drawBitmapText(&yHighRange[0], getX1() - ((float)precision / 1.5f) * lineSeparation, getY1() + yHighRange.length() / 2);

		// debug draw plot num
		std::string plot = "plotId:" + std::to_string(plotNum);
		char* plotAsArr = &plot[0];
		float plotIdOffset = yLabelVerticalOffset + lineSeparation;
		drawBitmapText(plotAsArr, getX1() + fractionOfWidth, getY2() + plotIdOffset);
	}
	
	void drawZones() {
		for (int i = 0; i < zones.size(); i++) {
			zones[i].drawZone();
		}
	}

	void addZone(Zone &zone) {
		zone.parentCenterX = &centerX;
		zone.parentCenterY = &centerY;
		zone.parentWidth = &width;
		zone.parentHeight = &height;
		zone.plotNum = plotNum;
		zone.isSingleAttributePlot = &isSingleAttributePlot;
		// zone.computeRealCoordinates();
		zone.computeSelectionZones();
		zones.push_back(zone);
	}

    void addUserRectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, RectangleType type) {
        UserRectangle userRectangle(x1, y1, x2, y2, type, plotNum);
		userRectangle.parentCenterX = &centerX;
		userRectangle.parentCenterY = &centerY;
		userRectangle.parentWidth = &width;
		userRectangle.parentHeight = &height;
        userRects.push_back(userRectangle);
    }

	/**
	 * @brief Function draws a point on the plot and accounts for any possible user rectangles
	 * 
	 * @param px The incoming point on range [0-1]
	 * @param py The incoming point on range [0-1]
	 * @return int The next plot this point leads to
	 */
	int drawData(std::vector<std::vector<std::string>>& parsedAttributePairs, std::map<std::string, int>& attributeNameToDataIndex,
		std::vector<std::vector<float>>& normalizedValues, std::vector<Plot>& plots, std::vector<float>& classTransparencies,
		std::map<int, std::vector<float>>& classColors, int caseNum, int caseClass) 
	{
		std::vector<std::string> attributePairAndPlotNum = parsedAttributePairs[plotNum];
		std::string attr1 = attributePairAndPlotNum[0];
		std::string attr2 = attributePairAndPlotNum[1];
		int attr1Index = attributeNameToDataIndex[attr1];
		int attr2Index = attributeNameToDataIndex[attr2];
		float x = normalizedValues[caseNum][attr1Index];
		float y = normalizedValues[caseNum][attr2Index];
		int plotToDrawNext = drawData(x, y, caseNum, caseClass);
		if (plotToDrawNext == -1) {
			return -1;
		}
		attributePairAndPlotNum = parsedAttributePairs[plotToDrawNext];
		attr1 = attributePairAndPlotNum[0];
		attr2 = attributePairAndPlotNum[1];
		attr1Index = attributeNameToDataIndex[attr1];
		attr2Index = attributeNameToDataIndex[attr2];
		float x2 = normalizedValues[caseNum][attr1Index];
		float y2 = normalizedValues[caseNum][attr2Index];
		for (int i = 0; i < plots.size(); i++) {
			Plot* plt = &plots[i];
			Zone* zone = plt->getZoneThatContainsPoint(x2, y2);
			if (zone == nullptr) {
				return -1;
			}
			if (zone->type == Decision) {
				glColor4ub(
			}
		}
		drawLine(x, y, x2, y2);
	}
	int drawData(GLfloat px, GLfloat py, int caseNum, int caseClass) {
		if (!isPointWithinPlot(px, py)) return -1;
		Zone* zonePointLandsIn = getZoneThatContainsPoint(px, py);
		if (zonePointLandsIn == NULL) return -1;
		// check userRectangles
		adjustToUserRectangles(px, py);
		// handle classifications
		constructClassification(caseNum, caseClass, zonePointLandsIn);
		GLubyte classTransparency = classTransparencies->at(caseClass);
		if (zonePointLandsIn->type == Decision) {
			GLubyte r = classColor->at(caseClass)[0];
			GLubyte g = classColor->at(caseClass)[1];
			GLubyte b = classColor->at(caseClass)[2];
			GLubyte a = classTransparency;
			glColor4ub(r, g, b, a);
		}
		else {
			glColor4ub(0, 0, 255, 255);
		}
		drawPoint(px, py);
		return zonePointLandsIn->destinationPlot;
	}

	void constructClassification(int caseNum, int caseClass, Zone* z) {
		if (z->type != Decision) return;
		if (classifiedCases.find(caseNum) != classifiedCases.end()) return;
		Classification classification(caseNum, caseClass, z->classNum);
		if (classification.isMisclassification) {
			misclassifiedCases.insert(classification);
		}
		else {
			correctlyClassifiedCases.insert(classification);
		}
		classifiedCases.insert(caseNum);
	}

	void clearClassifications() {
		classifiedCases.clear();
		misclassifiedCases.clear();
		correctlyClassifiedCases.clear();
	}

	void adjustToUserRectangles(GLfloat px, GLfloat py) {
		std::vector<UserRectangle*> userRectanglesThatPointLandsIn = getUserRectanglesThatPointLandsIn(px, py);
		for (int i = 0; i < userRectanglesThatPointLandsIn.size(); i++) {
			UserRectangle* userRect = userRectanglesThatPointLandsIn[i];
			userRect->adjustPoint(&px, &py);
		}
	}

	void drawPoint(GLfloat px, GLfloat py) {
		glBegin(GL_POINT);
		glVertex2f(px, py);
		glEnd();
	}

	void drawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
		glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glEnd();
	}

	std::vector<UserRectangle*> getUserRectanglesThatPointLandsIn(GLfloat px, GLfloat py) {
		std::vector<UserRectangle*> userRectanglesThatPointLandsIn;
		for (int i = 0; i < userRects.size(); i++) {
			if (userRects[i].isPointWithinRectangle(px, py)) {
				userRectanglesThatPointLandsIn.push_back(&userRects[i]);
			}
		}
		return userRectanglesThatPointLandsIn;
	}

	Zone* getZoneThatContainsPoint(GLfloat px, GLfloat py) {
		Zone* z = nullptr;
		if (!isPointWithinPlot(px, py)) {
			isPointWithinPlot(px, py); // debug
			return z;
		}
		for (int i = 0; i < zones.size(); i++) {
			if (zones[i].isPointWithinZone(px, py)) {
				z = &zones[i];
				break;
			}
		}
		return z;
	}

	bool isPointWithinPlot(GLfloat px, GLfloat py) {
		GLfloat x1 = getX1();
		GLfloat x2 = getX2();
		GLfloat y1 = getY1();
		GLfloat y2 = getY2();
		float eps = 0.001;
		if (px != x1 && abs(px - x1) < eps) {
			px = x1;
		} else if (px != x2 && abs(px - x2) < eps) {
			px = x2;
		}

		if (py != y1 && abs(py - y1) < eps) {
			py = y1;
		} else if (py != y2 && abs(py - y2) < eps) {
			py = y2;
		}
		

		if (px >= x1 && px <= x2 && py >= y1 && py <= y2) {
			return true;
		}

		return false;
	}

	GLfloat getX1() {
		return centerX - width / 2;
	}
	GLfloat getX2() {
		return centerX + width / 2;
	}
	GLfloat getY1() {
		return centerY - height / 2;
	}
	GLfloat getY2() {
		return centerY + height / 2;
	}

    void swapAxes() {
        isXYSwapped = !isXYSwapped;
        for (int i = 0; i < zones.size(); i++) {
            zones[i].swapAxes();
        }
    }

    void invertAxis(bool isXAxis) {
        if (isXAxis) {
            isXInverted = !isXInverted;
        } else {
            isYInverted = !isYInverted;
        }

        for (int i = 0; i < zones.size(); i++) {
            zones[i].invertAxis(isXAxis);
        }
    }
};