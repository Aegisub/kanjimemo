#pragma once

#include "halley/halley.h"
#include "word_result.h"

class WordStream {
public:
	virtual Halley::String GetWord(int offset=0) = 0;
	virtual void Next() = 0;
	virtual WordResult CheckResult(Halley::String entry) = 0;
};

typedef Halley::shared_ptr<WordStream> spWordStream;
