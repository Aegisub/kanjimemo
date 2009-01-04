#include "roomaji.h"
using namespace Japanese;

const int KATAKANA_START = 0x30A1;

Roomaji::Roomaji()
{
	entries[String(0x2015)] = "-";
	entries[String(0x2212)] = "-";
	entries[String(0x3000)] = " ";
	entries[String(0x3001)] = ", ";
	entries[String(0x301C)] = "~";
}

void Roomaji::SetKanaNames(String* names,size_t count)
{
	for (size_t i=0;i<count;i++) {
		String katakana = String((int)(i+KATAKANA_START));
		entries[katakana] = names[i];
	}
}

void Roomaji::AddGlide(String roomajiPrefix,int katakanaPrefix,String vowels)
{
	size_t n = vowels.Length();
	for (size_t i=0;i<n;i++) {
		int vowel = vowels[i];
		int kataVowel = 0;
		switch (vowel) {
			case 'a': kataVowel = 0x30E3; break;
			case 'u': kataVowel = 0x30E5; break;
			case 'o': kataVowel = 0x30E7; break;
			case 'A': kataVowel = 0x30A1; break;
			case 'I': kataVowel = 0x30A3; break;
			case 'U': kataVowel = 0x30A5; break;
			case 'E': kataVowel = 0x30A7; break;
			case 'O': kataVowel = 0x30A9; break;
		}

		String entry(katakanaPrefix);
		entry += String(kataVowel);
		entries[entry] = roomajiPrefix + String(vowel).AsciiLower();
	}
}

String Roomaji::GetKanaString() const
{
	String result;
	std::map<String,String>::const_iterator iter;
	for (iter = entries.begin(); iter != entries.end(); iter++) {
		result += iter->first + "=" + iter->second + ", ";
	}
	return result.Left(result.length()-2);
}


/////////////////////////////////////////
// Convert katakana to roomaji

String Roomaji::KatakanaToRoomaji(const String _katakana) const
{
	const StringUTF32 katakana = _katakana.GetUTF32();

	// Step 1: convert into an array of roomaji syllables
	// This step does not take context into account.
	size_t len = katakana.size();
	size_t nSyl = 0;
	std::vector<String> syllable(len);
	for (size_t i=0;i<len;i++) {
		// Try to grab two characters
		String syl = GetRoomaji(String(katakana.substr(i,2)));
		if (syl.IsEmpty()) {
			// Failed, try just one
			String curKata = String(katakana.substr(i,1));
			syl = GetRoomaji(curKata);
			if (syl.IsEmpty()) {
				// Failed again, abort
				int code = curKata.GetUTF32()[0];
				String codeStr = String::IntegerToString(code,16).AsciiUpper();
				codeStr = "U+" + std::string(5 - codeStr.length(),'0') + codeStr;
				String msg = "Could not convert katakana \"" + curKata + "\" (" + codeStr + ") to roomaji.";
				throw std::exception(msg.c_str());
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
	StringUTF32 str2 = str.GetUTF32();
	if (str2.length() == 1) {
		int firstChar = str2[0];
		if (IsFullWidthRoman(firstChar)) {
			// Convert to normal width
			int result = firstChar - 0xFF00 + 0x20;
			if (result >= 0x61 && result <= 0x7A) result -= 0x20;
			return String(result);
		}
	}

	// Search for entry on the table
	std::map<String,String>::const_iterator result = entries.find(str);
	if (result != entries.end()) {
		return result->second;
	}
	return "";
}

bool Roomaji::IsVowel(int letter) const
{
	return letter == 'a' || letter == 'e' || letter == 'i' || letter == 'o' || letter == 'u';
}

bool Roomaji::IsFullWidthRoman(int letter) const
{
	return letter >= 0xFF00 && letter <= 0xFF5E;
}

void Roomaji::Macronize(String &src)
{
	StringUTF32 source = src.GetUTF32();
	size_t len = source.size();
	for (size_t i=0; i<len; i++) {
		int macron = 0;
		switch(source[i]) {
			case L'a': macron = 257; break;
			case L'e': macron = 275; break;
			case L'i': macron = 299; break;
			case L'o': macron = 333; break;
			case L'u': macron = 363; break;
		}
		if (macron != 0) source[i] = macron;
	}
	src = String(source);
}


//////////////////////////////////////////
// Convert roomaji to katakana

String Roomaji::RoomajiToKatakana(const String roomaji) const
{
	if (true) throw std::exception("Not implemented");
	return "";
}
