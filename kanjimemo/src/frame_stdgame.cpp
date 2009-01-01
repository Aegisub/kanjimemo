#include "halley/halley.h"
#include "frame_stdgame.h"


FrameStandardGame::FrameStandardGame()
: Frame("standard_game")
{
}


void FrameStandardGame::Init()
{
}


void FrameStandardGame::DeInit()
{

}


void FrameStandardGame::DoUpdate(float time)
{
}


void FrameStandardGame::DoRender()
{
	if (false) {
		using namespace Halley;
		spTextDrawer text = TextDrawer::GetDrawer("Verdana", 25, true, false);
		text->SetColour(Colour(1,1,1));
		text->SetBorder(Colour(0.6f,0.5f,0.3f,1),5);
		text->Print("the quick brown fox jumps over the lazy dog!\n\nTHE QUICK BROWN FOX JUMPS OVER THE LAZY DOG!", Vector2f(20, 400));
		text->Print(String(L"Moonspeak: これは日本語！ ó_o UTF-16 surrogate: 𠀐"), Vector2f(20, 500));
		//text->Print(String(L"Moonspeak: これは日本語！"), Vector2f(20, 500));

		text = TextDrawer::GetDrawer("Harrington", 30, true, false);
		text->SetColour(Colour(1,1,1));
		text->SetBorder(Colour(0.6f,0.5f,0.3f,1),5);
		text->Print("Blah blah blah testing multiple fonts.", Vector2f(20, 200));
	}
}

