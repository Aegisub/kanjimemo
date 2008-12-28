/*********************************************************\
         __      __           __  __
        / /     / / _____    / / / / ______   __     __
       / /     / / /___  \  / / / / / ___  | / /    / /
      / /_____/ / _____| / / / / / / /__/ / / /    / /
     / ______  / / ___  / / / / / / _____/ / /    / /
    / /     / / / /__/ / / / / / / |____  / |____/ /
   /_/     /_/ |______/ /_/ /_/  \_____/  \_____  /
                                         ______/ /
            High Level Game Framework   /_______/

  ------------------------------------------------------

  Copyright (c) 2008 - Rodrigo Braz Monteiro.
  This file is subject to the terms of license.txt.

\*********************************************************/

#include "gl_primitive.h"
#include "SDL_opengl.h"
#include <cmath>
using namespace Halley;


#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795f
#endif


void GLPrimitive::DrawSquare(float side)
{
	DrawRectangle(side, side);
}

void GLPrimitive::DrawRectangle(float width, float height)
{
	float w = width * 0.5f;
	float h = height * 0.5f;
	glBegin(GL_QUADS);
	glVertex2f(-w,-h);
	glVertex2f(-w,h);
	glVertex2f(w,h);
	glVertex2f(w,-h);
	glEnd();
}

void GLPrimitive::DrawStar(int tips, float rad1, float rad2)
{
	int n = tips*2;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0,0);
	for (int i=0; i <= n; i++) {
		float r = (i%2 == 0)? rad1 : rad2;
		float ang = (float)i / (float)n * 2.0f * (float)M_PI;
		glVertex2f(cos(ang)*r,sin(ang)*r);
	}
	glEnd();
}
