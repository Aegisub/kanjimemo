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

  Copyright (c) 2009 - Rodrigo Braz Monteiro.
  This file is subject to the terms of license.txt.

\*********************************************************/

#pragma once

#include "interfaces.h"
#include <deque>
struct SDL_KeyboardEvent;

namespace Halley {
	class InputKeyboard {
	private:
		friend class Game;

		std::deque<wpInputKeyboard> children;
		std::vector<bool> keyPressed;
		std::vector<bool> keyReleased;
		std::vector<bool> keyDown;
		std::deque<int> letters;
		bool isExclusive;
		
		InputKeyboard(bool exclusive=false);
		
		void VerifyChildren();

		void ProcessEvent(const SDL_KeyboardEvent &event);
		void DoProcessEvent(const SDL_KeyboardEvent &event);
		void ClearPresses();
		void DoClearPresses();

		void KeyPressed(int code);
		void KeyReleased(int code);
		void LetterEntered(int code);

	public:
		spInputKeyboard CreateChild(bool exclusive);

		bool IsKeyPressed(int code);
		bool IsKeyReleased(int code);
		bool IsKeyDown(int code);
		int GetNextLetter();
	};
}
