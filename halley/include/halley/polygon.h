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

#include <vector>
#include "vector2d.h"

namespace Halley {
	class Polygon {
	private:
		void Project(const Vector2f &axis,float &min,float &max) const;
		void Unproject(const Vector2f &axis,const float point,std::vector<Vector2f> &ver) const;

	public:
		float outerRadius;
		std::vector<Vector2f> vertices;
		Vector2f origin;
		
		Polygon();
		void Realize();
		bool IsPointInside(const Vector2f &point) const;
		bool Overlaps(const Polygon &param,Vector2f *translation=NULL,Vector2f *collisionPoint=NULL) const;
	};
}
