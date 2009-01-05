#pragma once
#include "halley/halley.h"
#include "glyph_reading.h"

class GlyphSet {
public:
	GlyphSet(Halley::String name);

	Halley::String GetName() const;
	Halley::String GetDisplayName() const;

	size_t GetNumberGroups() const;
	const GlyphGroup& GetGroup(size_t n) const;

private:
	Halley::String name;
	Halley::String displayName;

	std::vector<GlyphGroup> groups;

	void AddGroup(Halley::String data);
	GlyphGroup GetReadings(Halley::String glyph);
};

typedef Halley::shared_ptr<GlyphSet> spGlyphSet;
