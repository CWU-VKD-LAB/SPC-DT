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
    //ClassData *data;
    std::vector<std::vector<float>>* parsedData;
    std::map<int, std::map<int, std::set<int>>>* plotNumZoneTotalCases;
    int* maxCasesPerPlotZone;
    GLfloat* parentCenterX;
	GLfloat* parentCenterY;
    GLfloat* parentWidth;
	GLfloat* parentHeight;
    bool* isSingleAttributePlot;
    float edgeSelectionZones[4][2][2];
    float selectionZoneWidth;
    bool hasDarkBackground = false;
    GLfloat* backgroundClassColorCoeff;
    bool* isBackgroundDensityColoringMode;
    //Edge* edges[4]; // left, top, right, bottom
    
    Zone() {}
    Zone(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, int i,
        int destinationPlot, int classNum, ZoneType type, float selectionZoneWidth,
        std::vector<float>* color, std::vector<std::vector<float>>* parsedData, int* maxCasesPerPlotZone,
        std::map<int, std::map<int, std::set<int>>>* plotNumZoneTotalCases,
        bool* isBackgroundDensityColoringMode,
        GLfloat* backgroundClassColorCoeff) 
    {
		this->x1 = x1;
		this->x2 = x2;
	    this->y1 = y1;
		this->y2 = y2;
		this->id = i;
		this->destinationPlot = destinationPlot;
		this->classNum = classNum;
		this->type = type;
		this->selectionZoneWidth = selectionZoneWidth;
		this->color = *color;
		this->parsedData = parsedData;
		this->maxCasesPerPlotZone = maxCasesPerPlotZone;
		this->plotNumZoneTotalCases = plotNumZoneTotalCases;
		this->isBackgroundDensityColoringMode = isBackgroundDensityColoringMode;
		this->backgroundClassColorCoeff = backgroundClassColorCoeff;
    }
	
    //Zone(GLfloat& x1, GLfloat& y1, GLfloat& x2, GLfloat& y2, int id, int destinationPlot, int classNum, ZoneType type, float selectionZoneWidth, std::vector<float>* color, std::vector<std::vector<float>> &parsedData) {
    //    this->id = id;
    //    this->destinationPlot = destinationPlot;
    //    this->classNum = classNum;
    //    this->x1 = x1;
    //    this->x2 = x2;
    //    this->y1 = y1;
    //    this->y2 = y2;
    //    this->selectionZoneWidth = selectionZoneWidth;
    //    this->type = type;
    //    this->color = *color;
    //    //this->data = data;
    //    this->parsedData = &parsedData;
    //    computeRealCoordinates();
    //}
    void invertX() {
        x1 = 1.0f - x1;
        x2 = 1.0f - x2;
        computeRealCoordinates();
        computeSelectionZones();
    }
    void invertY() {
        y1 = 1.0f - y1;
        y2 = 1.0f - y2;
        computeRealCoordinates();
        computeSelectionZones();
    }
    void swapXY() {
        swap(x1, y1);
        swap(x2, y2);
        computeRealCoordinates();
        computeSelectionZones();
    }
    void updateFromParser() {
        x1 = parsedData->at(id)[0];
		y1 = parsedData->at(id)[1];
		x2 = parsedData->at(id)[2];
		y2 = parsedData->at(id)[3];
        computeRealCoordinates();
        computeSelectionZones();
    }
    void computeRealCoordinates() {
        realX1 = (*parentCenterX - (*parentWidth / 2)) + x1 * (*parentWidth);
		realX2 = (*parentCenterX - (*parentWidth / 2)) + x2 * (*parentWidth);
		realY1 = (*parentCenterY - (*parentHeight / 2)) + y1 * (*parentHeight);
		realY2 = (*parentCenterY - (*parentHeight / 2)) + y2 * (*parentHeight);
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
    void drawZone() {
        updateFromParser();
        computeRealCoordinates();
        GLfloat backgroundTransparency = computeBackgroundTransparency();
        setColor(*backgroundClassColorCoeff, backgroundTransparency);
        glRectf(realX1, realY1, realX2, realY2);
    }
    void computeSelectionZones(float selectionWidth) {
        computeRealCoordinates();
        // left edge
        edgeSelectionZones[0][0][0] = realX1 - selectionWidth * 0.5;
        edgeSelectionZones[0][0][1] = realY1;
        edgeSelectionZones[0][1][0] = realX1 + selectionWidth * 0.5;
        edgeSelectionZones[0][1][1] = realY2;
        // bottom edge
        edgeSelectionZones[1][0][0] = realX1;
        edgeSelectionZones[1][0][1] = realY2 + selectionWidth * 0.5;
        edgeSelectionZones[1][1][0] = realX2;
        edgeSelectionZones[1][1][1] = realY2 - selectionWidth * 0.5;
        // right edge
        edgeSelectionZones[2][0][0] = realX2 - selectionWidth * 0.5;
        edgeSelectionZones[2][0][1] = realY1;
        edgeSelectionZones[2][1][0] = realX2 + selectionWidth * 0.5;
        edgeSelectionZones[2][1][1] = realY2;
        // top edge
        edgeSelectionZones[3][0][0] = realX1;
        edgeSelectionZones[3][0][1] = realY1 + selectionWidth * 0.5;
        edgeSelectionZones[3][1][0] = realX2;
        edgeSelectionZones[3][1][1] = realY1 - selectionWidth * 0.5;
    }
    void computeSelectionZones() {
        computeSelectionZones(selectionZoneWidth);
    }

    std::vector<int> findEdgeForPoint(float &x, float &y) {
        // returns left, bottom, right, top, none (0, 1, 2, 3, -1)
        std::vector<int> returnVect;
        // edge
        for (int i = 0; i < 4; i++) {
            float highX = max(edgeSelectionZones[i][0][0], edgeSelectionZones[i][1][0]);
            float lowX = min(edgeSelectionZones[i][0][0], edgeSelectionZones[i][1][0]);
            float highY = max(edgeSelectionZones[i][0][1], edgeSelectionZones[i][1][1]);
            float lowY = min(edgeSelectionZones[i][0][1], edgeSelectionZones[i][1][1]);
            if (isPointWithinRect(x, y, lowX, lowY, highX, highY)) {
                returnVect.push_back(id);
                returnVect.push_back(i);
                break;
            }
       }
        return returnVect;
    }
    void drawEdges() {
        //drawSelectionZones(); // debug
        glColor4ub(0, 0, 0, 255);
        glBegin(GL_LINE_LOOP);
        glVertex2f(realX1, realY1);
        glVertex2f(realX2, realY1);
        glVertex2f(realX2, realY2);
        glVertex2f(realX1, realY2);
        glEnd();
    }
    void drawSelectionZones() {
        for (int i = 0; i < 4; i++) {
            glBegin(GL_POINTS);
            glColor3ub(255, 0, 0);
            glVertex2f(edgeSelectionZones[i][0][0], edgeSelectionZones[i][0][1]);
            glColor3ub(0, 255, 0);
            glVertex2f(edgeSelectionZones[i][1][0], edgeSelectionZones[i][1][1]);
            glEnd();
            glBegin(GL_LINE);
            glVertex2f(edgeSelectionZones[i][0][0], edgeSelectionZones[i][0][1]);
            glVertex2f(edgeSelectionZones[i][1][0], edgeSelectionZones[i][1][1]);
            glEnd();

            glBegin(GL_POLYGON);
            // draw a rectangle
            glVertex2f(edgeSelectionZones[i][0][0], edgeSelectionZones[i][0][1]);
            glVertex2f(edgeSelectionZones[i][1][0], edgeSelectionZones[i][0][1]);
            glVertex2f(edgeSelectionZones[i][0][0], edgeSelectionZones[i][0][1]);
            glVertex2f(edgeSelectionZones[i][0][0], edgeSelectionZones[i][1][1]);
            glEnd();
        }
    }
    bool isPointWithinZone(float& px, float& py) {
        computeRealCoordinates();
        float highX = max(realX1, realX2);
        float lowX = min(realX1, realX2);
        float highY = max(realY1, realY2);
        float lowY = min(realY1, realY2);
        return isPointWithinRect(px, py, lowX, lowY, highX, highY);
    }

    GLfloat computeBackgroundTransparency() {
        GLubyte backgroundTransparency = 100;
        const GLubyte maxVal = 245;
        // check if plot has similar attributes
        int plot = plotNum;
        int zoneId = id;
        int zoneClass = classNum;

        if (*isBackgroundDensityColoringMode && zoneId >= 0 && zoneClass >= 0)
        {
            if (*isSingleAttributePlot)
            {
                int singleAttributePlotSum = (float)plotNumZoneTotalCases->at(plot)[zoneId].size();
                float zoneDensity = ((float)singleAttributePlotSum / (float)(*maxCasesPerPlotZone));
                backgroundTransparency = min(((float)singleAttributePlotSum / (float)(*maxCasesPerPlotZone)) * maxVal + backgroundTransparency, maxVal);
                // we need to sum up all the connected, similarly colored zones
                std::vector<std::vector<float>*> parserElementsWithPlotNum;
                for (int i = 0; i < parsedData->size(); i++) {
                    if (parsedData->at(i)[4] == plot) {
                        parserElementsWithPlotNum.push_back(&parsedData->at(i));
                    }
                }
                //std::set<int> visitedZones;
                //std::vector<ConnectedZone> connectedZones;
                //for (int i = 0; i < parserElementsWithPlotNum.size(); i++) {
                //    connectedZones.push_back(ConnectedZone(i, parserElementsWithPlotNum[i]->at(4), parserElementsWithPlotNum[i]));
                //}
                //for (int i = 0; i < parserElementsWithPlotNum.size(); i++) {
                //    visitedZones.insert(i);
                //    ConnectedZone* currentZoneNode = &connectedZones[i];
                //    for (int j = 0; j < parserElementsWithPlotNum.size(); j++) {
                //        if (i == j) continue;
                //        if (parserElementsWithPlotNum[i][4] != parserElementsWithPlotNum[j][4]) continue;
                //        if ((parserElementsWithPlotNum[i][0] == parserElementsWithPlotNum[j][0] || parserElementsWithPlotNum[i][0] == parserElementsWithPlotNum[j][2]) &&
                //            (parserElementsWithPlotNum[i][1] == parserElementsWithPlotNum[j][1] || parserElementsWithPlotNum[i][1] == parserElementsWithPlotNum[j][3]) &&
                //            (visitedZones.find(j) == visitedZones.end())) {
                //            currentZoneNode->connectedZones.push_back(&connectedZones[j]);
                //        }
                //    }
                //}
            }
            else if (zoneClass >= 0 && *maxCasesPerPlotZone != 0)
            {
                float zoneDensity = ((float)plotNumZoneTotalCases[plot][zoneId].size() / (float)(*maxCasesPerPlotZone));
                backgroundTransparency = min(zoneDensity * maxVal + backgroundTransparency, maxVal);
                // debug
                computeRealCoordinates();
                // debug: draws background value on top of background zone
                //float px = zone.realX1 + (zone.realX2 - zone.realX1) * 0.5;
                //float py = zone.realY1 + (zone.realY2 - zone.realY1) * 0.5;
                //data.drawBitmapText(std::to_string(zoneDensity).c_str(), px, py);
                //if (zoneDensity != 0 && zoneDensity < worstZoneNumDensity)
                //{
                //    worstZoneNumDensity = zoneDensity;
                //    worstZoneNum = zoneId;
                //}
                // if (backgroundLightnessSet.find(backgroundTransparencyCopy) == backgroundLightnessSet.end())
                // {
                //     backgroundLightnessSet.insert(backgroundTransparencyCopy);
                // }
            }
        }

        hasDarkBackground = backgroundTransparency >= 128;
        
        return backgroundTransparency;
    }

	

private:
    void swap(float &x, float &y) {
        float tmp = x;
        x = y;
        y = tmp;
    }
    bool isPointWithinRect(float& px, float& py, float& rectX1, float& rectY1, float& rectX2, float& rectY2) {
        if (abs(py - rectY2 < 1) && abs(py - rectY2 > 0)) {
            std::cout << "debug";
        }
        if (px >= rectX1 && px <= rectX2 && py <= rectY2 && py >= rectY1) {
            return true;
        }
        return false;
    }
};