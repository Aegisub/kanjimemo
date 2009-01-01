#include "halley/halley.h"
#include "frame_stdgame.h"
using namespace Halley;


FrameStandardGame::FrameStandardGame()
: Frame("standard_game")
{
}


void FrameStandardGame::Init()
{
	font = TextDrawer::GetDrawer("Verdana", 45, true, false);
	font->LoadGlyphs("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
}


void FrameStandardGame::DeInit()
{
}


void FrameStandardGame::DoUpdate(float time)
{
}


void FrameStandardGame::DoRender()
{
	font->SetColour(Colour(1,1,1));
	font->SetBorder(Colour(0.6f,0.5f,0.3f,1),5);
	font->Print("Never gonna give you up...", Vector2f(800, 100), Vector2f(0.5, 0));
}

