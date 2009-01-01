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

#include <cmath>
#include "angle.h"

namespace Halley {
	//////////////////////////////
	// Vector2D class declaration
	template <typename T=float,class U=Angle<float> >
	class Vector2D {
	private:
		template <typename T>
		T Mod(T a, T b) const { return a % b; }

		template <>
		float Mod(float a, float b) const { return fmod(a,b); }

		template <>
		double Mod(double a, double b) const { return fmod(a,b); }

	public:
		T x,y;

		// Constructors
		Vector2D () { x = y = 0; }
		Vector2D (const T _x,const T _y) { x = _x; y = _y; }
		Vector2D (const Vector2D<T> &vec) { x = vec.x; y = vec.y; }
		template <typename V>
		explicit Vector2D (const Vector2D<V> &vec) { x = (T)vec.x; y = (T)vec.y; }
		Vector2D (const T length,const U &angle) { x = angle.Cos()*length; y = angle.Sin()*length; }

		// Assignment and comparison
		inline void operator = (const Vector2D &param) { x = param.x; y = param.y; }
		inline void operator = (const T param) { x = param; y = param; }
		inline bool operator == (const Vector2D &param) const { return x == param.x && y == param.y; }
		inline bool operator != (const Vector2D &param) const { return x != param.x || y != param.y; }

		// Basic algebra
		inline Vector2D operator + (const Vector2D &param) const { return Vector2D(x + param.x,y + param.y); }
		inline Vector2D operator - (const Vector2D &param) const { return Vector2D(x - param.x,y - param.y); }
		inline Vector2D operator * (const Vector2D &param) const { return Vector2D(x * param.x,y * param.y); }
		inline Vector2D operator / (const Vector2D &param) const { return Vector2D(x / param.x,y / param.y); }
		inline Vector2D operator % (const Vector2D &param) const { return Vector2D(Mod<T>(x, param.x), Mod<T>(y, param.y)); }

		inline Vector2D operator - () const { return Vector2D(-x,-y); }

		inline Vector2D operator * (const T param) const { return Vector2D(x * param,y * param); }
		inline Vector2D operator / (const T param) const { return Vector2D(x / param,y / param); }

		// In-place operations
		inline Vector2D operator += (const Vector2D &param) { x += param.x; y += param.y; return *this; }
		inline Vector2D operator -= (const Vector2D &param) { x -= param.x; y -= param.y; return *this; }
		inline Vector2D operator *= (const T param) { x *= param; y *= param; return *this; }
		inline Vector2D operator /= (const T param) { x /= param; y /= param; return *this; }

		// Get the normalized vector (unit vector)
		inline Vector2D GetUnit () const
		{
			float len = Length();
			if (len != 0) {
				Vector2D temp;
				temp.x = x;
				temp.y = y;
				return temp / len;
			}
			else return Vector2D(0,0);
		}

		// Get the orthogonal vector
		inline Vector2D GetOrthoLeft () const { return Vector2D(-y,x); }
		inline Vector2D GetOrthoRight () const { return Vector2D(y,-x); }

		// Cross product (the Z component of it)
		inline T Cross (const Vector2D &param) const { return x * param.y - y * param.x; }

		// Dot product
		inline T Dot (const Vector2D &param) const { return (x * param.x) + (y * param.y); }

		// Length
		inline T Length () const { return sqrt(x*x + y*y); }
		inline T Len () const { return Length(); }

		// Squared length, often useful and much faster
		inline T SquaredLength () const { return x*x+y*y; }

		// Projection on another vector
		inline Vector2D Projection (const Vector2D &param) const { Vector2D unit = param.GetUnit(); return Dot(unit) * unit; }
		inline T ProjectionLength (const Vector2D &param) const { Vector2D unit = param.GetUnit(); return Dot(unit); }

		// Gets the angle that this vector is pointing to
		inline U GetAngle () const
		{
			U angle;
			angle.SetRadian(atan2(y,x));
			return angle;
		}

		// Rotates vector by an angle
		inline Vector2D Rotate (const U angle) const
		{
			T cos = angle.Cos();
			T sin = angle.Sin();
			return Vector2D(x*cos - y*sin,x*sin + y*cos);
		}

		// Rotates vector by sine and cosine
		inline Vector2D Rotate (const T sine,const T cosine)
		{
			return Vector2D(x*cosine - y*sine,x*sine + y*cosine);
		}
	};


	////////////////////
	// Global operators
	template <typename T,class U,typename V> inline Vector2D<T,U> operator * (V f,const Vector2D<T,U> &v) { return Vector2D<T,U>(T(v.x * f),T(v.y * f)); }
	template <typename T,class U,typename V> inline Vector2D<T,U> operator / (V f,const Vector2D<T,U> &v) { return Vector2D<T,U>(T(f / v.x),T(f / v.y)); }

	////////////
	// Typedefs
	typedef Vector2D<double,Angle<double> > Vector2d;
	typedef Vector2D<> Vector2f;
	typedef Vector2D<int> Vector2i;
	typedef Vector2D<short> Vector2s;
	typedef Vector2D<char> Vector2c;
	typedef Vector2f Position;
	typedef Vector2f Size;
}
