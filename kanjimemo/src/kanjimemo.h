#include "halley/halley.h"
#include "japanese.h"

class KanjiMemo : public Halley::Game {
protected:
	void Init();
	void DeInit();

public:
	Japanese::KanaConverter kanaConverter;
	Japanese::KanjiManager kanji;
	Japanese::WordManager words;

	KanjiMemo();
};
