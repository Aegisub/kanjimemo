#pragma once
#include "halley/halley.h"

class GlyphSet {
public:
	GlyphSet(Halley::String name);

	Halley::String GetName() const;
	Halley::String GetDisplayName() const;

	size_t GetNumberGroups() const;
	const Halley::StringArray& GetGroup(size_t n) const;

private:
	Halley::String name;
	Halley::String displayName;

	std::vector<Halley::StringArray> groups;

	void AddGroup(Halley::String data);
};

typedef Halley::shared_ptr<GlyphSet> spGlyphSet;
