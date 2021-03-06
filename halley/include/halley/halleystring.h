/*********************************************************\
         __      __           __  __
        / /     / / _____    / / / / ______   __     __
       / /     / / /___  \  / / / / / ___  | / /    / /
      / /_____/ / _____| / / / / / / /__/ / / /    / /
     / ______  / / ___  / / / / / / _____/ / /    / /
    / /     / / / /__/ / / / / / / |____  / |____/ /
   /_/     /_/ |______/ /_/ /_/  \_____/  \_____  /
                                         ______/ /
            High Level Game Framework   /_______/

  ------------------------------------------------------

  Copyright (c) 2007 - Rodrigo Braz Monteiro.
  This file is subject to the terms of license.txt.

\*********************************************************/


#pragma once

#include <vector>
#include <string>
#include <boost/serialization/serialization.hpp>

#define WX_COMPAT
#ifdef WX_COMPAT
#include <wx/wxprec.h>
#endif

namespace Halley {

	typedef char Character;
	typedef std::basic_string<int> StringUTF32;

	// String class
	class String : public std::basic_string<Character> {
	public:

		String();
		String(const char* utf8);
		String(const char* utf8,size_t bytes);
		String(const basic_string<Character>& str);
		explicit String(const wchar_t* utf16);
		explicit String(const StringUTF32 &utf32);
		explicit String(char character);
		explicit String(wchar_t character);
		explicit String(int integer);
		explicit String(float number);
		explicit String(double number);

#ifdef WX_COMPAT
		explicit String(const wxString& wxstring);
		wxString GetWxString() const;
		String operator += (const wxString &p);
#endif

		bool IsEmpty() const;
		size_t Length() const;
		
		void SetSize(size_t size);
		void Truncate(size_t size);

		String& Trim(bool fromRight);
		String& TrimBoth();

		bool Contains(const String& string) const;
		size_t Find(Character c) const;
		size_t Find(String str) const;

		void Replace(String before, String after, bool all=true);
		void Shrink();

		String Left(size_t n) const;
		String Right(size_t n) const;
		String Mid(size_t start,size_t count=npos) const;

		bool StartsWith(const String& string,bool caseSensitive=true) const;
		bool EndsWith(const String& string,bool caseSensitive=true) const;

		void WriteText(const Character* src,size_t len,size_t &pos);
		void WriteChar(const Character &src,size_t &pos);
		void WriteNumber(Character *temp,int number,int pad,size_t &pos);

		bool IsNumber() const;

		//String Lower() const;
		//String Upper() const;
		//void MakeUpper();
		//void MakeLower();

		String AsciiLower() const;
		String AsciiUpper() const;
		void AsciiMakeUpper();
		void AsciiMakeLower();
		bool AsciiCompareNoCase(const Character *src) const;

		void AppendCharacter(int unicode);

		// Convert a string to an integer
		int ToInteger() const;
		int SubToInteger(size_t start,size_t end) const;

		//

		static const Character* StringPtrTrim(Character *chr,size_t len,size_t startPos);
		static const Character* StringTrim(String &str,size_t startPos);

		// Number to string functions
		static String PrettyFloat(String src);
		static String FloatToString(double value);
		static String FloatToString(float value);
		static String IntegerToString(int value, int base=10);
		static String PrettySize(int bytes);

		// Unicode routines
		StringUTF32 GetUTF32() const;

		// Static unicode routines
		static size_t GetUTF8Len(const wchar_t *utf16);
		static size_t GetUTF8Len(const StringUTF32 &utf32);

		//////////

		String operator += (const String &p);
		String operator += (const char* p);
		String operator += (const wchar_t* p);
		String operator += (const double &p);
		String operator += (const int &p);
		String operator += (const Character &p);


	private:
		Character* GetCharPointer(size_t pos);
		static size_t UTF8toUTF16(const char *utf8,wchar_t *utf16);
		static size_t UTF16toUTF8(const wchar_t *utf16,char *utf8);
		static size_t UTF32toUTF8(const int *utf32,char *utf8);

		// Boost serialization
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive &ar, const unsigned int version)
		{
			(void) version;
			std::basic_string<Character> &me = *this;
			ar & me;
		}
	};

	void operator <<(double &p1,String &p2);

	//typedef std::basic_string<wchar_t> String;
	typedef std::vector<String> StringArray;

}
