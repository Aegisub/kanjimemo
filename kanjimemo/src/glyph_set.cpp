#include "glyph_set.h"
using namespace Halley;


GlyphSet::GlyphSet(String _name)
: name(_name)
{

}

String GlyphSet::GetName() const
{
	return name;
}

size_t GlyphSet::GetNumberGroups() const
{
	return groups.size();
}

const StringArray& GlyphSet::GetGroup(size_t n) const
{
	return groups[n];
}
