
#pragma once

#include <fstream>
#include <set>
#include "libstring.h"
#include "word.h"

#include <boost/serialization/serialization.hpp>

namespace Japanese {

	class WordManager {
	public:
		void LoadFromJMDict(wxInputStream &file);		// Loads data from JMDict
		void LoadFromJMDict(std::string filename);

		void SerializeFrom(std::ifstream &input);
		void SerializeTo(std::ofstream &output);

		const Word& GetWord(String name) const;			// Gets a Word if it exists, nullWord otherwise
		size_t GetCount() const;
		const std::list<String>& GetWordList() const;

		void AddWord(wxXmlNode* node);					// Adds a Word given its JMDict XML node

	private:
		Word nullWord;
		std::map<String,Word> words;
		std::list<String> wordList;

		// Boost serialization
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive &ar, const unsigned int version)
		{
			(void) version;
			ar & words;
			ar & wordList;
		}
	};

}