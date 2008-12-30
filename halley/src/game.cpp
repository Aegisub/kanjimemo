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

#include "SDL.h"
#include "SDL_opengl.h"
#include "game.h"
#include <ctime>
using namespace Halley;


///////////////
// Constructor
Game::Game()
{
	run = true;
	fps = 60;
	srand((unsigned)time(NULL));
}


//////////////
// Destructor
Game::~Game()
{
}


/////////
// Start
void Game::Start()
{
	// Initialize SDL
	int result;
	result = SDL_Init(SDL_INIT_EVERYTHING);
	if (result == -1) {
		// TODO
		throw std::exception(SDL_GetError());
	}

	// Initialize the game
	try {
		Init();
	}
	catch (...) {}

	// Run the game
	try {
		Run();
	}
	catch (...) {}

	// Deinitialize the game
	try {
		DeInit();
	}
	catch (...) {}

	// Close SDL
	SDL_Quit();
}


///////
// Run
void Game::Run()
{
	// Set up the counters
	unsigned int timePerFrame = 1000/fps;
	unsigned int lastTime = SDL_GetTicks();
	unsigned int curTime = lastTime;

	// Run game
	while (run) {
		// Handle events
		PollEvents();
		if (!run) break;

		// Update the logic
		topFrame->Update(timePerFrame/1000.0f);

		// If it has time, update the screen
		curTime = SDL_GetTicks();
		if (curTime - lastTime < timePerFrame) {
			// Update screen
			topFrame->Render();
			SDL_GL_SwapBuffers();

			// Give away the remaining time
			curTime = SDL_GetTicks();
			while (curTime - lastTime < timePerFrame) {
				SDL_Delay(timePerFrame+lastTime-curTime);
				curTime = SDL_GetTicks();
			}
		}
		lastTime += timePerFrame;

		// Check if it has fallen too much behind (over one second)
		if (curTime - lastTime > 1000) lastTime = curTime;
	}
}


/////////////
// Quit game
void Game::Quit()
{
	SetTopFrame(spFrame());
}


/////////////////////////////
// Child frame was destroyed
void Game::ChildFrameChange(spFrame from,spFrame to)
{
	(void)from;
	if (!to) Quit();
	else SetTopFrame(to);
}


/////////////////
// Set top frame
void Game::SetTopFrame(spFrame frame)
{
	if (topFrame) topFrame->Die();
	if (frame) {
		topFrame = frame;
		frame->SetParent(instance, frame);
		frame->TryInit();
	}
	else run = false;
}


////////////////
// Get instance
spGame Game::GetInstance()
{
	return instance;
}
spGame Game::instance;


////////////////
// Set instance
void Game::SetInstance(spGame game)
{
	if (instance) throw std::exception("Game already exists!");
	instance = game;
}


///////////////
// Poll events
void Game::PollEvents()
{
	SDL_Event event;
	SDL_PumpEvents();
	while (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_ALLEVENTS) > 0) {
		switch (event.type) {
			case SDL_KEYDOWN:
				{
					int type = event.key.type;
					int code = event.key.keysym.sym;
					int mod = event.key.keysym.mod;
					if (code == SDLK_ESCAPE) Quit();
					break;
				}
			case SDL_QUIT:
				{
					Quit();
					break;
				}
		}
	}
}
