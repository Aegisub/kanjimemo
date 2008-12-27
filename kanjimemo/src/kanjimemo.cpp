#include "kanjimemo.h"
#include "frame_bg.h"
#include "halley/halley.h"


//////////////
// Initialize
void KanjiMemo::Init()
{
	using namespace Halley;
	Video::SetVideo(false,Vector2i(1280,700),Vector2f(1600,1000));
	SetTopFrame(spFrame(new FrameBackground));
}


////////////////
// Deinitialize
void KanjiMemo::DeInit()
{

}


///////////////////
// Initialize game
HalleyGame(KanjiMemo);


//////////////////
// Load libraries
#ifdef _DEBUG
	#pragma comment(lib,"SDL_d.lib")
	#pragma comment(lib,"SDLmain_d.lib")
#else
	#pragma comment(lib,"SDL.lib")
	#pragma comment(lib,"SDLmain.lib")
#endif
#pragma comment(lib,"OpenGL32.lib")
