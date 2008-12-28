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

#include "vector2d.h"

namespace Halley {
	class PrimitiveDrawer {
	public:
		void SetCenter(Vector2f pos);
		void SetScale(Vector2f scale);
		void SetRotation(Angle1f rotation);
		void SetPivot(Vector2f pivot);

		void DrawRectangle(Vector2f size) const;
		void DrawSquare(float side) const;

		void DrawLine(Vector2f p1, Vector2f p2) const;
		void DrawDashedLine(Vector2f p1, Vector2f p2,float dashLen) const;

		void DrawEllipse(float radiusX,float radiusY) const;
		void DrawCircle(float radius) const { DrawEllipse(radius,radius); }
		void DrawRing(float r1,float r2,float ar=1.0f,float arcStart=0.0f,float arcEnd=0.0f) const;

		void DrawTriangle(Vector2f p1, Vector2f p2, Vector2f p3) const;
		void DrawStar(int tips, float rad1, float rad2) const;
	};
}
