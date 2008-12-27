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
#include <algorithm>
using namespace Halley;


///////////////
// Constructor
Frame::Frame(std::string _name)
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


/////////////////////
// Remove this frame
void Frame::Die()
{
	//Destroy(spFrame());
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

			// Initialize
			child->TryInit();
		}
	}
}


//////////////////////////////////////
// Remove a destroyed child from list
void Frame::ChildFrameChange(spFrame from,spFrame to)
{
	children.remove(from);
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
	for (std::list<spFrame>::iterator cur=children.begin();cur!=children.end();cur++) {
		(*cur)->Update(time);
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