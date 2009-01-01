#include "halley/halley.h"

class FrameStandardGame : public Halley::Frame {
private:
	Halley::spTextDrawer font;
	std::vector<Halley::spTextDrawer> kanjiFonts;
	Halley::spInputKeyboard kb;

	Halley::String curInput;

protected:
	void DoUpdate(float time);
	void DoRender();
	void Init();
	void DeInit();

public:
	FrameStandardGame();
};
