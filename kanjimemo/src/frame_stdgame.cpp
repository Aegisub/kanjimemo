#include "halley/halley.h"
#include "frame_stdgame.h"
#include "word_stream_std.h"
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

	font->LoadGlyphs("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ");
	curInput = "";
	kb = GetKeyboard(true);

	spKanjiMemo game = std::tr1::dynamic_pointer_cast<KanjiMemo>(Game::GetInstance());
	spGlyphSet glyphs(new GlyphSet("katakana"));
	spPlayerProgress progress(new PlayerProgress());
	words = spWordStream(new StandardWordStream(game, glyphs, progress));
}


void FrameStandardGame::DeInit()
{
}


void FrameStandardGame::DoUpdate(float time)
{
	// Quit program
	if (kb->IsKeyPressed(SDLK_ESCAPE)) {
		Exit();
		return;
	}

	// Get text input
	while (int letter = kb->GetNextLetter()) {
		// Enter pressed
		if (letter == '\r') {
			WordResult result = words->CheckResult(GetNormalizedInput());
			if (result.success) words->Next();
			kanjiFonts[0]->Clear();

			curInput.clear();
		}
		
		// Normal key
		else {
			curInput.AppendCharacter(letter);
		}
	}
}


void FrameStandardGame::DoRender()
{
	spTextDrawer f;
	
	f = kanjiFonts[0];
	f->SetColour(Colour(1,1,1));
	f->SetBorder(Colour(), 5);
	f->Print(words->GetWord(), Vector2f(800,500), Vector2f(0.5f, 0.5f));

	f = font;
	f->SetColour(Colour(1,1,1));
	f->SetBorder(Colour(),3);
	f->Print(GetNormalizedInput()+"_", Vector2f(800, 100), Vector2f(0.5f, 0));
}

String FrameStandardGame::GetNormalizedInput()
{
	String input = curInput.AsciiLower();
	input.Replace("aa",String(257));
	input.Replace("ee",String(275));
	//input.Replace("ii",String(299));
	input.Replace("oo",String(333));
	input.Replace("ou",String(333));
	input.Replace("uu",String(363));
	return input;
}
