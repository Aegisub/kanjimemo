#pragma once
#include "halley/halley.h"
#include <vector>

class GlyphReading {
public:
	GlyphReading(Halley::String _glyph) : glyph(_glyph), reading(_glyph) {}
	GlyphReading(Halley::String _glyph, Halley::String _reading) : glyph(_glyph), reading(_reading) {}

	Halley::String GetGlyph() { return glyph; }
	Halley::String GetReading() { return reading; }

	bool operator==(const GlyphReading& p) const { return glyph == p.glyph && reading == p.reading; }
	bool operator<(const GlyphReading& p) const { return glyph+reading < p.glyph+p.reading; }

private:
	Halley::String glyph;
	Halley::String reading;
};

typedef std::vector<GlyphReading> GlyphGroup;
