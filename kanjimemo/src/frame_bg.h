#include "halley/halley.h"

class FrameBackground : public Halley::Frame {
private:
	Halley::SpriteCollection sprites;

	void DoUpdate(float time);
	void DoRender();
	void Init();
	void DeInit();

public:
	FrameBackground();
};
