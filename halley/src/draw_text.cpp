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
std::map<String, shared_ptr<TextDrawer> > TextDrawer::instances;

#ifdef _MSC_VER
#pragma warning (disable: 4748)
#endif

shared_ptr<TextDrawer> TextDrawer::GetDrawer(String face, int size, bool bold, bool italics)
{
	std::stringstream _id;
	_id << face << ":" << size << ":" << bold << ":" << italics;
	String curId = _id.str();

	std::map<String, shared_ptr<TextDrawer> >::iterator result = instances.find(curId);

	// Found, return it
	if (result != instances.end()) {
		return result->second;
	}

	// Not found, so create it
	// Get the actual font size
	int actualSize = int((size * Video::GetScale()) + 0.5);

	// Create instance
	spTextDrawer res = shared_ptr<TextDrawer>(new OpenGLText());
	res->SetFont(face,actualSize,bold,italics);
	instances[curId] = res;
	return res;
}

