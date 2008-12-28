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

  Copyright (c) 2007 - Rodrigo Braz Monteiro.
  This file is subject to the terms of license.txt.

\*********************************************************/

#include "SDL.h"
#include "SDL_opengl.h"
#include "video.h"
using namespace Halley;


///////////////
// Constructor
Video::Video()
{
	fullscreen = false;
}


/////////////
// Set video
void Video::SetVideo(bool _fullscreen, Vector2i windowSize, Vector2f virtualSize)
{
	// Set flags
	int flags = SDL_OPENGL;
	if (_fullscreen) flags |= SDL_FULLSCREEN;

	// Set GL attributes
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Set video mode
	SDL_Surface *result = SDL_SetVideoMode(windowSize.x,windowSize.y,32,flags);
	if (result == NULL) {
		throw std::exception(SDL_GetError());
	}

	// Calculate virtualSize
	Vector2f p1; // Bottom-left
	Vector2f p2; // Top-right
	if (virtualSize.x == 0 || virtualSize.y == 0) {
		virtualSize = Vector2f(windowSize);
		p2 = virtualSize;
	} else {
		float wAR = (float) windowSize.x / (float) windowSize.y;
		float vAR = virtualSize.x / virtualSize.y;
		p2 = virtualSize;
		if (wAR > vAR) {
			// Letterbox on left/right
			float border = (virtualSize.y * wAR - virtualSize.x) * 0.5f;
			p1.x -= border;
			p2.x += border;
		} else {
			// Letterbox on top/bottom
			//float border = windowSize.y - windowSize.x / vAR;
			float border = (virtualSize.x / wAR - virtualSize.y) * 0.5f;
			p1.y -= border;
			p2.y += border;
		}
	}
	p1 += Vector2f(0.5f, 0.5f);
	p2 += Vector2f(0.5f, 0.5f);

	// Store values
	spVideo vid = Get();
	vid->windowSize = windowSize;
	vid->virtualSize = virtualSize;
	vid->fullscreen = _fullscreen;
	vid->p1 = p1;
	vid->p2 = p2;

	// Initialize GL projection
	glClearColor(0,0,0,0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(p1.x,p2.x,p1.y,p2.y,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapBuffers();
}


////////////////
// Get instance
spVideo Video::Get()
{
	if (!instance) instance = spVideo(new Video());
	return instance;
}
spVideo Video::instance;
