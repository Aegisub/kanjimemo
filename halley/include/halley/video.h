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

#pragma once
#include "interfaces.h"
#include "vector2d.h"

namespace Halley {
	class Video;
	typedef shared_ptr<Video> spVideo;

	class Video {
	private:
		Vector2i windowSize;
		Vector2f virtualSize;
		Vector2f p1, p2;
		bool fullscreen;
		static spVideo instance;

		static spVideo Get();
		Video();

	public:
		static void SetVideo(bool fullscreen, Vector2i windowSize, Vector2f virtualSize=Vector2f());
		static Vector2i GetWindowSize() { return Get()->windowSize; }
		static Vector2f GetVirtualSize() { return Get()->virtualSize; }
		static Vector2f GetDisplaySize() { return Get()->p2 - Get()->p1; }
		static Vector2f GetOrigin() { return Get()->p1; }
		static bool IsFullscreen() { return Get()->fullscreen; }
	};
}
