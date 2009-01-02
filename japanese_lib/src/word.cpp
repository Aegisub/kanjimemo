#include "word.h"
#include "word_reading.h"
#include "kana_converter.h"
#include "kanji_manager.h"
using namespace Japanese;

Word::Word()
{

}

Word::Word(wxXmlNode* node)
{
	ParseJMDict(node);
}

bool Word::IsValid() const
{
	return (!kanjiString.IsEmpty());
}

String Word::GetKanjiString() const
{
	return kanjiString;
}

String Word::GetKanaString() const
{
	return kanaString;
}

void Word::ParseJMDict(wxXmlNode* jmdictLine)
{
	for (wxXmlNode* child = jmdictLine->GetChildren(); child; child = child->GetNext()) {
		wxString name = child->GetName();
		if (name == _T("k_ele")) XMLParseKEle(child);
		if (name == _T("r_ele") && kanaString.IsEmpty()) XMLParseREle(child);
		if (name == _T("sense")) XMLParseSense(child);
	}	
}

void Word::XMLParseKEle(wxXmlNode* node)
{
	for (wxXmlNode* child = node->GetChildren(); child; child = child->GetNext()) {
		wxString name = child->GetName();
		if (name == _T("keb")) {
			wxString temp = child->GetNodeContent();
			temp.Shrink();
			kanjiStrings.push_back(String(temp));
			if (kanjiStrings.size() == 1) kanjiString = String(temp);
		}
	}
}

void Word::XMLParseREle(wxXmlNode* node)
{
	for (wxXmlNode* child = node->GetChildren(); child; child = child->GetNext()) {
		wxString name = child->GetName();
		if (name == _T("reb")) {
			kanaString = String(child->GetNodeContent());
			kanaString.Shrink();
			if (kanjiString.IsEmpty()) {
				kanjiStrings.push_back(kanaString);
				kanjiString = kanaString;
			}
		}
	}
}

void Word::XMLParseSense(wxXmlNode* node)
{
	for (wxXmlNode* child = node->GetChildren(); child; child = child->GetNext()) {
		wxString name = child->GetName();
	}
}

WordReading Word::GetWordReading(const KanjiManager& kanjiManager) const
{
	if (IsValid()) {
		try {
			return WordReading::Parse(kanjiManager.NormalizeKanji(kanjiString).GetUTF32(),KanaConverter::KanaToKatakana(kanaString).GetUTF32(),kanjiManager);
		} catch (...) {
			return WordReading();
		}
	} else {
		return WordReading();
	}
}

const StringVector& Word::GetKanjiStrings() const
{
	return kanjiStrings;
}

void Word::SetKanjiString(String str)
{
	kanjiString = str;
}