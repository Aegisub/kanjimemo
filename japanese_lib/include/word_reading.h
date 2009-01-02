#pragma once

#include "libstring.h"
#include <deque>

namespace Japanese {
	class KanjiManager;
	class Word;

	// Structure to store reading fragments
	struct WordReadingFragment {
		String text;
		String reading;
	};


	// Word reading parse exception class
	class WordReadingException : public std::exception {
	public:
		WordReadingException() : std::exception("Could not parse reading of word.") {}
	};


	// Word reading list
	typedef std::deque<WordReadingFragment> WordReadingList;


	// Word reading class
	class WordReading {
		friend class Word;

	public:
		String GetFullReading() const;
		WordReadingList GetReadings() const;
		bool IsRegular() const;

	private:
		WordReading();

		void PrependReading(String kanji,String reading);

		static WordReading Parse(String kanji,String katakana,const KanjiManager &kanjiManager);
		static bool ReadingMatches(String reading,String katakana,String restOfKanji);
		static String ClearReading(String reading);

		WordReadingList readings;
		int irregularity;
	};
}