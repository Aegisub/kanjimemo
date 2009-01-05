
#pragma once

#include "libstring.h"
#include <wx/xml/xml.h>
#include <boost/serialization/serialization.hpp>

namespace Japanese {
	class KanjiManager;
	class WordReading;

	class Word {
	public:
		Word();								// Constructs an invalid word
		Word(wxXmlNode* node);				// Calls ParseJMDict()
		Word(String kanji, String kana);

		void ParseJMDict(wxXmlNode* node);	// Parse a line from JMDict
		bool IsValid() const;			// Checks if this is a valid word

		void SetKanjiString(String str);
		String GetKanjiString() const;
		const StringVector& GetKanjiStrings() const;
		String GetKanaString() const;
		WordReading GetWordReading(const KanjiManager& kanjiManager) const;

		bool operator<(const Word& param) const { return GetKanjiString() < param.GetKanjiString(); }

	private:
		String kanjiString;
		StringVector kanjiStrings;
		String kanaString;
		StringVector meanings;

		void XMLParseKEle(wxXmlNode* node);
		void XMLParseREle(wxXmlNode* node);
		void XMLParseSense(wxXmlNode* node);

		// Boost serialization
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive &ar, const unsigned int version)
		{
			(void) version;
			ar & kanjiString;
			ar & kanjiStrings;
			ar & kanaString;
			ar & meanings;
		}
	};
}
