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
	maxHistory = 0;

	for (size_t i=0; i<1+2*maxHistory; i++) {
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
	// Get kana reading
	String curWord = GetWord();
	String kana;
	if (kanaConverter.IsKana(curWord)) {
		kana = curWord;
	} else {
		Japanese::Word word = jwords.GetWord(curWord);
		kana = word.GetKanaString();
	}
	String reading = kanaConverter.KanaToRoomaji(kana);

	// Generate result
	WordResult res;
	res.success = reading == entry;
	res.correctReading = reading;

	// Update progress
	if (res.success) {
		progress->GotReadingCorrect(curWord);
	} else {
		progress->GotReadingWrong(curWord, entry);
	}

	return res;
}

String StandardWordStream::GetNewWord()
{
	using Halley::StringArray;
	using Halley::String;

	int nGroups = glyphs->GetNumberGroups();
	std::vector<std::pair<int, String> > glyphOdds;
	int totalOdds = 0;
	int curLevel = 0;
	
	// Build a list of all glyphs that are unlocked
	for (int i=0; i<nGroups; i++) {
		GlyphGroup group = glyphs->GetGroup(i);
		bool complete = true;

		// For each glyph, compute the odds that it'll show up
		for (size_t j=0; j<group.size(); j++) {
			float comp = progress->GetProgress(group[j]);
			if (comp < 0.65f) complete = false;
			int odds = ComputeOdds(comp);
			totalOdds += odds;
			glyphOdds.push_back(std::pair<int, String>(odds, group[j].GetGlyph()));
		}

		// If this group is not complete, don't add any further groups
		if (!complete) {
			curLevel = i;
			break;
		}
	}

	// Pick one
	int n = random.Get(0, totalOdds);
	int accum = 0;
	for (size_t i=0; i<glyphOdds.size(); i++) {
		accum += glyphOdds[i].first;
		if (accum >= n) return glyphOdds[i].second;
	}

	// Failed
	throw std::exception("ops.");
}

int StandardWordStream::ComputeOdds(float completion)
{
	return 20 - int(completion * 20);
}
