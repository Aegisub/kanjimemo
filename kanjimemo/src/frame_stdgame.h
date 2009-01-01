#include "halley/halley.h"

class FrameStandardGame : public Halley::Frame {
private:
	Halley::spTextDrawer font;

protected:
	void DoUpdate(float time);
	void DoRender();
	void Init();
	void DeInit();

public:
	FrameStandardGame();
};
