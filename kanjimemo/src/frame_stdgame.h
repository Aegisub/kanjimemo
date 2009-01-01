#include "halley/halley.h"

class FrameStandardGame : public Halley::Frame {
private:
	void DoUpdate(float time);
	void DoRender();
	void Init();
	void DeInit();

public:
	FrameStandardGame();
};
