#include "roomaji_hepburn.h"
using namespace Japanese;

RoomajiHepburn::RoomajiHepburn(int _type)
{
	type = _type;
	if (type < 0 || type > 2) throw std::exception("Invalid Hepburn roomaji type. Must be between 0 and 2 (inclusive).");

	wxString entries[] = {
		L"la" , L"a"  , L"li" , L"i"  , L"lu" , L"u"  , L"le" , L"e"  , L"lo" , L"o"  , // U+3041 - U+304A
		L"ka" , L"ga" , L"ki" , L"gi" , L"ku" , L"gu" , L"ke" , L"ge" , L"ko" , L"go" , // U+304B - U+3054
		L"sa" , L"za" , L"shi", L"ji" , L"su" , L"zu" , L"se" , L"ze" , L"so" , L"zo" , // U+3055 - U+305E
		L"ta" , L"da" , L"chi", L"ji" , L"ltsu", L"tsu", L"zu" , L"te" , L"de" , L"to" , L"do" , // U+305F - U+3069
		L"na" , L"ni" , L"nu" , L"ne" , L"no" ,											// U+306A - U+306E
		L"ha" , L"ba" , L"pa" , L"hi" , L"bi" , L"pi" , L"fu" , L"bu" , L"pu" , L"he" , L"be" , L"pe" , L"ho" , L"bo" , L"po" ,	// U+306F - U+307D
		L"ma" , L"mi" , L"mu" , L"me" , L"mo" ,
		L"lya", L"ya" , L"lyu", L"yu" , L"lyo", L"yo" ,
		L"ra" , L"ri" , L"ru" , L"re" , L"ro" ,
		L"lwa", L"wa" , L"wi" , L"we" , L"wo" , L"n"  , L"vu" ,
		L"lka", L"lke", L"va" , L"vi" , L"ve" , L"vo" , L"."  , L"-"
	};

	SetKanaNames(entries,sizeof(entries)/sizeof(wxString));

	AddGlide(L"ky",0x30ad,L"auo");
	AddGlide(L"gy",0x30ae,L"auo");
	AddGlide(L"sh",0x30b7,L"auo");
	AddGlide(L"j" ,0x30b8,L"auo");
	AddGlide(L"j" ,0x30c2,L"auo");
	AddGlide(L"ch",0x30c1,L"auo");
	AddGlide(L"ny",0x30cb,L"auo");
	AddGlide(L"hy",0x30d2,L"auo");
	AddGlide(L"by",0x30d3,L"auo");
	AddGlide(L"py",0x30d4,L"auo");
	AddGlide(L"my",0x30df,L"auo");
	AddGlide(L"ry",0x30ea,L"auo");

	AddGlide(L"y" ,0x30a4,L"E");
	AddGlide(L"w" ,0x30a6,L"IEO");
	AddGlide(L"f" ,0x30d5,L"AIEO");
	AddGlide(L"fy",0x30d5,L"u");
	AddGlide(L"vy",0x30f4,L"auo");
	AddGlide(L"v" ,0x30f4,L"AIEO");

	AddGlide(L"sh",0x30b7,L"E");
	AddGlide(L"j" ,0x30b8,L"E");
	AddGlide(L"ch",0x30c1,L"E");
	AddGlide(L"ts",0x30c4,L"AIEO");

	AddGlide(L"t" ,0x30c6,L"IU");
	AddGlide(L"ty",0x30c6,L"u");
	AddGlide(L"d" ,0x30c7,L"IU");
	AddGlide(L"dy",0x30c7,L"u");
}

void RoomajiHepburn::AdjustSyllableForPrevious(String &current,const String &previous) const
{
	wxChar lastChar = previous[previous.Length()-1];
	if (current == _T("e") && lastChar == _T('e')) {
		current = _T("i");
	}
}

bool RoomajiHepburn::AdjustSyllableForNext(String &current,const String &next) const
{
	wxChar nextChar = next[0];

	// Check if it's the little tsu
	if (current == _T("ltsu")) {
		if (next.StartsWith(_T("ch"))) current = _T("t");
		else current = nextChar;
	}
	
	// Not little tsu
	else {
		wxChar lastChar = current[current.Length()-1];
		if (lastChar == 'n') {
			if (nextChar == 'y' || nextChar == 'n' || IsVowel(nextChar)) {
				current += _T('\'');
			}
		} else {
			if (nextChar == '-') {
				AddMacrons(current);
				return false;
			}

			if (nextChar == lastChar || (lastChar == 'o' && nextChar == 'u')) {
				if (lastChar == 'a' || lastChar == 'o' || lastChar == 'u') {
					AddMacrons(current);
					return false;
				}
			}
		}
	}
	return true;
}
