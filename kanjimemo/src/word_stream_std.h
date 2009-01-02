#pragma once

#include "word_stream.h"
#include <deque>

class StandardWordStream : public WordStream {
private:
	std::deque<Halley::String> words;
	size_t pos;
	size_t maxHistory;

public:
	StandardWordStream();

	Halley::String GetWord(int offset=0);
	void Next();
	WordResult CheckResult(Halley::String entry);
};
