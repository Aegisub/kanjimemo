#include "player_progress.h"
using namespace Halley;


void PlayerProgress::GotReadingCorrect(String word)
{

}

void PlayerProgress::GotReadingWrong(String word, String reading)
{

}

float PlayerProgress::GetProgress(GlyphReading glyph)
{
	std::map<GlyphReading, float>::iterator res = progress.find(glyph);
	if (res != progress.end()) {
		return res->second;
	}
	return 0.5f;
}
