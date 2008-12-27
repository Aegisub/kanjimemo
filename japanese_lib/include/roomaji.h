#pragma once

#include "libstring.h"
#include <map>

namespace Japanese {
	class Roomaji {
	public:
		Roomaji();
		virtual ~Roomaji() {}

		String GetKanaString() const;
		String KatakanaToRoomaji(const String katakana) const;
		String RoomajiToKatakana(const String roomaji) const;

	protected:
		std::map<String,String> entries;

		void SetKanaNames(String* names,size_t count);
		void AddGlide(String roomajiPrefix,wxChar katakanaPrefix,String vowels);
		bool IsVowel(wxChar letter) const;
		bool IsFullWidthRoman(wxChar letter) const;
		void AddMacrons(String &source) const;

		virtual void AdjustSyllableForPrevious(String &current,const String &previous) const { (void) current; (void) previous; }
		// Return 'true' if you want to process the next syllable; 'false' to skip it
		virtual bool AdjustSyllableForNext(String &current,const String &next) const { (void) current; (void) next; return true; }

	private:
		String GetRoomaji(const String str) const;
	};
};
