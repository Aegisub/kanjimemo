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


#include "polygon.h"
using namespace Halley;


///////////////
// Constructor
Polygon::Polygon()
{
	outerRadius = 0.0f;
}


//////////////////////////////////////////////
// Realize that the polygon has changed shape
void Polygon::Realize()
{
	// Calculate the outer radius
	outerRadius = 0.0f;
	size_t len = vertices.size();
	float cur;
	for (size_t i=0;i<len;i++) {
		cur = vertices[i].SquaredLength();
		if (cur > outerRadius) outerRadius = cur;
	}
	outerRadius = sqrt(outerRadius);
}


///////////////////////////////////////////////////////
// Checks if a particular point is inside the polygon
bool Polygon::IsPointInside(const Vector2f &point) const
{
	Vector2f p = point - origin;
	Vector2f a,b;

	// Do cross product with all the segments, except for last
	size_t len = vertices.size();
	for (size_t i=0;i<len-1;i++) {
		a = p - vertices[i];
		b = vertices[i+1] - vertices[i];
		if (a.Cross(b) < 0) return false;
	}

	// Try last segment (duplicated code just to avoid a %, fuck yeah)
	len--;
	a = p - vertices[len];
	b = vertices[0] - vertices[len];
	if (a.Cross(b) < 0) return false;

	// Nothing failed, so it's inside
	return true;
}


///////////////////////////////////////////////////////
// Checks if any of the points are inside the polygon
// Using the separating axis theorem here
bool Polygon::Overlaps(const Polygon &param,Vector2f *translation,Vector2f *collisionPoint) const
{
	// Check if they are within overlap range
	float maxDist = outerRadius + param.outerRadius;
	if ((origin-param.origin).SquaredLength() >= maxDist*maxDist) return false;

	// Prepare
	float minDist = -999999.0f;
	Vector2f bestAxis;
	bool hasBestAxis = false;
	float bmin1=0,bmax1=0,bmin2=0,bmax2=0;

	// For each edge
	size_t len1 = vertices.size();
	size_t len2 = param.vertices.size();
	for (size_t i=0;i<len1+len2;i++) {
		// Find the orthonormal axis
		Vector2f axis;
		if (i < len1) axis = (vertices[(i+1)%len1]-vertices[i]).GetOrthoLeft().GetUnit();
		else axis = (param.vertices[(i-len1+1)%len2]-param.vertices[i-len1]).GetOrthoLeft().GetUnit();

		// Project both polygons there
		float min1,max1,min2,max2;
		Project(axis,min1,max1);
		param.Project(axis,min2,max2);

		// Find the distance between the projections
		float dist = min1<min2 ? min2 - max1 : min1 - max2;
		if (dist >= 0) return false;
		else {
			if (translation && dist > minDist) {
				bestAxis = axis;
				hasBestAxis = true;
				minDist = dist;
				bmin1 = min1;
				bmin2 = min2;
				bmax1 = max1;
				bmax2 = max2;
			}
		}
	}

	// Gather additional data based on best axis
	if (hasBestAxis) {
		// Find all vertices possibly involved in the collision
		float dist;
		int sign;
		std::vector<Vector2f> v1,v2;
		if (bmin1 < bmin2) {
			dist = bmin2 - bmax1;
			sign = 1;
			if (collisionPoint) {
				Unproject(bestAxis,bmax1,v1);
				param.Unproject(bestAxis,bmin2,v2);
			}
		}
		else {
			dist = bmin1 - bmax2;
			sign = -1;
			if (collisionPoint) {
				Unproject(bestAxis,bmin1,v1);
				param.Unproject(bestAxis,bmax2,v2);
			}
		}

		// Find the collision point
		if (collisionPoint) {
			Vector2f colPoint = (origin + param.origin)/2.0f;
			if (v1.size() == 1) colPoint = v1[0];
			else if (v2.size() == 1) colPoint = v2[0];
			else if (v1.size() > 0) colPoint = v1[0];
			else if (v2.size() > 0) colPoint = v2[0];
			*collisionPoint = colPoint;
		}

		// Find the translation vector
		*translation = bestAxis*(dist*sign);
	}

	// Done
	return true;
}


////////////////////////////////
// Project polygon into an axis
void Polygon::Project(const Vector2f &_axis,float &_min,float &_max) const
{
	Vector2f axis = _axis;
	float dot = axis.Dot(vertices[0]+origin);
	float min = dot;
	float max = dot;
	size_t len = vertices.size();
	for (size_t i=1;i<len;i++) {
		dot = axis.Dot(vertices[i]+origin);
		if (dot < min) min = dot;
		else if (dot > max) max = dot;
	}
	_min = min;
	_max = max;
}


/////////////
// Unproject
// Finds all vertices whose projection on a given axis is the value given
void Polygon::Unproject(const Vector2f &axis,const float point,std::vector<Vector2f> &ver) const
{
	size_t len = vertices.size();
	float dot;
	for (size_t i=0;i<len;i++) {
		dot = axis.Dot(vertices[i]+origin);
		if (dot == point) ver.push_back(vertices[i] + origin);
	}
}
