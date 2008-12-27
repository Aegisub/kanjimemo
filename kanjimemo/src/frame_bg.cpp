#include "halley/halley.h"
#include "frame_bg.h"

void FrameBackground::DoUpdate(float time)
{

}


void FrameBackground::DoRender()
{
	glClearColor(0.99f, 0.71f, 0.45f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	using namespace Halley;
	Vector2f size = Video::GetVirtualSize();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0,0,0,0.5f);
	glBegin(GL_QUADS);
		glVertex2f(0,0);
		glVertex2f(0,size.y);
		glVertex2f(size.x,size.y);
		glVertex2f(size.x,0);
	glEnd();
}

