#include "glyph_set.h"
#include "kanjimemo.h"
#include <sstream>
using namespace Halley;


GlyphSet::GlyphSet(String _name)
: name(_name)
{
	String path = Game::GetProgramPath() + "sets/" + name + ".txt";
	StringArray file = TextReader::ReadFile(path);

	size_t len = file.size();
	if (len > 0) {
		displayName = file[0];

		for (size_t i=1; i<len; i++) {
			String cur = file[i];
			if (!cur.IsEmpty()) {
				AddGroup(cur);
			}
		}
	}
}

String GlyphSet::GetName() const
{
	return name;
}

String GlyphSet::GetDisplayName() const
{
	return displayName;
}

size_t GlyphSet::GetNumberGroups() const
{
	return groups.size();
}

const StringArray& GlyphSet::GetGroup(size_t n) const
{
	return groups[n];
}

void GlyphSet::AddGroup(String data)
{
	// Fix fullwidth spaces
	unsigned char fw[] = { 0xE3, 0x80, 0x80, 0 };
	data.Replace((char*) fw, " ");

	// Create stream and extract data from it
	std::stringstream ss(data);
	std::string tmp;
	StringArray group;

	while (ss >> tmp) {
		if (tmp.size() > 0) {
			group.push_back(tmp);
		}
	}

	if (group.size() > 0) {
		groups.push_back(group);
	}
}
