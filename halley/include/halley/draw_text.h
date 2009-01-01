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

#include <string>
#include <map>
#include "colour.h"
#include "interfaces.h"
#include "vector2d.h"
#include "halleystring.h"

namespace Halley {
	// OpenGL Text Drawing class
	class TextDrawer {
	private:
		static std::map<String, shared_ptr<TextDrawer> > instances;

	protected:
		virtual void SetFont(String face,int size,bool bold,bool italics) = 0;

	public:
		virtual ~TextDrawer() {}

		static shared_ptr<TextDrawer> GetDrawer(String face="Verdana",int size=10,bool bold=true,bool italics=false);

		virtual void SetColour(Colour col) = 0;
		virtual void SetBorder(Colour col, float width) = 0;
		virtual void Print(String text, Vector2f pos, float scale=1.0f) = 0;
		virtual void GetExtent(String text, Vector2f &pos) = 0;

		virtual void LoadGlyphs(String text) = 0;
		virtual void Clear() = 0;
	};

	typedef shared_ptr<TextDrawer> spTextDrawer;
}
