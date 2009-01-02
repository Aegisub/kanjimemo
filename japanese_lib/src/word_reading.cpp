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
	if (!IsRegular()) result = wxString::Format(_T("[IRREGULAR - %i]  "),irregularity);
	size_t n = readings.size();
	for (size_t i=0;i<n;i++) {
		if (i > 0) result += _T(",  ");
		result += readings[i].text + _T(" = ") + readings[i].reading;
	}
	return result;
}

WordReadingList WordReading::GetReadings() const
{
	return readings;
}

WordReading WordReading::Parse(String kanjiString,String katakana,const KanjiManager &kanjiManager)
{
	// Stop condition
	if (kanjiString.IsEmpty()) {
		if (katakana.IsEmpty()) return WordReading();
		else throw WordReadingException();
	}
	if (katakana.IsEmpty()) throw WordReadingException();

	// Get kanji if it is one
	Kanji kanji = kanjiManager.GetKanji(kanjiString[0]);

	// Fill readings
	size_t regularReadings = 0;
	StringVector readings;
	if (kanji.IsValid()) {
		readings = kanji.GetReadings();
		regularReadings = readings.size();

		// As a fallback, add all possible katakana readings for this kanji
		int kataLen = signed(katakana.Length()) + 1 - signed(kanjiString.Length());
		if (kataLen < 0) wxLogMessage(_T("LOLWTF?"));
		for (int i=1;i<=kataLen;i++) {
			// Make sure that the remaining katakana won't start with a small kana
			if (i == kataLen || !KanaConverter::IsSmallKana(katakana[i])) {
				readings.Add(katakana.Left(i));
			}
		}
	} else {
		readings.Add(kanjiString[0]);
		regularReadings = 1;
	}

	// Irregular reading
	boolean irregularFound = false;
	WordReading irregular;
	irregular.irregularity = 9999999;

	// Loop through each reading
	size_t n = readings.Count();
	String restOfKanji = kanjiString.Mid(1);
	for (size_t i=0;i<n;i++) {
		if (ReadingMatches(readings[i],katakana,restOfKanji)) {
			// Strips the reading of okurigana and prefix/suffix marks
			String clearReading = ClearReading(readings[i]);

			// Try to get a whole reading starting with this particular reading
			// If it fails, it'll throw an exception and discard this whole sub-tree
			try {
				WordReading tryReading = Parse(kanjiString.Mid(1),katakana.Mid(clearReading.Length()),kanjiManager);

				// Getting here means that the word parsed correctly from this point to the end.
				// Prepend the current meaning to the result
				tryReading.PrependReading(kanjiString[0],clearReading);

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
	kataReading.Replace(_T("-"),_T(""),true);

	// Split okurigana
	int pos = kataReading.Find(_T("."));

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
	reading.Replace(_T("-"),_T(""),true);
	int pos = reading.Find(_T("."));
	if (pos == -1) return reading;
	return reading.Left(pos);
}

void WordReading::PrependReading(String kanji,String reading)
{
	readings.push_front(WordReadingFragment());
	readings[0].text = kanji;
	readings[0].reading = reading;
}
