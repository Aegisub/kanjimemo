#include "word_stream_std.h"

using Halley::String;

StandardWordStream::StandardWordStream(Halley::shared_ptr<KanjiMemo> game)
: kanaConverter(game->kanaConverter),
  kanji(game->kanji),
  jwords(game->words)
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
	if (n < 0 || n >= (int)words.size()) return "";
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
	String curWord = GetWord();
	String kana;
	if (kanaConverter.IsKana(curWord)) {
		kana = curWord;
	} else {
		Japanese::Word word = jwords.GetWord(curWord);
		kana = word.GetKanaString();
	}
	String reading = kanaConverter.KanaToRoomaji(kana);

	WordResult res;
	res.success = reading == entry;
	res.correctReading = reading;
	return res;
}
