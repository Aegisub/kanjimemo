#include "roomaji_hepburn.h"
using namespace Japanese;

RoomajiHepburn::RoomajiHepburn(int _type)
{
	type = _type;
	if (type < 0 || type > 2) throw std::exception("Invalid Hepburn roomaji type. Must be between 0 and 2 (inclusive).");

	String entries[] = {
		"la" , "a"  , "li" , "i"  , "lu" , "u"  , "le" , "e"  , "lo" , "o"  , // U+3041 - U+304A
		"ka" , "ga" , "ki" , "gi" , "ku" , "gu" , "ke" , "ge" , "ko" , "go" , // U+304B - U+3054
		"sa" , "za" , "shi", "ji" , "su" , "zu" , "se" , "ze" , "so" , "zo" , // U+3055 - U+305E
		"ta" , "da" , "chi", "ji" , "ltsu", "tsu", "zu" , "te" , "de" , "to" , "do" , // U+305F - U+3069
		"na" , "ni" , "nu" , "ne" , "no" ,											// U+306A - U+306E
		"ha" , "ba" , "pa" , "hi" , "bi" , "pi" , "fu" , "bu" , "pu" , "he" , "be" , "pe" , "ho" , "bo" , "po" ,	// U+306F - U+307D
		"ma" , "mi" , "mu" , "me" , "mo" ,
		"lya", "ya" , "lyu", "yu" , "lyo", "yo" ,
		"ra" , "ri" , "ru" , "re" , "ro" ,
		"lwa", "wa" , "wi" , "we" , "wo" , "n"  , "vu" ,
		"lka", "lke", "va" , "vi" , "ve" , "vo" , "."  , "-"
	};

	SetKanaNames(entries,sizeof(entries)/sizeof(String));

	AddGlide("ky",0x30ad,"auo");
	AddGlide("gy",0x30ae,"auo");
	AddGlide("sh",0x30b7,"auo");
	AddGlide("j" ,0x30b8,"auo");
	AddGlide("j" ,0x30c2,"auo");
	AddGlide("ch",0x30c1,"auo");
	AddGlide("ny",0x30cb,"auo");
	AddGlide("hy",0x30d2,"auo");
	AddGlide("by",0x30d3,"auo");
	AddGlide("py",0x30d4,"auo");
	AddGlide("my",0x30df,"auo");
	AddGlide("ry",0x30ea,"auo");

	AddGlide("y" ,0x30a4,"E");
	AddGlide("w" ,0x30a6,"IEO");
	AddGlide("f" ,0x30d5,"AIEO");
	AddGlide("fy",0x30d5,"u");
	AddGlide("vy",0x30f4,"auo");
	AddGlide("v" ,0x30f4,"AIEO");

	AddGlide("sh",0x30b7,"E");
	AddGlide("j" ,0x30b8,"E");
	AddGlide("ch",0x30c1,"E");
	AddGlide("ts",0x30c4,"AIEO");

	AddGlide("t" ,0x30c6,"IU");
	AddGlide("ty",0x30c6,"u");
	AddGlide("d" ,0x30c7,"IU");
	AddGlide("dy",0x30c7,"u");
}

void RoomajiHepburn::AdjustSyllableForPrevious(String &current,const String &previous) const
{
	char lastChar = previous[previous.length()-1];
	if (current == "e" && lastChar == 'e') {
		current[0] = 'i';
	}
}

bool RoomajiHepburn::AdjustSyllableForNext(String &current,const String &next) const
{
	char nextChar = next[0];

	// Check if it's the little tsu
	if (current == "ltsu") {
		if (next.StartsWith("ch")) current = "t";
		else current = String(nextChar);
	}
	
	// Not little tsu
	else {
		int lastChar = current[current.Length()-1];
		if (lastChar == 'n') {
			if (nextChar == 'y' || nextChar == 'n' || IsVowel(nextChar)) {
				current += '\'';
			}
		} else {
			if (nextChar == '-') {
				Macronize(current);
				return false;
			}

			if (nextChar == lastChar || (lastChar == 'o' && nextChar == 'u')) {
				if (lastChar == 'a' || lastChar == 'o' || lastChar == 'u') {
					Macronize(current);
					return false;
				}
			}
		}
	}
	return true;
}
