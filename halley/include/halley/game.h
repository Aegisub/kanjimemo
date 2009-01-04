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

#include "frame.h"
#include "interfaces.h"
#include "input_keyboard.h"

namespace Halley {
	class Game;
	typedef shared_ptr<Game> spGame;
	typedef weak_ptr<Game> wpGame;

	class Game : public IFrameParent {
	private:
		static spGame instance;

		int fps;
		bool run;
		String name;
		spFrame topFrame;
		InputKeyboard keyboard;
		String programPath;

		void Exit();
		void ChildFrameChange(spFrame from,spFrame to);
		spInputKeyboard CreateKeyboard(bool exclusive);
		void ParseProgramPath(String path);

		void Run();
		void PollEvents();

	protected:
		virtual void Init()=0;
		virtual void DeInit()=0;
		void SetTopFrame(spFrame frame);

		void Quit();

	public:
		Game(String gameName);
		virtual ~Game();

		void Start(StringArray args);

		static spGame GetInstance();
		static void SetInstance(spGame game);
		static String GetProgramPath();
	};
}
