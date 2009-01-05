#include "player_progress.h"
#include "kanjimemo.h"
using namespace Halley;


void PlayerProgress::GotReadingCorrect(String wordStr)
{
	// Get manager
	using namespace Japanese;
	spKanjiMemo km = dynamic_pointer_cast<KanjiMemo>(Game::GetInstance());
	KanjiManager &kanji = km->kanji;
	WordManager &words = km->words;

	// Get word
	Word word = words.GetWord(wordStr);
	if (!word.IsValid()) {
		word = Word(wordStr, wordStr);
	}

	// Get word reading
	WordReading reading = word.GetWordReading(kanji);
	WordReadingList readingList = reading.GetReadings();
	size_t len = readingList.size();

	// Loop through readings
	for (size_t i=0; i<len; i++) {
		WordReadingFragment frag = readingList[i];
		UpdateProgress(GlyphReading(frag.text, frag.reading), 0.1f);
	}
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

void PlayerProgress::UpdateProgress(GlyphReading glyph, float score)
{
	std::map<GlyphReading, float>::iterator res = progress.find(glyph);
	if (res == progress.end()) {
		progress[glyph] = 0.5f;
	}

	progress[glyph] = std::max(0.0f, std::min(1.0f, progress[glyph] + score));
}
