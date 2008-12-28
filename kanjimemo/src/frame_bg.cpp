#include "halley/halley.h"
#include "frame_bg.h"
#include "sprite_bg_decor.h"


FrameBackground::FrameBackground()
: Frame("background")
{
}


void FrameBackground::Init()
{
	using namespace Halley;

	Vector2f p1 = Video::GetOrigin();
	Vector2f p2 = Video::GetDisplaySize()+p1;
	Vector2f border(100,100);
	p1 -= border;
	p2 += border;
	Rect4f area(p1, p2);
	Random r;

	for (int i=0;i<60;i++) {
		Vector2f pos(r.Get(p1.x,p2.x),r.Get(p1.y,p2.y));
		sprites.push_back(spSprite(new SpriteBgDecor(pos,area)));
	}
}


void FrameBackground::DeInit()
{

}


void FrameBackground::DoUpdate(float time)
{
	sprites.Update(time);
}


void FrameBackground::DoRender()
{
	// Clear background
	glClearColor(0.99f, 0.71f, 0.45f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	sprites.Draw();
}

