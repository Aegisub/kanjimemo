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

#include "sprite_collection.h"
using namespace Halley;

void SpriteCollection::Update(float time)
{
	for (std::list<spSprite>::iterator cur = begin(); cur != end(); cur++) {
		(*cur)->Update(time);
	}
}


void SpriteCollection::Draw()
{
	for (std::list<spSprite>::iterator cur = begin(); cur != end(); cur++) {
		(*cur)->Draw();
	}
}
