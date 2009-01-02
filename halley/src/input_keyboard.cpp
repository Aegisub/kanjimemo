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

#include "input_keyboard.h"
using namespace Halley;

InputKeyboard::InputKeyboard(bool _isExclusive)
{
	isExclusive = _isExclusive;

	int nKeys = SDLK_LAST - SDLK_FIRST;
	keyPressed.resize(nKeys);
	keyReleased.resize(nKeys);
	keyDown.resize(nKeys);
}

void InputKeyboard::ProcessEvent(const SDL_KeyboardEvent &event)
{
	VerifyChildren();
	size_t len = children.size();
	bool gotExclusive = false;
	for (size_t i=0; i<len; i++) {
		spInputKeyboard child = children[i].lock();
		child->ProcessEvent(event);
		
		gotExclusive = child->isExclusive;
		if (gotExclusive)
			break;
	}

	if (!gotExclusive) {
		DoProcessEvent(event);
	}
}

void InputKeyboard::DoProcessEvent(const SDL_KeyboardEvent &event)
{
	switch (event.type) {
		case SDL_KEYUP:
			KeyPressed(event.keysym.sym);
			break;
		case SDL_KEYDOWN:
			KeyPressed(event.keysym.sym);
			LetterEntered(event.keysym.unicode);
			break;
	}
}

spInputKeyboard InputKeyboard::CreateChild(bool exclusive)
{
	spInputKeyboard child(new InputKeyboard(exclusive));
	
	if (exclusive)
		children.push_front(child);
	else
		children.push_back(child);
	
	return child;
}

void InputKeyboard::VerifyChildren()
{
	for (size_t i=0; i<children.size();) {
		if (children[i].expired()) {
			children.erase(children.begin() + i);
		} else {
			i++;
		}
	}
}

void InputKeyboard::KeyPressed(int code)
{
	keyPressed[code] = true;
	keyDown[code] = true;
}

void InputKeyboard::KeyReleased(int code)
{
	keyReleased[code] = true;
	keyDown[code] = false;
}

void InputKeyboard::LetterEntered(int code)
{
	letters.push_back(code);
}

void InputKeyboard::ClearPresses()
{
	VerifyChildren();
	size_t len = children.size();
	for (size_t i=0; i<len; i++) {
		spInputKeyboard child = children[i].lock();
		child->DoClearPresses();
	}

	DoClearPresses();
}

void InputKeyboard::DoClearPresses()
{
	size_t len = keyPressed.size();
	for (size_t i=0; i<len; i++) {
		keyPressed[i] = false;
		keyReleased[i] = false;
	}
}

bool InputKeyboard::IsKeyPressed(int code)
{
	return keyPressed[code];
}

bool InputKeyboard::IsKeyReleased(int code)
{
	return keyReleased[code];
}

bool InputKeyboard::IsKeyDown(int code)
{
	return keyDown[code];
}

int InputKeyboard::GetNextLetter()
{
	if (letters.size()) {
		int letter = letters.front();
		letters.pop_front();
		return letter;
	} else {
		return 0;
	}
}
