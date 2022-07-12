#include "stdafx.h"
#include "Zone.h"
#include "DrawingUtils.h"
#pragma once

struct Plot {
	int plotNum;
	GLfloat centerX, centerY, width, height;
	std::vector<Zone> zones;
	std::string attribute1Name, attribute2Name;
	std::map<std::string, std::vector<float>>* attributeMinMax;
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

	Zone* getZoneThatContainsPoint(GLfloat px, GLfloat py) {
		Zone* z = nullptr;
		for (int i = 0; i < zones.size(); i++) {
			if (zones[i].isPointWithinZone(px, py)) {
				z = &zones[i];
				break;
			}
		}
		return z;
	}

	bool isPointWithinPlot(GLfloat px, GLfloat py) {
		if (px >= getX1() && px <= getX2() && py >= getY1() && py <= getY2()) {
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