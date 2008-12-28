#include "halley/halley.h"
#include "sprite_bg_decor.h"
using namespace Halley;

SpriteBgDecor::SpriteBgDecor(Vector2f pos, Rect4f _area)
: area(_area)
{
	Random r;
	float size = r.Get(40.0f,60.0f);
	rotSpeed = -r.Get(30.f,60.f);

	SetPosition(pos);
	SetScale(Vector2f(size,size));
	SetPivot(GetScale() / 2);
	SetRotation(Angle1f::FromDegrees(r.Get(0.f,360.f)));
	SetAccel();
}

void SpriteBgDecor::DoDraw()
{
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.00f, 0.76f, 0.54f, 1.0f);

	GLPrimitive::DrawStar(5, 1.0f, 0.55f);	// 5-star
	//GLPrimitive::DrawStar(4, 1, 0.707f);	// Square
	//GLPrimitive::DrawStar(10, 1, 1);		// Circle
	//GLPrimitive::DrawStar(3, 1.0f, 0.3f);	// 3-star
}


void SpriteBgDecor::Update(float time)
{
	SetRotation(GetRotation() + Angle1f::FromDegrees(rotSpeed * time));

	Random r;
	if (r.Get(0,50) == 0) {
		SetAccel();
	}

	Vector2f pos = GetPosition();
	vel += accel * time;
	vel *= 0.995f;
	pos += vel * time;
	SetPosition(area.WrapInside(pos));
}


void SpriteBgDecor::SetAccel()
{
	Random r;
	accel = Vector2f(r.Get(0.f,1.f),-r.Get(0.f,1.f));
	accel *= 100;
}
