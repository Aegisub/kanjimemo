#pragma once

#include "word_stream.h"
#include "japanese.h"
#include "kanjimemo.h"
#include "glyph_set.h"
#include "player_progress.h"
#include <deque>

class StandardWordStream : public WordStream {
public:
	StandardWordStream(spKanjiMemo game, spGlyphSet glyphs, spPlayerProgress progress);

	Halley::String GetWord(int offset=0);
	void Next();
	WordResult CheckResult(Halley::String entry);

private:
	std::deque<Halley::String> words;
	size_t pos;
	size_t maxHistory;
	spGlyphSet glyphs;
	spPlayerProgress progress;

	Japanese::KanaConverter &kanaConverter;
	Japanese::KanjiManager &kanji;
	Japanese::WordManager &jwords;

};
