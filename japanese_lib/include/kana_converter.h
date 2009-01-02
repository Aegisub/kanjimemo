#pragma once

#include "libstring.h"

namespace Japanese {
	
	class Roomaji;

	class KanaConverter {
	public:
		KanaConverter();
		KanaConverter(Roomaji *roomaji);
		~KanaConverter();

		String KanaToRoomaji(const String kana) const;
		String RoomajiToKatakana(const String roomaji) const;
		String RoomajiToHiragana(const String roomaji) const;

		static wxChar KanaToHiragana(wxChar kana);
		static wxChar KanaToKatakana(wxChar kana);

		static String KanaToHiragana(const String kana);
		static String KanaToKatakana(const String kana);
		static String HiraganaToKatakana(const String hiragana);
		static String KatakanaToHiragana(const String katakana);
		static StringVector GetPronunciationChanges(const String kana);

		static bool IsHiragana(wxChar kana);
		static bool IsKatakana(wxChar kana);
		static bool IsKana(wxChar kana);
		static bool IsSmallKana(wxChar kana);

		static bool IsKana(const String kana);

	private:
		static void OffsetCharacterValues(String &str,int offset);

		Roomaji *roomaji;
	};
}