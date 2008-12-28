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
#include "colour.h"
#include "interfaces.h"
#include "vector2d.h"

namespace Halley {
	// OpenGL Text Drawing class
	class TextDrawer {
	private:
		static shared_ptr<TextDrawer> GetInstance();

	protected:
		virtual void DoSetFont(std::string face,int size,bool bold,bool italics) = 0;
		virtual void DoSetColour(Colour col) = 0;
		virtual void DoPrint(std::string text, Vector2f pos) = 0;
		virtual void DoGetExtent(std::string text, Vector2f &pos) = 0;

	public:
		virtual ~TextDrawer() {}

		static void SetFont(std::string face="Verdana",int size=10,bool bold=true,bool italics=false) { GetInstance()->DoSetFont(face,size,bold,italics); }
		static void SetColour(Colour col) { GetInstance()->DoSetColour(col); }
		static void Print(std::string text, Vector2f pos) { GetInstance()->DoPrint(text,pos); }
		static void GetExtent(std::string text, Vector2f &pos) { GetInstance()->DoGetExtent(text,pos); }
	};
}
