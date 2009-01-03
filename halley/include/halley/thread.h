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

namespace Halley {
	class Thread {
	public:
		void Start();
		void Wait();
		bool IsDone();

		Thread();
		virtual ~Thread();

	protected:
		virtual void Run() = 0;

	private:
		bool running;
		SDL_Thread *thread;

		static int RunThread(void *data);
	};
}
