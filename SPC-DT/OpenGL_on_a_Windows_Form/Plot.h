#include "stdafx.h"
#include "DrawingUtils.h"
#include "Zone.h"
#pragma once

struct Plot {
	int plotNum;
	std::vector<Zone> zones;
	GLfloat centerX, centerY, width, height = -1;
	std::string attribute1Name, attribute2Name = "";
	std::map<std::string, std::vector<float>> attributeMinMax;
	bool isXInverted, isYInverted, isXYSwapped = false;
	Plot() {};

	Plot(int plotNum, std::map<std::string, std::vector<float>> &attributeMinMax) {
		this->plotNum = plotNum;
		this->attributeMinMax = attributeMinMax;
	}

	Plot(GLfloat centerX, GLfloat centerY, GLfloat width, GLfloat height, std::string attribute1Name, std::string attribute2Name) {
		this->centerX = centerX;
		this->centerY = centerY;
		this->width = width;
		this->height = height;
		this->attribute1Name = attribute1Name;
		this->attribute2Name = attribute2Name;
	}

	void draw() {
		// draw plot
		
		// draw zones
		for (int i = 0; i < zones.size(); i++) {
			//zones[i].drawZone();
		}
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
	bool isPointWithinPlot(GLfloat px, GLfloat py) {
		return px >= getX1() && px <= getX2() && py >= getY1() && py <= getY2();
	}
	
	Zone* getZoneFromPoint(GLfloat px, GLfloat py) {
		for (int i = 0; i < zones.size(); i++) {
			if (zones[i].isPointWithinZone(px, py)) {
				return &zones[i];
			}
		}
	}
	
	void drawPlot() {
		//glPushMatrix(); // Makes a new layer
		//glTranslatef(x + pan_x, y + pan_y, 0.0f);
		//glScalef((plotW / 2), (plotH / 2), 0.0f);
		////glRotatef(45, 1, 1, 1);
		//glBegin(GL_LINE_STRIP);
		//glColor3ub(0, 0, 0); // Line color
		//glVertex2f(nonOrthoX1[plotNum], nonOrthoY1[plotNum]); //defines Y axis -1 for 90 degrees, change the first element to change angle	
		//glVertex2f(-1, 1);
		//glVertex2f(nonOrthoX2[plotNum], nonOrthoY2[plotNum]); //defines X axis, change the second element to change angle	
		//glEnd();
		//glPopMatrix(); // Removes the layer

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
		float plotWidth = nonOrthoX2[i] - nonOrthoX1[i];*/
		float fractionOfWidth = width / 3;
		drawBitmapText(xLabelArr, getX1() + fractionOfWidth, getY2() + xLabelVerticalOffset);
		drawBitmapText(yLabelArr, getX1() + fractionOfWidth, getY2() + yLabelVerticalOffset);


		int precision = 4;

		// draw axes ranges
		std::string attr1 = attribute1Name;
		std::string attr2 = attribute2Name;
		//std::string xLowRange = std::to_string((int)attributeMinMax[attr1][0]);
		std::string xLowRange = std::to_string(attributeMinMax[attr1][0]).substr(0, precision);
		std::string xHighRange = std::to_string(attributeMinMax[attr1][1]).substr(0, precision);
		//std::string yLowRange = std::to_string((int)attributeMinMax[attr2][0]);
		std::string yLowRange = std::to_string(attributeMinMax[attr2][0]).substr(0, precision);
		std::string yHighRange = std::to_string(attributeMinMax[attr2][1]).substr(0, precision);


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
};