
#pragma once

#include <fstream>
#include <map>
#include <boost/serialization/serialization.hpp>
#include "libstring.h"
#include "kanji.h"

namespace Japanese {

	class Word;
	
	class KanjiManager {
	public:
		KanjiManager();

		void LoadFromKanjidic(wxInputStream &file);		// Loads data from KANJIDIC
		void LoadFromKanjidic2(wxInputStream &file);	// Loads data from KANJIDIC2 (XML)
		void LoadFromKanjidic2(std::string filename);

		void SerializeFrom(std::ifstream &input);
		void SerializeTo(std::ofstream &output);

		const Kanji& GetKanji(String name) const;		// Gets a Kanji if it exists, nullKanji otherwise
		const Kanji& GetKanji(int unicodeValue) const;	// Gets a Kanji if it exists, nullKanji otherwise
		size_t GetCount() const;

		void AddKanji(const Kanji &kanji);				// Adds a kanji
		void AddWordReference(const Word &word);

		String NormalizeKanji(String kanji) const;

	private:
		Kanji& GetMutableKanji(String name);			// Gets a Kanji if it exists, nullKanji otherwise

		Kanji nullKanji;
		std::map<int,Kanji> kanji;

		int minCodePointBMP;
		int maxCodePointBMP;
		int minCodePointSIP;
		int maxCodePointSIP;
		size_t numSurrogate;

		// Boost serialization
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive &ar, const unsigned int version)
		{
			(void) version;
			ar & kanji;
			ar & minCodePointBMP;
			ar & maxCodePointBMP;
			ar & minCodePointSIP;
			ar & maxCodePointSIP;
			ar & numSurrogate;
		}
	};

}
