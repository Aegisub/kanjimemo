#pragma once

#include "word_stream.h"
#include "japanese.h"
#include "kanjimemo.h"
#include <deque>

class StandardWordStream : public WordStream {
private:
	std::deque<Halley::String> words;
	size_t pos;
	size_t maxHistory;

	Japanese::KanaConverter &kanaConverter;
	Japanese::KanjiManager &kanji;
	Japanese::WordManager &jwords;

public:
	StandardWordStream(Halley::shared_ptr<KanjiMemo> game);

	Halley::String GetWord(int offset=0);
	void Next();
	WordResult CheckResult(Halley::String entry);
};
