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

#pragma once

// Include all public Halley headers
#include "SDL.h"
#include "SDL_opengl.h"
#include "interfaces.h"
#include "vector2d.h"
#include "video.h"
#include "game.h"
#include "frame_debug.h"
#include "polygon.h"
#include "sprite_collection.h"
#include "random.h"
#include "rect.h"
#include "draw_primitive.h"
#include "draw_text.h"
#include "halleystring.h"

// Macro to implement program
#define HalleyGame(T) int main(int argc, char* argv[]) { (void) argc; (void) argv; Halley::Game::SetInstance(Halley::spGame(new T)); Halley::Game::GetInstance()->Start(); return 0; }
