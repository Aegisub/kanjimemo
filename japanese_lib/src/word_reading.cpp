#include "word_reading.h"
#include "kanji_manager.h"
#include "kana_converter.h"
using namespace Japanese;



WordReading::WordReading()
{
	irregularity = 0;
}

bool WordReading::IsRegular() const
{
	return (irregularity == 0);
}

String WordReading::GetFullReading() const
{
	String result;
	if (!IsRegular()) result = String(wxString::Format(_T("[IRREGULAR - %i]  "),irregularity));
	size_t n = readings.size();
	for (size_t i=0;i<n;i++) {
		if (i > 0) result += _T(",  ");
		result += readings[i].text + " = " + readings[i].reading;
	}
	return result;
}

WordReadingList WordReading::GetReadings() const
{
	return readings;
}

WordReading WordReading::Parse(StringUTF32 kanjiString,StringUTF32 katakana,const KanjiManager &kanjiManager)
{
	// Stop condition
	if (kanjiString.size() == 0) {
		if (katakana.size() == 0) return WordReading();
		else throw WordReadingException();
	}
	if (katakana.size() == 0) throw WordReadingException();

	// Get kanji if it is one
	Kanji kanji = kanjiManager.GetKanji(kanjiString[0]);

	// Fill readings
	size_t regularReadings = 0;
	StringVector readings;
	size_t kanjiLen = 1;

	// Check if this kanji is actually known
	if (kanji.IsValid()) {
		readings = kanji.GetReadings();
		regularReadings = readings.size();

		// As a fallback, add all possible katakana readings for this kanji
		int kataLen = signed(katakana.size()) + 1 - signed(kanjiString.size());
		if (kataLen < 0) throw std::exception("wtf?");
		for (int i=1;i<=kataLen;i++) {
			// Make sure that the remaining katakana won't start with a small kana
			if (i == kataLen || !KanaConverter::IsSmallKana(katakana[i])) {
				readings.push_back(String(katakana.substr(0,i)));
			}
		}
	} else {
		// This probably means that the "kanji" is actually a kana.
		if (kanjiString.length() > 0 && KanaConverter::IsSmallKana(kanjiString[1])) {
			// If it's a glide, take TWO characters
			readings.push_back(String(kanjiString.substr(0,2)));
			kanjiLen = 2;
		} else {
			readings.push_back(String(kanjiString[0]));
		}
		regularReadings = 1;
	}

	// Irregular reading
	boolean irregularFound = false;
	WordReading irregular;
	irregular.irregularity = 9999999;

	// Loop through each reading
	size_t n = readings.size();
	StringUTF32 restOfKanji = kanjiString.substr(kanjiLen);
	for (size_t i=0;i<n;i++) {
		if (ReadingMatches(readings[i],String(katakana),String(restOfKanji))) {
			// Strips the reading of okurigana and prefix/suffix marks
			String clearReading = ClearReading(readings[i]);

			// Try to get a whole reading starting with this particular reading
			// If it fails, it'll throw an exception and discard this whole sub-tree
			try {
				WordReading tryReading = Parse(kanjiString.substr(kanjiLen),katakana.substr(clearReading.GetUTF32().size()),kanjiManager);

				// Getting here means that the word parsed correctly from this point to the end.
				// Prepend the current meaning to the result
				tryReading.PrependReading(String(kanjiString.substr(0,kanjiLen)),clearReading);

				// If the current reading exceeds the number of regular readings, flag this result as irregular
				if (i >= regularReadings) tryReading.irregularity++;

				// Don't merge this with the above if; "false" already be set
				if (tryReading.IsRegular()) {
					// Regular, so just return
					return tryReading;
				} else {
					// Irregular, store and keep looking
					irregularFound = true;
					if (tryReading.irregularity < irregular.irregularity) {
						irregular = tryReading;
					}
				}
			} catch (...) {}
		}
	}

	// Fallback to irregular
	if (irregularFound) return irregular;

	// No matches
	throw WordReadingException();
}

bool WordReading::ReadingMatches(String reading,String katakana,String restOfKanji)
{
	String kataReading = KanaConverter::KanaToKatakana(reading);

	// Check for exact match
	if (katakana.StartsWith(kataReading)) return true;

	// Strip out dashes
	kataReading.Replace("-","");

	// Split okurigana
	int pos = kataReading.Find(".");

	// Found okurigana
	if (pos != -1) {
		String okurigana = reading.Mid(pos+1);
		kataReading = kataReading.Left(pos);

		if (katakana.StartsWith(kataReading)) {
			// So far, it matches the kanji reading, but still need to check okurigana
			return restOfKanji.StartsWith(okurigana);
		}
		return false;
	}

	// Didn't find, just do a normal check
	else {
		return (katakana.StartsWith(kataReading));
	}
}

String WordReading::ClearReading(String reading)
{
	reading.Replace("-","",true);
	size_t pos = reading.Find(".");
	if (pos == std::string::npos) return reading;
	return reading.Left(pos);
}

void WordReading::PrependReading(String kanji,String reading)
{
	readings.push_front(WordReadingFragment());
	readings[0].text = kanji;
	readings[0].reading = reading;
}
