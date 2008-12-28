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

#include <memory>

#pragma once

namespace Halley {

	using std::tr1::shared_ptr;
	using std::tr1::weak_ptr;


	// Forward references
	class Frame;
	class Sprite;


	// Smart pointers
	typedef shared_ptr<Frame> spFrame;
	typedef weak_ptr<Frame> wpFrame;
	typedef shared_ptr<Sprite> spSprite;
	typedef weak_ptr<Sprite> wpSprite;


	// FrameParent interface, used by both frames themselves and by the game core
	class IFrameParent {
	public:
		virtual ~IFrameParent() {}
		virtual void ChildFrameChange(shared_ptr<Frame> from,shared_ptr<Frame> to)=0;
	};
	typedef weak_ptr<IFrameParent> wpFrameParent;
	typedef shared_ptr<IFrameParent> spFrameParent;

}
