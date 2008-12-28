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

#include "draw_primitive.h"
#include "SDL_opengl.h"
#include <cmath>
using namespace Halley;


#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795f
#endif


void PrimitiveDrawer::DrawSquare(float side) const
{
	DrawRectangle(Vector2f(side, side));
}

void PrimitiveDrawer::DrawRectangle(Vector2f size) const
{
	float w = size.x * 0.5f;
	float h = size.y * 0.5f;
	glBegin(GL_QUADS);
	glVertex2f(-w,-h);
	glVertex2f(-w,h);
	glVertex2f(w,h);
	glVertex2f(w,-h);
	glEnd();
}

void PrimitiveDrawer::DrawStar(int tips, float rad1, float rad2) const
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
