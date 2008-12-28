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

namespace Halley {
	// OpenGL Text Drawing class
	class TextDrawer {
	private:
		static Colour col;
		static std::string curId;
		static std::map<std::string, shared_ptr<TextDrawer> > instances;
		static shared_ptr<TextDrawer> GetInstance();

	protected:
		virtual void DoSetFont(std::string face,int size,bool bold,bool italics) = 0;
		virtual void DoSetColour(Colour col) = 0;
		virtual void DoPrint(std::string text, Vector2f pos, float scale) = 0;
		virtual void DoGetExtent(std::string text, Vector2f &pos) = 0;

	public:
		virtual ~TextDrawer() {}

		static void SetFont(std::string face="Verdana",int size=10,bool bold=true,bool italics=false);
		static void SetColour(Colour col);
		static void Print(std::string text, Vector2f pos);
		static void GetExtent(std::string text, Vector2f &pos);
	};
}
