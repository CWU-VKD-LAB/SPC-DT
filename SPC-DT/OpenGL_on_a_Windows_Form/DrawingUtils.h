#include "stdafx.h"
#pragma once


static void drawBitmapText(const char* string, float x, float y)
{
	const char* c;
	glRasterPos2f(x, y);

	int i = 0;
	for (c = string; *c != ' '; c++)
	{
		if (i < strlen(string))
		{
			//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		}
		else
		{
			break;
		}
		i++;
	}
}