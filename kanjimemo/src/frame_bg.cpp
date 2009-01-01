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

	if (false) {
		float w = Halley::Video::GetVirtualSize().x;
		float h = Halley::Video::GetVirtualSize().y;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0,0,0,0.1f);
		glBegin(GL_QUADS);
			glVertex2f(0,0);
			glVertex2f(0,h);
			glVertex2f(w,h);
			glVertex2f(w,0);
		glEnd();
	}

	sprites.Draw();

	if (true) {
		using namespace Halley;
		spTextDrawer text = TextDrawer::GetDrawer("Verdana", 25, true, false);
		text->SetColour(Colour(1,1,1));
		text->SetBorder(Colour(0.6f,0.5f,0.3f,1),5);
		text->Print("the quick brown fox jumps over the lazy dog!\n\nTHE QUICK BROWN FOX JUMPS OVER THE LAZY DOG!", Vector2f(20, 400));
		text->Print(String(L"Moonspeak: これは日本語！ ó_o UTF-16 surrogate: 𠀐"), Vector2f(20, 500));
		//text->Print(String(L"Moonspeak: これは日本語！"), Vector2f(20, 500));

		text = TextDrawer::GetDrawer("Harrington", 30, true, false);
		text->SetColour(Colour(1,1,1));
		text->SetBorder(Colour(0.6f,0.5f,0.3f,1),5);
		text->Print("Blah blah blah testing multiple fonts.", Vector2f(20, 200));
	}
}

