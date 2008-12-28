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

void drawSquare(float side)
{
	float s = side * 0.5f;
	glBegin(GL_QUADS);
	glVertex2f(-s,-s);
	glVertex2f(-s,s);
	glVertex2f(s,s);
	glVertex2f(s,-s);
	glEnd();
}

void drawStar(int tips, float rad1, float rad2)
{
	int n = tips*2;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0,0);
	for (int i=0; i <= n; i++) {
		float r = (i%2 == 0)? rad1 : rad2;
		float ang = (float)i / (float)n * 2.0f * (float)M_PI;
		glVertex2f(cos(ang)*r,sin(ang)*r);
	}
	glEnd();
}

void SpriteBgDecor::DoDraw()
{
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.00f, 0.76f, 0.54f, 1.0f);

	drawStar(5, 1.0f, 0.55f);	// 5-star
	//drawStar(4, 1, 0.707f);	// Square
	//drawStar(10, 1, 1);		// Circle
	//drawStar(3, 1.0f, 0.3f);	// 3-star
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
