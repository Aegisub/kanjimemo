#include "kana_converter.h"
#include "roomaji_manager.h"
using namespace Japanese;


///////////////////////////////////
// Constructors/destructors


KanaConverter::KanaConverter()
{
	roomaji = RoomajiManager::GetRoomaji();
}

KanaConverter::KanaConverter(Roomaji *_roomaji)
{
	roomaji = _roomaji;
}

KanaConverter::~KanaConverter()
{
}

/////////////////////////////////////
// Kana to roomaji


String KanaConverter::KanaToRoomaji(const String kana) const
{
	return roomaji->KatakanaToRoomaji(KanaToKatakana(kana));
}


/////////////////////////////////////
// Roomaji to Kana


String KanaConverter::RoomajiToHiragana(const String str) const
{
	return KatakanaToHiragana(RoomajiToKatakana(str));
}

String KanaConverter::RoomajiToKatakana(const String str) const
{
	return roomaji->RoomajiToKatakana(str);
}


/////////////////////////////////////////
// Convert between Hiragana and Katakana

const int HIRA_TO_KATA_OFFSET = 0x60;

String KanaConverter::HiraganaToKatakana(const String hiragana)
{
	String result = hiragana;
	OffsetCharacterValues(result,HIRA_TO_KATA_OFFSET);
	return result;
}

String KanaConverter::KatakanaToHiragana(const String katakana)
{
	String result = katakana;
	OffsetCharacterValues(result,-HIRA_TO_KATA_OFFSET);
	return result;
}

void KanaConverter::OffsetCharacterValues(String &str,int offset)
{
	size_t n = str.Length();
	for (size_t i=0; i<n; i++) {
		str[i] += offset;
	}
}

wxChar KanaConverter::KanaToHiragana(wxChar kana)
{
	if (IsKatakana(kana)) return kana - HIRA_TO_KATA_OFFSET;
	return kana;
}

wxChar KanaConverter::KanaToKatakana(wxChar kana)
{
	if (IsHiragana(kana)) return kana + HIRA_TO_KATA_OFFSET;
	return kana;
}

String KanaConverter::KanaToHiragana(const String str)
{
	String result = str;
	size_t n = result.Length();
	for (size_t i=0;i<n;i++) {
		if (IsKatakana(result[i])) {
			result[i] -= HIRA_TO_KATA_OFFSET;
		}
	}
	return result;
}

String KanaConverter::KanaToKatakana(const String str)
{
	String result = str;
	size_t n = result.Length();
	for (size_t i=0;i<n;i++) {
		if (IsHiragana(result[i])) {
			result[i] += HIRA_TO_KATA_OFFSET;
		}
	}
	return result;
}

StringVector KanaConverter::GetPronunciationChanges(const String kana)
{
	StringVector result;
	size_t len = kana.Length();
	if (len == 0) return result;

	wxChar firstChar = kana[0];
	wxChar lastChar = kana[len-1];
	wxChar firstCharKata = KanaToKatakana(firstChar);
	wxChar lastCharKata = KanaToKatakana(lastChar);

	// tsu -> little tsu
	if (lastCharKata == 0x30C4) {
		result.Add(kana.Left(len-1) + wxChar(lastChar-1));
	}

	// k->g, s->z, t->d
	if (firstCharKata >= 0x30AB && firstCharKata <= 0x30C1 && firstCharKata % 2 == 1) {
		result.Add(wxChar(firstChar+1) + kana.Mid(1));
	}

	// t->d
	if (firstCharKata >= 0x30C4 && firstCharKata <= 0x30C8 && firstCharKata % 2 == 0) {
		result.Add(wxChar(firstChar+1) + kana.Mid(1));
	}

	// h->b, h->p
	if (firstCharKata >= 0x30CF && firstCharKata <= 0x30DD && firstCharKata % 3 == 0) {
		result.Add(wxChar(firstChar+1) + kana.Mid(1));
		result.Add(wxChar(firstChar+2) + kana.Mid(1));
	}

	return result;
}

bool KanaConverter::IsHiragana(wxChar kana)
{
	return kana >= 0x3041 && kana <= 0x3094;
}

bool KanaConverter::IsKatakana(wxChar kana)
{
	return kana >= 0x30A1 && kana <= 0x30F6;
}

bool Japanese::KanaConverter::IsKana(wxChar kana)
{
	return IsHiragana(kana) || IsKatakana(kana);
}

bool KanaConverter::IsSmallKana(wxChar kana)
{
	if (IsHiragana(kana)) kana += HIRA_TO_KATA_OFFSET;
	if (kana >= 0x30A1 && kana <= 0x30A9 && (kana % 2 == 1)) return true;
	if (kana >= 0x30E3 && kana <= 0x30E7 && (kana % 2 == 1)) return true;
	return false;
}
