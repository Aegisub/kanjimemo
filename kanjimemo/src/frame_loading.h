#include "halley/halley.h"

class FrameLoading : public Halley::Frame {
private:
	Halley::spThread load;

	void DoUpdate(float time);
	void DoRender();
	void Init();
	void DeInit();

public:
	FrameLoading();
};
