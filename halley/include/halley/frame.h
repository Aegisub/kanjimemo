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

#include <list>
#include "interfaces.h"

namespace Halley {
	class Frame;
	typedef shared_ptr<Frame> spFrame;
	typedef weak_ptr<Frame> wpFrame;

	// Frame class
	class Frame : public IFrameParent {
		friend class Game;

	private:
		std::string name;
		wpFrameParent parent;
		wpFrame weakThis;
		std::list<spFrame> children;
		bool hasBeenInit;

		void Destroy(spFrame newFrame);

		void TryInit();
		void TryDeInit();

		void Update(float time);
		void Render();
		void ChildFrameChange(spFrame from,spFrame to);
		void SetParent(wpFrameParent parent, wpFrame weakThis);

	protected:
		virtual void Init() {}
		virtual void DeInit() {}
		virtual void DoUpdate(float time)=0;
		virtual void DoRender()=0;

		void Switch(spFrame to);
		void Die();
		void AddChild(spFrame child);

	public:
		Frame(std::string name);
		virtual ~Frame();
	};

}
