
#pragma once

#include <set>
#include "libstring.h"
#include "word.h"

namespace Japanese {

	class WordManager {
	public:
		void LoadFromJMDict(wxInputStream &file);		// Loads data from JMDict

		const Word& GetWord(String name) const;			// Gets a Word if it exists, nullWord otherwise
		size_t GetCount() const;
		const std::list<String>& GetWordList() const;

		void AddWord(wxXmlNode* node);					// Adds a Word given its JMDict XML node

	private:
		Word nullWord;
		std::map<String,Word> words;
		std::list<String> wordList;
	};

}