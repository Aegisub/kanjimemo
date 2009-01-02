#include "kanjimemo.h"
#include "frame_bg.h"
#include "halley/halley.h"
#include <wx/wfstream.h>
#include <wx/zipstrm.h>


///////////////
// Constructor
KanjiMemo::KanjiMemo()
: Halley::Game("KanjiMemo")
{
}


//////////////
// Initialize
void KanjiMemo::Init()
{
	using namespace Halley;
	Video::SetVideo(false,Vector2i(1280,720),Vector2f(1600,1000));
	//Video::SetVideo(true,Vector2i(1680,1050),Vector2f(1600,1000));
	//Video::SetVideo(false,Vector2i(1600,1000));
	//Video::SetVideo(false,Vector2i(1680/4,1050/4),Vector2f(1680,1050));
	SetTopFrame(spFrame(new FrameBackground));

	
	if (true) {
		wxFileInputStream input(_T("kanjidic2.zip"));
		wxBufferedInputStream buffer(input);
		wxZipInputStream zip(buffer);
		wxZipEntry *entry = zip.GetNextEntry();
		if (entry) {
			kanji.LoadFromKanjidic2(zip);
			delete entry;
		}
		zip.CloseEntry();
	}

	if (true) {
		wxFileInputStream input(_T("jmdict.zip"));
		wxBufferedInputStream buffer(input);
		wxZipInputStream zip(buffer);
		wxZipEntry *entry = zip.GetNextEntry();
		if (entry) {
			words.LoadFromJMDict(zip);
			delete entry;
		}
		zip.CloseEntry();
	}
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
#ifdef _MSC_VER
#ifdef _DEBUG
	#pragma comment(lib,"SDL_d.lib")
	#pragma comment(lib,"SDLmain_d.lib")
#else
	#pragma comment(lib,"SDL.lib")
	#pragma comment(lib,"SDLmain.lib")
#endif
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"rpcrt4.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"advapi32.lib")
#pragma comment(lib,"wsock32.lib")
#endif
