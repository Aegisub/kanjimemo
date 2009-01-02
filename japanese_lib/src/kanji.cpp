#include "kanji.h"
#include "kana_converter.h"

using namespace Japanese;


//wxRegEx Kanji::regExp;


Kanji::Kanji()
{
	value = 0;
}

Kanji::Kanji(String kanjidicLine)
{
	ParseKanjidic(kanjidicLine);
}

Kanji::Kanji(wxXmlNode* node)
{
	ParseKanjidic(node);
}

bool Kanji::IsValid() const
{
	return value != 0;
}

unsigned int Kanji::GetValue() const
{
	return value;
}

unsigned int Kanji::GetJLPT() const
{
	return jlpt;
}


////////////////////////////////////////
// Plain KANJIDIC parsing

void Kanji::ParseKanjidic(String kanjidicLine)
{
	// !!THIS CODE DOES NOT WORK!!
/*
	if (!regExp.IsValid()) {
		wxString expression =
			wxString(_T("^([^\\s]*)"))
			+ wxString(_T("[^U]*[U]+([^\\s]+)"))
			+ wxString(_T("[^G]*[G]+([^\\s]+)"))
			+ wxString(_T("[^F]*[F]+([^\\s]+)"))
			+ wxString(_T("(?:[[:alnum:][:punct:]\\s]*([^[:alnum:][:punct:]\\s]+)[\\s]*)*"))
			+ wxString(_T("[^\\{]*(?:[\\{]([^\\}]*)[\\}][^\\{]*)*"))
			;
		bool result = regExp.Compile(expression,wxRE_ADVANCED);
		if (!result) throw 0;
	}

	if (regExp.Matches(kanjidicLine)) {
		String matches = _T("");
		int n = regExp.GetMatchCount();
		for (int i=1;i<n;i++) {
			matches += wxString(_T("("))+regExp.GetMatch(kanjidicLine,i)+_T(") ");
		}
		wxLogMessage(wxString::Format(_T("Got %i matches: "),n)+matches);
	}	
	*/
}



////////////////////////////////////////
// XML parsing for KANJIDIC2


void Kanji::ParseKanjidic(wxXmlNode* node)
{
	wxXmlNode* child = node->GetChildren();
	wxXmlNode* child2 = NULL;
	while (child) {
		wxString name = child->GetName();
		if (name == L"codepoint") XMLParseCodepoint(child);
		if (name == L"misc") XMLParseMisc(child);
		if (name == L"reading_meaning") {
			child2 = child->GetChildren();
			while (child2) {
				if (child2->GetName() == L"rmgroup") XMLParseRMGroup(child2);
				child2 = child2->GetNext();
			}
		}
		child = child->GetNext();
	}

	//wxLogMessage(wxString::Format(_T("Got %c with %i on'yomi, %i kun'yomi, grade %i, jlpt %i, frequency %i, %i strokes"),value,onYomi.size(),kunYomi.size(),grade,jlpt,rankOrderFrequency,strokeCount));
}

void Kanji::XMLParseCodepoint(wxXmlNode* node)
{
	wxXmlNode* child = node->GetChildren();
	while (child) {
		if (child->GetName() == _T("cp_value")) {
			if (child->GetPropVal(_T("cp_type"),_T("")) == _T("ucs")) {
				wxString content = child->GetNodeContent();
				wxLongLong_t tmp;
				content.ToLongLong(&tmp,16);
				value = tmp;
			}
		}
		child = child->GetNext();
	}
}

int StringToInt(wxString str) {
	long tmp;
	str.ToLong(&tmp);
	return tmp;
}

void Kanji::XMLParseMisc(wxXmlNode* node)
{
	wxXmlNode* child = node->GetChildren();
	while (child) {
		wxString name = child->GetName();
		if (name == _T("grade")) grade = StringToInt(child->GetNodeContent());
		if (name == _T("stroke_count")) strokeCount = StringToInt(child->GetNodeContent());
		if (name == _T("jlpt")) jlpt = StringToInt(child->GetNodeContent());
		if (name == _T("freq")) rankOrderFrequency = StringToInt(child->GetNodeContent());
		child = child->GetNext();
	}
}

void Kanji::XMLParseRMGroup(wxXmlNode* node)
{
	for (wxXmlNode* child = node->GetChildren(); child; child = child->GetNext()) {
		wxString name = child->GetName();
		if (name == _T("reading")) {
			wxString r_type = child->GetPropVal(_T("r_type"),_T(""));

			// Parse on'yomi reading
			if (r_type == _T("ja_on")) {
				wxString content = child->GetNodeContent();
				onYomi.push_back(String(content));
			}

			// Parse kun'yomi reading
			if (r_type == _T("ja_kun")) {
				wxString content = child->GetNodeContent();
				kunYomi.push_back(String(content));
			}
		}

		// Parse English meaning
		if (name == _T("meaning")) {
			wxString m_lang = child->GetPropVal(_T("m_lang"),_T("eng"));
			if (m_lang == _T("eng")) {
				wxString content = child->GetNodeContent();
				meanings.push_back(String(content));
			}
		}
	}
}

String Kanji::GetName() const
{
	return String((UnicodeCharacter)value);
}


/////////////////


const StringVector& Kanji::GetOnYomi() const
{
	return onYomi;
}

const StringVector& Kanji::GetKunYomi() const
{
	return kunYomi;
}

StringVector Kanji::GetReadings() const
{
	StringVector result;

	// Get on'yomi
	size_t n = onYomi.size();
	for (size_t i=0;i<n;i++) {
		result.push_back(onYomi[i]);
	}

	// Get kun'yomi
	n = kunYomi.size();
	for (size_t i=0;i<n;i++) {
		result.push_back(kunYomi[i]);
	}

	// Get variations of on'yomi
	n = onYomi.size();
	for (size_t i=0;i<n;i++) {
		StringVector variations = KanaConverter::GetPronunciationChanges(onYomi[i]);
		size_t m = variations.size();
		for (size_t j=0;j<m;j++) {
			result.push_back(variations[j]);
		}
	}

	// Get variations of kun'yomi
	n = kunYomi.size();
	for (size_t i=0;i<n;i++) {
		StringVector variations = KanaConverter::GetPronunciationChanges(kunYomi[i]);
		size_t m = variations.size();
		for (size_t j=0;j<m;j++) {
			result.push_back(variations[j]);
		}
	}

	return result;
}

void Japanese::Kanji::AddWordReference(String word,String reading,bool isRegular)
{
	(void) word;
	(void) reading;
	(void) isRegular;
}
