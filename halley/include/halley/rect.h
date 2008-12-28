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

#include "vector2d.h"

namespace Halley {
	//////////////////////////////
	// Vector2D class declaration
	template <typename T=float>
	class Rect2D {
		Vector2D<T> p1, p2;

		template <typename T>
		T Mod(T a, T b) const { return a % b; }

		template <>
		float Mod(float a, float b) const { return fmod(a,b); }

		template <>
		double Mod(double a, double b) const { return fmod(a,b); }

	public:
		Rect2D() {}

		Rect2D(Vector2D<T> point1, Vector2D<T> point2)
		{
			p1.x = std::min(point1.x, point2.x);
			p1.y = std::min(point1.y, point2.y);
			p2.x = std::max(point1.x, point2.x);
			p2.y = std::max(point1.y, point2.y);
		}

		Rect2D(Vector2D<T> origin, T width, T height)
		{
			*this = Rect2D(origin, Vector2D<T>(p1.x+width, p1.y+height));
		}

		Vector2D<T> GetP1() const { return p1; }
		Vector2D<T> GetP2() const { return p2; }
		Vector2D<T> GetSize() const { return p2-p1; }
		T GetWidth() const { return p2.x - p1.x; }
		T GetHeight() const { return p2.y - p1.y; }

		bool IsInside(Vector2D<T> p) const
		{
			return (p.x >= p1.x && p.x <= p2.x && p.y >= p1.y && p.y <= p2.y);
		}

		Vector2D<T> WrapInside(Vector2D<T> p) const
		{
			Vector2D<T> size = GetSize();
			return ((((p-p1) % size) + size) % size) + p1;
		}
	};

	typedef Rect2D<float> Rect4f;
}