#include "word_manager.h"
using namespace Japanese;

void WordManager::LoadFromJMDict(wxInputStream &file)
{
	wxXmlDocument document;
	document.Load(file);
	if (document.GetRoot()->GetName() == _T("JMdict")) {
		for (wxXmlNode* child = document.GetRoot()->GetChildren(); child; child = child->GetNext()) {
			if (child->GetName() == _T("entry")) {
				AddWord(child);
			}
		}
	}
}

void WordManager::AddWord(wxXmlNode* node)
{
	Word result(node);
	if (result.IsValid()) {
		const StringVector& strings = result.GetKanjiStrings();
		for (size_t i=0; i<strings.size(); i++) {
			String word = strings[i];
			words[word] = result;
			words[word].SetKanjiString(word);
			wordList.push_back(word);
		}
	}
}

size_t WordManager::GetCount() const
{
	return words.size();
}

const Word& WordManager::GetWord(String name) const
{
	std::map<String,Word>::const_iterator result = words.find(name);
	if (result != words.end()) {
		return result->second;
	}
	return nullWord;
}

const std::list<String>& WordManager::GetWordList() const
{
	return wordList;
}
