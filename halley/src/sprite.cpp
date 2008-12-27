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

#include "sprite.h"
#include "SDL_opengl.h"
using namespace Halley;


////////////////////
// Draw this sprite
void Sprite::Draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glScalef(scale.x,scale.y,1);
	glTranslatef(pos.x,pos.y,0);
	glRotatef(rotation.GetRadians(),0,0,1);
	glTranslatef(-pivot.x,-pivot.y,0);
	
	DoDraw();

	glPopMatrix();
}
