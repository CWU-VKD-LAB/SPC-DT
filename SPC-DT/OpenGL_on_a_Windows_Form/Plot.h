#include "stdafx.h"
#include "Zone.h"
#pragma once

struct Plot {
	GLfloat centerX, centerY, width, height;
	std::vector<Zone> zones;
	
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
	}
	
	void drawZones() {
		for (int i = 0; i < zones.size(); i++) {
			zones[i].drawZone(0, 0);
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
};