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

#include "frame_debug.h"


//////////////
// Initialize
void FrameDebug::Init()
{
	step = 0;
}


////////////////
// Deinitialize
void FrameDebug::DeInit()
{
}


//////////
// Update
void FrameDebug::DoUpdate(float time)
{
	(void) time;
	step++;
	if (step >= 256) step = 0;
}


//////////
// Render
void FrameDebug::DoRender()
{
	glClearColor(std::min(step,255-step)/127.0f,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
}
