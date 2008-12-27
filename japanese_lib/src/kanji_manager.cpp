#include <wx/txtstrm.h>
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
			AddKanji(Kanji(curLine));
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

String KanjiManager::NormalizeKanji(String kanji) const
{
	size_t len = kanji.Length();
	for (size_t i=1; i<len; i++) {
		wxChar curKanji = kanji[i];

		// Iteration mark
		if (curKanji == 0x3005) kanji[i] = kanji[i-1];
	}
	return kanji;
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
