#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include "kanji_manager.h"
#include "word_reading.h"
#include "word.h"

using namespace Japanese;



///////////////////////////////////
// Constructors


KanjiManager::KanjiManager()
{
	minCodePointBMP = 0x7FFFFFF;
	maxCodePointBMP = 0;
	minCodePointSIP = 0x7FFFFFF;
	maxCodePointSIP = 0;
	numSurrogate = 0;
}


//////////////////////////////////
// Load

void KanjiManager::LoadFromKanjidic(wxInputStream &file)
{
	wxCSConv conv(_T("EUC-JP"));
	wxTextInputStream text(file,_T(" \t"),conv);
	wxString curLine;
	while (!file.Eof()) {
		curLine = text.ReadLine();
		if (!curLine.IsEmpty()) {
			AddKanji(Kanji(String(curLine)));
		}
	}
}

void KanjiManager::LoadFromKanjidic2(wxInputStream &file)
{
	wxXmlDocument document;
	document.Load(file);
	if (document.GetRoot()->GetName() == _T("kanjidic2")) {
		wxXmlNode* child = document.GetRoot()->GetChildren();
		while (child) {
			if (child->GetName() == _T("character")) {
				AddKanji(Kanji(child));
			}
			child = child->GetNext();
		}
	}

	//wxLogMessage(wxString::Format(_T("Got %i BMP kanji from U+%04X to U+%04X, and %i SIP kanji from U+%04X to U+%04X"),kanji.size()-numSurrogate,minCodePointBMP,maxCodePointBMP,numSurrogate,minCodePointSIP,maxCodePointSIP));
}

void KanjiManager::LoadFromKanjidic2(std::string filename)
{
	wxFileInputStream input(wxString(filename.c_str(),wxConvUTF8));
	wxBufferedInputStream buffer(input);
	wxZipInputStream zip(buffer);
	wxZipEntry *entry = zip.GetNextEntry();
	if (entry) {
		LoadFromKanjidic2(zip);
		delete entry;
	}
	zip.CloseEntry();
}


////////////////////////////////////////
// Utilities


void KanjiManager::AddKanji(const Kanji &result)
{
	if (result.IsValid()) {
		int value = result.GetValue();
		kanji[value] = result;
		if (value > 0xFFFF) {
			numSurrogate++;
			if (value < minCodePointSIP) minCodePointSIP = value;
			if (value > maxCodePointSIP) maxCodePointSIP = value;
		}
		else {
			if (value < minCodePointBMP) minCodePointBMP = value;
			if (value > maxCodePointBMP) maxCodePointBMP = value;
		}
	}
}

size_t KanjiManager::GetCount() const
{
	return kanji.size();
}

const Kanji& KanjiManager::GetKanji(String name) const
{
	return GetKanji(name[0]);
}

const Kanji& KanjiManager::GetKanji(int unicodeValue) const
{
	std::map<int,Kanji>::const_iterator result = kanji.find(unicodeValue);
	if (result != kanji.end()) {
		return result->second;
	}
	return nullKanji;
}

Kanji& KanjiManager::GetMutableKanji(String name)
{
	std::map<int,Kanji>::iterator result = kanji.find(name[0]);
	if (result != kanji.end()) {
		return result->second;
	}
	return nullKanji;
}

String KanjiManager::NormalizeKanji(String _kanji) const
{
	StringUTF32 kanji = _kanji.GetUTF32();
	size_t len = kanji.length();
	for (size_t i=1; i<len; i++) {
		int curKanji = kanji[i];

		// Iteration mark
		if (curKanji == 0x3005) kanji[i] = kanji[i-1];
	}
	return String(kanji);
}

void KanjiManager::AddWordReference(const Word &word)
{
	WordReading reading = word.GetWordReading(*this);
	WordReadingList readings = reading.GetReadings();
	size_t len = readings.size();
	for (size_t i=0; i<len; i++) {
		WordReadingFragment frag = readings[i];
		Kanji &kanji = GetMutableKanji(frag.text);
		if (kanji.IsValid()) {
			kanji.AddWordReference(word.GetKanjiString(),frag.reading,reading.IsRegular());
		}
	}
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

void Japanese::KanjiManager::SerializeFrom(std::ifstream &input)
{
	boost::archive::text_iarchive ia(input);
	ia >> *this;
}

void Japanese::KanjiManager::SerializeTo(std::ofstream &output)
{
	boost::archive::text_oarchive oa(output);
	oa << *this;
}