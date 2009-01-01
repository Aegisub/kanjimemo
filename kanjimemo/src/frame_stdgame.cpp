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
	kanjiFonts.push_back(TextDrawer::GetDrawer("MS Mincho", 150, true, false));

	curInput = "";
	kb = GetKeyboard(true);
}


void FrameStandardGame::DeInit()
{
}


void FrameStandardGame::DoUpdate(float time)
{
	// Quit program
	if (kb->IsKeyPressed(SDLK_ESCAPE)) {
		Exit();
	}

	// Get text input
	while (int letter = kb->GetNextLetter()) {
		curInput.AppendCharacter(letter);
	}
}


void FrameStandardGame::DoRender()
{
	spTextDrawer f;
	
	f = kanjiFonts[0];
	f->SetColour(Colour(1,1,1));
	f->SetBorder(Colour(), 5);
	f->Print(String(L"日本語"), Vector2f(800,500), Vector2f(0.5f, 0.5f));

	f = font;
	f->SetColour(Colour(1,1,1));
	f->SetBorder(Colour(),3);
	f->Print(curInput+"_", Vector2f(800, 100), Vector2f(0.5f, 0));
}

