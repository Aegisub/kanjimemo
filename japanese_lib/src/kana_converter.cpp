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

wxChar KanaConverter::KanaToHiragana(UnicodeCharacter kana)
{
	if (IsKatakana(kana)) return kana - HIRA_TO_KATA_OFFSET;
	return kana;
}

wxChar KanaConverter::KanaToKatakana(UnicodeCharacter kana)
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
	StringUTF32 result = str.GetUTF32();
	size_t n = result.size();
	for (size_t i=0;i<n;i++) {
		if (IsHiragana(result[i])) {
			result[i] += HIRA_TO_KATA_OFFSET;
		}
	}
	return String(result);
}

StringVector KanaConverter::GetPronunciationChanges(const String _kana)
{
	StringUTF32 kana = _kana.GetUTF32();
	StringVector result;
	size_t len = kana.length();
	if (len == 0) return result;

	int firstChar = kana[0];
	int lastChar = kana[len-1];
	int firstCharKata = KanaToKatakana(firstChar);
	int lastCharKata = KanaToKatakana(lastChar);

	// tsu -> little tsu
	if (lastCharKata == 0x30C4) {
		result.push_back(String(kana.substr(0,len-1) + StringUTF32(1, lastChar-1)));
	}

	// k->g, s->z, t->d
	if (firstCharKata >= 0x30AB && firstCharKata <= 0x30C1 && firstCharKata % 2 == 1) {
		result.push_back(String(StringUTF32(1, firstChar+1) + kana.substr(1)));
	}

	// t->d
	if (firstCharKata >= 0x30C4 && firstCharKata <= 0x30C8 && firstCharKata % 2 == 0) {
		result.push_back(String(StringUTF32(1, firstChar+1) + kana.substr(1)));
	}

	// h->b, h->p
	if (firstCharKata >= 0x30CF && firstCharKata <= 0x30DD && firstCharKata % 3 == 0) {
		result.push_back(String(StringUTF32(1, firstChar+1) + kana.substr(1)));
		result.push_back(String(StringUTF32(1, firstChar+2) + kana.substr(1)));
	}

	return result;
}

bool KanaConverter::IsHiragana(UnicodeCharacter kana)
{
	return kana >= 0x3041 && kana <= 0x3094;
}

bool KanaConverter::IsKatakana(UnicodeCharacter kana)
{
	return kana >= 0x30A1 && kana <= 0x30F6;
}

bool KanaConverter::IsKana(UnicodeCharacter kana)
{
	return IsHiragana(kana) || IsKatakana(kana);
}

bool KanaConverter::IsKana(const String kana)
{
	size_t len = kana.Length();
	for (size_t i=0; i<len; i++) {
		if (!IsKana(kana[i])) return false;
	}
	return true;
}

bool KanaConverter::IsSmallKana(UnicodeCharacter kana)
{
	if (IsHiragana(kana)) kana += HIRA_TO_KATA_OFFSET;
	if (kana >= 0x30A1 && kana <= 0x30A9 && (kana % 2 == 1)) return true;
	if (kana >= 0x30E3 && kana <= 0x30E7 && (kana % 2 == 1)) return true;
	return false;
}
