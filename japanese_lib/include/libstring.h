
#pragma once

//#include <wx/wx.h>
#include <vector>
#include "halley/halleystring.h"

namespace Japanese {
	//typedef wxString String;
	//typedef wxArrayString StringVector;
	using Halley::String;
	using Halley::StringUTF32;
	using Halley::Character;
	typedef std::vector<String> StringVector;
	typedef int UnicodeCharacter;
}
