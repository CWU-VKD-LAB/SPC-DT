#include "stdafx.h"
#include "glut.h"
#pragma once

enum RectangleType {Condense, Exclude, Expand, None};
// structure representing a user rectangle
struct UserRectangle {
	int id;
	int plotNum;
	GLfloat X1, X2, Y1, Y2;
    GLfloat realX1, realX2, realY1, realY2;
	GLubyte frameColor[3];
	RectangleType type;
	ClassData* data;
	UserRectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, RectangleType type, int plotNum, ClassData* data) {
		//X1 = (x1 - data->x1CoordPlot[plotNum]) / (data->x2CoordPlot[plotNum] - data->x1CoordPlot[plotNum]);
  //      X2 = (x2 - data->x1CoordPlot[plotNum]) / (data->x2CoordPlot[plotNum] - data->x1CoordPlot[plotNum]);
  //      Y1 = (y1 - data->y1CoordPlot[plotNum]) / (data->y2CoordPlot[plotNum] - data->y1CoordPlot[plotNum]);
  //      Y2 = (y2 - data->y1CoordPlot[plotNum]) / (data->y2CoordPlot[plotNum] - data->y1CoordPlot[plotNum]);
        X1 = (x1 - data->plots[plotNum].getX1()) / (data->plots[plotNum].width);
		X2 = (x2 - data->plots[plotNum].getX1()) / (data->plots[plotNum].width);
		Y1 = (y1 - data->plots[plotNum].getY1()) / (data->plots[plotNum].height);
		Y2 = (y2 - data->plots[plotNum].getY1()) / (data->plots[plotNum].height);
		id = time(NULL);
		this->plotNum = plotNum;
		this->type = type;
		this->data = data;
        computeRealCoords();
	}
    void invertX() {
        X1 = 1.0f - X1;
        X2 = 1.0f - X2;
        computeRealCoords();
    }
    void invertY() {
        Y1 = 1.0f - Y1;
        Y2 = 1.0f - Y2;
        computeRealCoords();
    }
    void computeRealCoords() {
        //realX1 = data->x1CoordPlot[plotNum] + (X1 * (data->x2CoordPlot[plotNum] - data->x1CoordPlot[plotNum]));
        //realX2 = data->x1CoordPlot[plotNum] + (X2 * (data->x2CoordPlot[plotNum] - data->x1CoordPlot[plotNum]));
        //realY1 = data->y1CoordPlot[plotNum] + (Y1 * (data->y2CoordPlot[plotNum] - data->y1CoordPlot[plotNum]));
        //realY2 = data->y1CoordPlot[plotNum] + (Y2 * (data->y2CoordPlot[plotNum] - data->y1CoordPlot[plotNum]));
		realX1 = data->plots[plotNum].getX1() + (X1 * data->plots[plotNum].width);
		realX2 = data->plots[plotNum].getX1() + (X2 * data->plots[plotNum].width);
		realY1 = data->plots[plotNum].getY1() + (Y1 * data->plots[plotNum].height);
		realY2 = data->plots[plotNum].getY1() + (Y2 * data->plots[plotNum].height);
    }
	void drawEdges() {
        computeRealCoords();
		glColor4ub(0, 0, 0, 255);
		glBegin(GL_LINE_LOOP);
        glVertex2f(realX1, realY1);
        glVertex2f(realX2, realY1);
        glVertex2f(realX2, realY2);
        glVertex2f(realX1, realY2);
		glEnd();
	}
    void drawEdges(float lineThickness) {
        glColor3ub(frameColor[0], frameColor[1], frameColor[2]);
        glLineWidth(lineThickness);
        drawEdges();
        glLineWidth(1.0); // TODO: replace with global line width
    }
    bool isPointWithinRect(float&px, float&py) {
        computeRealCoords();
        if (px >= realX1 && px <= realX2 && py >= realY2 && py <= realY1) {
            return true;
        }
        return false;
    }
    void setFrameColor(GLubyte R, GLubyte G, GLubyte B) {
        frameColor[0] = R;
        frameColor[1] = G;
        frameColor[2] = B;
    }
    bool operator==(const UserRectangle& rect) const {
        return rect.id == id;
    }
};
