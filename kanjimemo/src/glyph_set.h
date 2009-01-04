#pragma once
#include "halley/halley.h"

class GlyphSet {
public:
	GlyphSet(Halley::String name);

	Halley::String GetName() const;
	size_t GetNumberGroups() const;
	const Halley::StringArray& GetGroup(size_t n) const;

private:
	Halley::String name;
	std::vector<Halley::StringArray> groups;
};

typedef Halley::shared_ptr<GlyphSet> spGlyphSet;
