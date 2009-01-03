#include <wx/wfstream.h>
#include <wx/zipstrm.h>
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

void WordManager::LoadFromJMDict(std::string filename)
{
	wxFileInputStream input(wxString(filename.c_str(),wxConvUTF8));
	wxBufferedInputStream buffer(input);
	wxZipInputStream zip(buffer);
	wxZipEntry *entry = zip.GetNextEntry();
	if (entry) {
		LoadFromJMDict(zip);
		delete entry;
	}
	zip.CloseEntry();
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


///////////////////////////////////////////////////////////////////////////
// Serialization

#pragma warning(disable: 4099)
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>

void WordManager::SerializeFrom(std::ifstream &input)
{
	boost::archive::text_iarchive ia(input);
	ia >> *this;
}

void WordManager::SerializeTo(std::ofstream &output)
{
	boost::archive::text_oarchive oa(output);
	oa << *this;
}
