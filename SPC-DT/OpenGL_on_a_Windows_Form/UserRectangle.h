#include "stdafx.h"
#include "glut.h"
#pragma once

enum RectangleType {Condense, Exclude, Expand, None};
// structure representing a user rectangle
struct UserRectangle {
	int id;
	int plotNum;
	GLfloat X1, X2, Y1, Y2;
    GLfloat* parentCenterX;
    GLfloat* parentCenterY;
    GLfloat* parentWidth;
    GLfloat* parentHeight;
	GLubyte frameColor[3];
	RectangleType type;
	// ClassData* data;
	UserRectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, RectangleType type, int plotNum) {
		//X1 = (x1 - data->x1CoordPlot[plotNum]) / (data->x2CoordPlot[plotNum] - data->x1CoordPlot[plotNum]);
  //      X2 = (x2 - data->x1CoordPlot[plotNum]) / (data->x2CoordPlot[plotNum] - data->x1CoordPlot[plotNum]);
  //      Y1 = (y1 - data->y1CoordPlot[plotNum]) / (data->y2CoordPlot[plotNum] - data->y1CoordPlot[plotNum]);
  //      Y2 = (y2 - data->y1CoordPlot[plotNum]) / (data->y2CoordPlot[plotNum] - data->y1CoordPlot[plotNum]);
        X1 = (x1 - getParentRealX1()) / (*parentWidth);
		X2 = (x2 - getParentRealX1()) / (*parentWidth);
		Y1 = (y1 - getParentRealY1()) / (*parentHeight);
		Y2 = (y2 - getParentRealY1()) / (*parentHeight);
		id = time(NULL);
		this->plotNum = plotNum;
		this->type = type;
		//this->data = data;
        //computeRealCoords();
	}
	void swapXY() {
		GLfloat temp = X1;
		X1 = Y1;
		Y1 = temp;
		temp = X2;
		X2 = Y2;
		Y2 = temp;
	}
    void invertX() {
        X1 = 1.0f - X1;
        X2 = 1.0f - X2;
        //computeRealCoords();
    }
    void invertY() {
        Y1 = 1.0f - Y1;
        Y2 = 1.0f - Y2;
        //computeRealCoords();
    }
	void draw() {
		glColor3ubv(frameColor);
		glBegin(GL_LINE_LOOP);
		glVertex2f(getRealX1(), getRealY1());
		glVertex2f(getRealX2(), getRealY1());
		glVertex2f(getRealX2(), getRealY2());
		glVertex2f(getRealX1(), getRealY2());
		glEnd();
	}
  //  void computeRealCoords() {
  //      //getRealX1() = data->x1CoordPlot[plotNum] + (X1 * (data->x2CoordPlot[plotNum] - data->x1CoordPlot[plotNum]));
  //      //getRealX2() = data->x1CoordPlot[plotNum] + (X2 * (data->x2CoordPlot[plotNum] - data->x1CoordPlot[plotNum]));
  //      //getRealY1() = data->y1CoordPlot[plotNum] + (Y1 * (data->y2CoordPlot[plotNum] - data->y1CoordPlot[plotNum]));
  //      //getRealY2() = data->y1CoordPlot[plotNum] + (Y2 * (data->y2CoordPlot[plotNum] - data->y1CoordPlot[plotNum]));
		//getRealX1() = getParentRealX1() + (X1 * (*parentWidth));
		//getRealX2() = getParentRealX1() + (X2 * (*parentWidth));
		//getRealY1() = getParentRealY1() + (Y1 * parentPlot->height);
		//getRealY2() = getParentRealY1() + (Y2 * parentPlot->height);
  //  }
	void drawEdges() {
		glColor4ub(0, 0, 0, 255);
		glBegin(GL_LINE_LOOP);
        glVertex2f(getRealX1(), getRealY1());
        glVertex2f(getRealX2(), getRealY1());
        glVertex2f(getRealX2(), getRealY2());
        glVertex2f(getRealX1(), getRealY2());
		glEnd();
	}
    void drawEdges(float lineThickness) {
        glColor3ub(frameColor[0], frameColor[1], frameColor[2]);
        glLineWidth(lineThickness);
        drawEdges();
        glLineWidth(1.0); // TODO: replace with global line width
    }
    bool isPointWithinRect(float&px, float&py) {
        if (px >= getRealX1() && px <= getRealX2() && py >= getRealY2() && py <= getRealY1()) {
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
    GLfloat getRealX1() {
		return (*parentCenterX) - (*parentWidth) / 2.0f + (X1 * (*parentWidth));
    }
	GLfloat getRealX2() {
		return (*parentCenterX) - (*parentWidth) / 2.0f + (X2 * (*parentWidth));
	}
	GLfloat getRealY1() {
		return (*parentCenterY) - (*parentHeight) / 2.0f + (Y1 * (*parentHeight));
	}
	GLfloat getRealY2() {
		return (*parentCenterY) - (*parentHeight) / 2.0f + (Y2 * (*parentHeight));
	}
    GLfloat getParentRealX1() {
		return (*parentCenterX) - (*parentWidth) / 2.0f;
    }
	GLfloat getParentRealX2() {
		return (*parentCenterX) + (*parentWidth) / 2.0f;
	}
	GLfloat getParentRealY1() {
		return (*parentCenterY) - (*parentHeight) / 2.0f;
	}
	GLfloat getParentRealY2() {
		return (*parentCenterY) + (*parentHeight) / 2.0f;
	}
	GLfloat getCenterX() {
		return getRealX1() + (getRealX2() - getRealX1()) / 2.0f;
	}
	GLfloat getCenterY() {
		return getRealY1() + (getRealY2() - getRealY1()) / 2.0f;
	}
	

	bool isPointWithinRectangle(GLfloat px, GLfloat py) {
		if (px >= getRealX1() && px <= getRealX2() && py >= getRealY2() && py <= getRealY1()) {
			return true;
		}
		return false;
	}

	void adjustPoint(GLfloat *px, GLfloat *py) {
		switch (type) {
		case Condense:
			*px = getCenterX();
			*py = getCenterY();
			break;
		// todo: more to come...
		}
	}
};
