#include "word_stream_std.h"

using Halley::String;

StandardWordStream::StandardWordStream(spKanjiMemo game, spGlyphSet _glyphs, spPlayerProgress _progress)
: kanaConverter(game->kanaConverter),
  kanji(game->kanji),
  jwords(game->words),
  glyphs(_glyphs),
  progress(_progress)
{
	pos = 0;
	maxHistory = 2;

	for (size_t i=0; i<5; i++) {
		words.push_back(GetNewWord());
	}
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
		words.push_back(GetNewWord());

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

String StandardWordStream::GetNewWord()
{
	int nGroups = glyphs->GetNumberGroups();
	Halley::StringArray group = glyphs->GetGroup(random.Get(0, nGroups));
	return group[random.Get(0, (int) group.size())];
}
