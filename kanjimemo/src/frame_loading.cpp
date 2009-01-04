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
	f->SetColour(Colour(1,1,1));
	f->SetBorder(Colour(),5);
	f->Print("Loading...", Vector2f(800,500), Vector2f(0.5f, 0.5f));
}

void FrameLoading::Init()
{
	using namespace Halley;
	class LoadingThread : public Thread {
		shared_ptr<KanjiMemo> game;
	public:
		LoadingThread(shared_ptr<KanjiMemo> _game) : game(_game) {}
		void Run() {
			game->kanji.SerializeFrom(std::ifstream((Game::GetProgramPath()+"data/kanji.txt").c_str()));
			game->words.SerializeFrom(std::ifstream((Game::GetProgramPath()+"data/words.txt").c_str()));
		}
	};

	load = spThread(new LoadingThread(dynamic_pointer_cast<KanjiMemo>(Halley::Game::GetInstance())));
	load->Start();
}

void FrameLoading::DeInit()
{

}
