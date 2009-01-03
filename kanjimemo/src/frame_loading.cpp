#include "frame_loading.h"
#include "frame_stdgame.h"
#include "kanjimemo.h"
#include "halley/halley.h"
#include <fstream>


FrameLoading::FrameLoading()
: Frame("loading")
{

}

void FrameLoading::DoUpdate(float time)
{
	if (load->IsDone()) {
		Switch(Halley::spFrame(new FrameStandardGame()));
	}
}

void FrameLoading::DoRender()
{
	using namespace Halley;
	spTextDrawer f = TextDrawer::GetDrawer("Verdana",50);
	f->Print("Loading...", Vector2f(800,500), Vector2f(0.5f, 0.5f));
	f->SetColour(Colour(1,1,1));
	f->SetBorder(Colour(),5);
}

void FrameLoading::Init()
{
	using namespace Halley;
	class LoadingThread : public Thread {
		shared_ptr<KanjiMemo> game;
	public:
		LoadingThread(shared_ptr<KanjiMemo> _game) : game(_game) {}
		void Run() {
			game->kanji.SerializeFrom(std::ifstream("kanji.txt"));
			game->words.SerializeFrom(std::ifstream("words.txt"));
		}
	};

	load = spThread(new LoadingThread(dynamic_pointer_cast<KanjiMemo>(Halley::Game::GetInstance())));
	load->Start();
}

void FrameLoading::DeInit()
{

}
