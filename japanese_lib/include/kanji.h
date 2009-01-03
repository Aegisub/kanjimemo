
#pragma once

#include "libstring.h"
#include <wx/regex.h>
#include <wx/xml/xml.h>
#include <boost/serialization/serialization.hpp>

namespace Japanese {
	class Kanji {
	public:
		Kanji();									// Constructs an invalid kanji
		Kanji(String kanjidicLine);					// Calls ParseKanjidic()
		Kanji(wxXmlNode* kanjidic2xmlNode);			// Calls ParseKanjidic()
		void ParseKanjidic(String kanjidicLine);	// Parse a line from KANJIDIC
		void ParseKanjidic(wxXmlNode* kanjidicLine);// Parse a line from KANJIDIC2

		bool IsValid() const;					// Checks if this is a valid kanji

		unsigned int GetValue() const;				// [U] field on KANJIDIC
		unsigned int GetRankOrderFrequency() const;	// [F] field on KANJIDIC
		unsigned int GetGrade() const;				// [G] field on KANJIDIC
		unsigned int GetJLPT() const;				// Which level of JLPT this corresponds to
		String GetName() const;						// String representing the kanji

		StringVector GetReadings() const;			// Array with all readings of the kanji
		const StringVector& GetOnYomi() const;		// Array with on'yomi readings of the kanji
		const StringVector& GetKunYomi() const;		// Array with kun'yomi readings of the kanji
		const StringVector& GetMeanings() const;	// Array with English meanings of the kanji

		void AddWordReference(String word,String reading,bool isRegular);

		bool operator<(const Kanji& param) const { return value < param.value; }

	private:
		unsigned int value;
		unsigned int rankOrderFrequency;
		unsigned char grade;
		unsigned char jlpt;
		unsigned char strokeCount;
		StringVector onYomi;
		StringVector kunYomi;
		StringVector meanings;

		static wxRegEx regExp;

		void XMLParseCodepoint(wxXmlNode* node);
		void XMLParseMisc(wxXmlNode* node);
		void XMLParseRMGroup(wxXmlNode* node);


		// Boost serialization
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive &ar, const unsigned int version)
		{
			(void) version;
			ar & value;
			ar & rankOrderFrequency;
			ar & grade;
			ar & jlpt;
			ar & strokeCount;
			ar & onYomi;
			ar & kunYomi;
			ar & meanings;
		}
	};
}
