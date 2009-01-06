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

#include "frame.h"
#include "input_keyboard.h"
#include "game.h"
#include <algorithm>
using namespace Halley;


///////////////
// Constructor
Frame::Frame(String _name)
: name(_name), hasBeenInit(false)
{
}


//////////////
// Destructor
Frame::~Frame()
{
	TryDeInit();
}


///////////////////////////
// Switch to another frame
void Frame::Switch(spFrame to)
{
	Destroy(to);
	if (to) to->TryInit();
}


/////////////////////////////////////
// Switch to another frame (by name)
void Frame::Switch(String to)
{
	Switch(Game::GetInstance()->CreateFrame(to));
}


/////////////////////
// Remove this frame
void Frame::Die()
{
	Destroy(spFrame());
}


/////////////////////////////////////////////
// Destroys a frame, replacing with newFrame
void Frame::Destroy(spFrame newFrame)
{
	// Remove from parent's list
	spFrameParent par = parent.lock();
	if (par) par->ChildFrameChange(weakThis.lock(),newFrame);
}


/////////////////////
// Add a child frame
void Frame::AddChild(spFrame child)
{
	if (child) {
		// Make sure that it doesn't already exist here
		if (std::find(children.begin(),children.end(),child) == children.end()) {
			// Insert in list
			children.push_back(child);

			// Switch parents, if necessary
			child->SetParent(weakThis,wpFrame(child));
			child->alive = true;

			// Initialize
			child->TryInit();
		}
	}
}


/////////////////////////////
// Add a child frame by name
void Frame::AddChild(String child)
{
	AddChild(Game::GetInstance()->CreateFrame(child));
}


//////////////////////////////////////
// Remove a destroyed child from list
void Frame::ChildFrameChange(spFrame from,spFrame to)
{
	from->alive = false;
	if (to) AddChild(to);
}


//////////////
// Set parent
void Frame::SetParent(wpFrameParent par, wpFrame wThis)
{
	spFrameParent sParent = parent.lock();
	if (sParent != par.lock()) {
		weakThis = wThis;
		if (sParent) sParent->ChildFrameChange(wThis.lock(),spFrame());
		parent = par;
	}
}


//////////
// Update
void Frame::Update(float time)
{
	DoUpdate(time);

	std::vector<spFrame> toRemove;
	for (std::list<spFrame>::iterator cur=children.begin();cur!=children.end();cur++) {
		spFrame curFrame = *cur;
		if (curFrame->alive) {
			curFrame->Update(time);
		} else {
			toRemove.push_back(curFrame);
		}
	}

	for (size_t i=0; i<toRemove.size(); i++) {
		children.remove(toRemove[i]);
	}
}


/////////
// Render
void Frame::Render()
{
	DoRender();
	for (std::list<spFrame>::iterator cur=children.begin();cur!=children.end();cur++) {
		(*cur)->Render();
	}
}


///////////////////////////
// Initialize/deinitialize
void Frame::TryInit()
{
	if (!hasBeenInit) Init();
	hasBeenInit = true;
}

void Frame::TryDeInit()
{
	if (hasBeenInit) DeInit();
	hasBeenInit = false;
}


///////////////////
// Create keyboard
spInputKeyboard Frame::CreateKeyboard(bool exclusive)
{
	if (kb)
		return kb->CreateChild(exclusive);
	else
		return parent.lock()->CreateKeyboard(exclusive);
}


////////////////
// Get keyboard
spInputKeyboard Frame::GetKeyboard(bool exclusive)
{
	if (kb)
		return kb;
	else
		return parent.lock()->CreateKeyboard(exclusive);
}


/////////////////
// Exit the game
void Frame::Exit()
{
	parent.lock()->Exit();
}
