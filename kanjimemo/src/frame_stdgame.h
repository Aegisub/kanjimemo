#include "halley/halley.h"
#include "word_stream.h"

class FrameStandardGame : public Halley::Frame {
private:
	spWordStream words;

	Halley::spTextDrawer font;
	std::vector<Halley::spTextDrawer> kanjiFonts;
	Halley::spInputKeyboard kb;

	Halley::String curInput;

	Halley::String GetNormalizedInput();

protected:
	void DoUpdate(float time);
	void DoRender();
	void Init();
	void DeInit();

public:
	FrameStandardGame();
};
