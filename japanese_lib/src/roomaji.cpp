#include "roomaji.h"
using namespace Japanese;

const int KATAKANA_START = 0x30A1;

Roomaji::Roomaji()
{
	entries[wxChar(0x2015)] = L"-";
	entries[wxChar(0x2212)] = L"-";
	entries[wxChar(0x3000)] = L" ";
	entries[wxChar(0x3001)] = L", ";
	entries[wxChar(0x301C)] = L"~";
}

void Roomaji::SetKanaNames(String* names,size_t count)
{
	for (size_t i=0;i<count;i++) {
		wxString katakana = wxChar(i+KATAKANA_START);
		entries[katakana] = names[i];
	}
}

void Roomaji::AddGlide(String roomajiPrefix,wxChar katakanaPrefix,String vowels)
{
	size_t n = vowels.Length();
	for (size_t i=0;i<n;i++) {
		wxChar vowel = vowels[i];
		wxChar kataVowel = 0;
		switch (vowel) {
			case L'a': kataVowel = 0x30E3; break;
			case L'u': kataVowel = 0x30E5; break;
			case L'o': kataVowel = 0x30E7; break;
			case L'A': kataVowel = 0x30A1; break;
			case L'I': kataVowel = 0x30A3; break;
			case L'U': kataVowel = 0x30A5; break;
			case L'E': kataVowel = 0x30A7; break;
			case L'O': kataVowel = 0x30A9; break;
		}

		wxString entry = katakanaPrefix;
		entry += kataVowel;
		entries[entry] = roomajiPrefix + wxString(vowel).Lower();
	}
}

String Roomaji::GetKanaString() const
{
	String result;
	std::map<String,String>::const_iterator iter;
	for (iter = entries.begin(); iter != entries.end(); iter++) {
		result += iter->first + _T("=") + iter->second + _T(", ");
	}
	return result.Left(result.Length()-2);
}


/////////////////////////////////////////
// Convert katakana to roomaji

String Roomaji::KatakanaToRoomaji(const String katakana) const
{
	// Step 1: convert into an array of roomaji syllables
	// This step does not take context into account.
	size_t len = katakana.Length();
	size_t nSyl = 0;
	std::vector<String> syllable(len);
	for (size_t i=0;i<len;i++) {
		// Try to grab two characters
		String syl = GetRoomaji(katakana.Mid(i,2));
		if (syl.IsEmpty()) {
			// Failed, try just one
			syl = GetRoomaji(katakana.Mid(i,1));
			if (syl.IsEmpty()) {
				// Failed again, abort
				wxString msg = _T("Could not convert katakana\"") + katakana.Mid(i,1) + _T("\" (") + wxString::Format(_T("U+%04X"),katakana[i]) + _T(") to roomaji.");
				throw std::exception(msg.mb_str(wxConvUTF8));
			}
		} else {
			i++;
		}
		syllable[nSyl++] = syl;
	}

	// Step 2: now that we have 'nSyl' syllables, use the context information to adjust them
	String final;
	for (size_t i=0;i<nSyl;i++) {
		// Analyze previous and syllables
		bool processNext = true;
		if (i > 0) {
			AdjustSyllableForPrevious(syllable[i],syllable[i-1]);
		}
		if (i < nSyl-1) {
			processNext = AdjustSyllableForNext(syllable[i],syllable[i+1]);
		}

		// Concatenate to ongoing final result
		final += syllable[i];

		// Skip next if requested
		if (!processNext) i++;
	}

	// Done
	return final;
}

String Roomaji::GetRoomaji(const String str) const
{
	// Check if it's fullwidth
	if (IsFullWidthRoman(str[0])) {
		if (str.Length() == 1) {
			// Convert to normal width
			wxChar result = str[0] - 0xFF00 + 0x20;
			if (result >= 0x61 && result <= 0x7A) result -= 0x20;
			return result;
		}
	}

	// Search for entry on the table
	std::map<String,String>::const_iterator result = entries.find(str);
	if (result != entries.end()) {
		return result->second;
	}
	return _T("");
}

bool Roomaji::IsVowel(wxChar letter) const
{
	return letter == 'a' || letter == 'e' || letter == 'i' || letter == 'o' || letter == 'u';
}

bool Roomaji::IsFullWidthRoman(wxChar letter) const
{
	return letter >= 0xFF00 && letter <= 0xFF5E;
}

void Roomaji::AddMacrons(String &source) const
{
	size_t len = source.Length();
	for (size_t i=0; i<len; i++) {
		wchar_t macron = 0;
		switch(source[i]) {
			case L'a': macron = 257; break;
			case L'e': macron = 275; break;
			case L'i': macron = 299; break;
			case L'o': macron = 333; break;
			case L'u': macron = 363; break;
		}
		if (macron != 0) source[i] = wxChar(macron);
	}	
}


//////////////////////////////////////////
// Convert roomaji to katakana

String Roomaji::RoomajiToKatakana(const String roomaji) const
{
	if (true) throw std::exception("Not implemented");
	return _T("");
}
