#include "halley/sprite.h"

class SpriteBgDecor : public Halley::Sprite {
private:
	float rotSpeed;
	Halley::Vector2f vel;
	Halley::Vector2f accel;
	Halley::Rect4f area;

	void SetAccel();

protected:
	void DoDraw();
	void Update(float time);

public:
	SpriteBgDecor(Halley::Vector2f pos, Halley::Rect4f area);
};
