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
#include "halleystring.h"

namespace Halley {
	// Frame class
	class Frame : public IFrameParent {
		friend class Game;

	private:
		String name;
		bool hasBeenInit;
		bool alive;

		wpFrameParent parent;
		wpFrame weakThis;
		spInputKeyboard kb;

		std::list<spFrame> children;

		void Destroy(spFrame newFrame);

		void TryInit();
		void TryDeInit();

		void ChildFrameChange(spFrame from,spFrame to);
		spInputKeyboard CreateKeyboard(bool exclusive);

		void SetParent(wpFrameParent parent, wpFrame weakThis);

		void Update(float time);
		void Render();

	protected:
		virtual void Init() {}
		virtual void DeInit() {}
		virtual void DoUpdate(float time)=0;
		virtual void DoRender()=0;

		void AddChild(spFrame child);
		void AddChild(String child);
		void Switch(spFrame to);
		void Switch(String to);

		void Die();
		void Exit();
		spInputKeyboard GetKeyboard(bool exclusive);

	public:
		explicit Frame(String name);
		virtual ~Frame();
	};

}
