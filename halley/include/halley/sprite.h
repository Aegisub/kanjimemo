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
#include "interfaces.h"

namespace Halley {
	class Sprite {
	private:
		Vector2f pos;
		Vector2f scale;
		Vector2f pivot;
		Angle1f rotation;

	protected:
		virtual void DoDraw() {}

	public:
		virtual void Update(float time) { (void) time; }
		void Draw();

		Vector2f GetPosition() const { return pos; }
		Vector2f GetPivot() const { return pivot; }
		Vector2f GetScale() const { return scale; }
		Angle1f GetRotation() const { return rotation; }

		void SetPosition(Vector2f _pos) { pos = _pos; }
		void SetPivot(Vector2f _pivot) { pivot = _pivot; }
		void SetScale(Vector2f _scale) { scale = _scale; }
		void SetRotation(Angle1f _rotation) { rotation = _rotation; }
	};
}
