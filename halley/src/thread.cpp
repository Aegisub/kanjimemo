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

#include "thread.h"
#include "SDL.h"
#include "SDL_thread.h"
using namespace Halley;


Thread::Thread()
: running(false), thread(NULL)
{
}

Thread::~Thread()
{
	if (thread) {
		thread = NULL;
	}
}

void Thread::Start()
{
	thread = SDL_CreateThread(RunThread, this);
	running = true;
}

void Thread::Wait()
{
	assert (thread);
	SDL_WaitThread(thread, NULL);
}

bool Thread::IsDone()
{
	return !running;
}

int Thread::RunThread(void *data)
{
	Thread *thread = (Thread*) data;
	thread->Run();
	thread->running = false;
	return 0;
}
