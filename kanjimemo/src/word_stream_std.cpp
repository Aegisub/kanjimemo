#include "word_stream_std.h"

using Halley::String;

StandardWordStream::StandardWordStream()
{
	pos = 0;
	maxHistory = 2;

	words.push_back(String(L"日本語"));
	words.push_back(String(L"あ"));
	words.push_back(String(L"ア"));
	words.push_back(String(L"カタカナ"));
	words.push_back(String(L"ひらがな"));
}

String StandardWordStream::GetWord(int offset)
{
	int n = (int) pos + offset;
	if (n < 0 || n >= words.size()) return "";
	return words[n];
}

void StandardWordStream::Next()
{
	pos++;
	if (pos > maxHistory) {
		// TODO: remove this
		words.push_back(words.front());

		pos--;
		words.pop_front();
	}
}

WordResult StandardWordStream::CheckResult(String entry)
{
	String reading = "nihongo";
	WordResult res;
	res.success = reading == entry;
	res.correctReading = reading;
	return res;
}
