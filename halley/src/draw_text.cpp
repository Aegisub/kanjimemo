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

#include "draw_text.h"
#include "draw_text_gl.h"
#include "video.h"
#include <sstream>
using namespace Halley;


//////////////////
// Static members
std::string TextDrawer::curId;
std::map<std::string, shared_ptr<TextDrawer> > TextDrawer::instances;
Colour TextDrawer::col;
Colour TextDrawer::borderColor;
float TextDrawer::borderWidth;


shared_ptr<TextDrawer> TextDrawer::GetInstance()
{
	return instances[curId];
}


void TextDrawer::SetFont(std::string face, int size, bool bold, bool italics)
{
	std::stringstream _id;
	_id << face << ":" << size << ":" << bold << ":" << italics;
	curId = _id.str();

	std::map<std::string, shared_ptr<TextDrawer> >::iterator result = instances.find(curId);
	if (result == instances.end()) {
		// Get the actual font size
		int actualSize = int((size * Video::GetScale()) + 0.5);

		// Create instance
		instances[curId] = shared_ptr<TextDrawer>(new OpenGLText());
		GetInstance()->DoSetFont(face,actualSize,bold,italics);
	}
}


void TextDrawer::SetColour(Colour _col)
{
	col = _col;
}


void TextDrawer::SetBorder(Colour _col, float width)
{
	borderColor = _col;
	borderWidth = width*Video::GetScale();
}


void TextDrawer::Print(std::string text, Vector2f pos)
{
	shared_ptr<TextDrawer> instance = GetInstance();
	instance->DoSetColour(col);
	instance->DoSetBorder(borderColor, borderWidth);
	instance->DoPrint(text,pos,1/Video::GetScale());
}


void TextDrawer::GetExtent(std::string text, Vector2f &pos)
{
	GetInstance()->DoGetExtent(text,pos);
}
