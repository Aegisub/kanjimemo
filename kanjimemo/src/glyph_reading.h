#pragma once
#include "halley/halley.h"
#include <vector>

class GlyphReading {
public:
	GlyphReading(Halley::String _glyph) : glyph(_glyph), reading(_glyph) {}
	GlyphReading(Halley::String _glyph, Halley::String _reading) : glyph(_glyph), reading(_reading) {}

	Halley::String GetGlyph() { return glyph; }
	Halley::String GetReading() { return reading; }

private:
	Halley::String glyph;
	Halley::String reading;
};

typedef std::vector<GlyphReading> GlyphGroup;
