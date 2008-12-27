#include "halley/halley.h"

class FrameBackground : public Halley::Frame {
private:
	virtual void DoUpdate(float time);
	virtual void DoRender();

public:
	FrameBackground();
};
