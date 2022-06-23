#include "stdafx.h"
#include "glut.h"
#include "ColorUtils.h"
#include <vector>
#pragma once

enum ZoneType {Decision, Continue};
struct Zone {
	int id;
	int plotNum;
	int destinationPlot;
	int classNum;
    GLfloat x1, x2, y1, y2 = -1; // from parser (fractions of the overall plot)
    GLfloat realX1, realX2, realY1, realY2 = -1; // real: meaning actual coordinates
	ZoneType type;
    std::vector<float> color;
    ClassData *data;
    float edgeSelectionZones[4][2][2];
    Zone(GLfloat& x1, GLfloat& y1, GLfloat& x2, GLfloat& y2, int id, int plotNum, int destinationPlot, int classNum, ZoneType type, std::vector<float>* color, ClassData *data) {
        this->id = id;
        this->plotNum = plotNum;
        this->destinationPlot = destinationPlot;
        this->classNum = classNum;
        this->x1 = x1;
        this->x2 = x2;
        this->y1 = y1;
        this->y2 = y2;
        this->type = type;
        this->color = *color;
        this->data = data;
        computeRealCoordinates();
    }
    void invertX() {
        x1 = 1.0f - x1;
        x2 = 1.0f - x2;
    }
    void invertY() {
        y1 = 1.0f - y1;
        y2 = 1.0f - y2;
    }
    void computeRealCoordinates() {
        realX1 = data->x1CoordPlot[plotNum] + x1 * data->plotWidth[plotNum];
        realX2 = data->x1CoordPlot[plotNum] + x2 * data->plotWidth[plotNum];
        realY2 = data->y2CoordPlot[plotNum] - y1 * data->plotHeight[plotNum];
        realY1 = data->y2CoordPlot[plotNum] - y2 * data->plotHeight[plotNum];
    }
    void setColor(float backgroundClassColorCoefficient, GLubyte transparency) {
        // TODO: handle multiple decision classes
        // adjust color based on class
        std::vector<float> hsl = RGBtoHSL(color);
        // we need to adjust the lightness of the background color
        hsl[2] *= backgroundClassColorCoefficient;
        std::vector<GLubyte> rgb = HSLtoRGB(hsl);
        glColor4ub(rgb[0], rgb[1], rgb[2], transparency);
    }
    void drawZone(float backgroundClassColorCoeffient, GLubyte transparency) {
        computeRealCoordinates();
        setColor(backgroundClassColorCoeffient, transparency);
        glRectf(realX1, realY1, realX2, realY2);
    }
    void computeSelectionZones(float selectionWidth) {
        computeRealCoordinates();
        // left edge
        edgeSelectionZones[0][0][0] = realX1 - selectionWidth * 0.5;
        edgeSelectionZones[0][0][1] = realY1;
        edgeSelectionZones[0][1][0] = realX1 + selectionWidth * 0.5;
        edgeSelectionZones[0][1][1] = realY2;
        // right edge
        edgeSelectionZones[1][0][0] = realX2 - selectionWidth * 0.5;
        edgeSelectionZones[1][0][1] = realY1;
        edgeSelectionZones[1][1][0] = realX2 + selectionWidth * 0.5;
        edgeSelectionZones[1][1][1] = realY2;
        // top edge
        edgeSelectionZones[2][0][0] = realX1;
        edgeSelectionZones[2][0][1] = realY1 + selectionWidth * 0.5;
        edgeSelectionZones[2][1][0] = realX2;
        edgeSelectionZones[2][1][1] = realY1 - selectionWidth * 0.5;
        // bottom edge
        edgeSelectionZones[3][0][0] = realX1;
        edgeSelectionZones[3][0][1] = realY2 + selectionWidth * 0.5;
        edgeSelectionZones[3][1][0] = realX2;
        edgeSelectionZones[3][1][1] = realY2 - selectionWidth * 0.5;
    }
    void drawEdges() {
        glColor4ub(0, 0, 0, 255);
        glBegin(GL_LINE_LOOP);
        glVertex2f(realX1, realY1);
        glVertex2f(realX2, realY1);
        glVertex2f(realX2, realY2);
        glVertex2f(realX1, realY2);
        glEnd();
    }
};