#include "halley/halley.h"
#include "japanese.h"

class KanjiMemo : public Halley::Game {
protected:
	void Init();
	void DeInit();
	Halley::spFrame CreateFrame(Halley::String frame);

public:
	Japanese::KanaConverter kanaConverter;
	Japanese::KanjiManager kanji;
	Japanese::WordManager words;

	KanjiMemo();
};

typedef Halley::shared_ptr<KanjiMemo> spKanjiMemo;
