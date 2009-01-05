#pragma once
#include "halley/halley.h"
#include "glyph_reading.h"

class PlayerProgress {
public:
	void GotReadingCorrect(Halley::String word);
	void GotReadingWrong(Halley::String word, Halley::String reading);

	float GetProgress(GlyphReading glyph);

private:
	std::map<GlyphReading, float> progress;

	void UpdateProgress(GlyphReading glyph, float score);
};

typedef Halley::shared_ptr<PlayerProgress> spPlayerProgress;
