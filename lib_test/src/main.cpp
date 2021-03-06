#include <fstream>
#include <iostream>
#include "main.h"
#include "../../japanese_lib/include/japanese.h"

IMPLEMENT_APP(TestApp)
DEFINE_EVENT_TYPE(wxEVT_MY_EVENT)


/////////////////////////////


void LoadKanji(Japanese::KanjiManager &kanji)
{
	if (false) {
		kanji.LoadFromKanjidic2("E:/Projects/japanese_tools/kanjidic2.zip");
		kanji.SerializeTo(std::ofstream("kanji.txt"));
	}
	else {
		kanji.SerializeFrom(std::ifstream("kanji.txt"));
	}
}


void LoadWords(Japanese::WordManager &words)
{
	if (false) {
		words.LoadFromJMDict("E:/Projects/japanese_tools/jmdict.zip");
		words.SerializeTo(std::ofstream("words.txt"));
	}
	else {
		words.SerializeFrom(std::ifstream("words.txt"));
	}
}


wxThread::ExitCode WorkThread::Entry()
{
	using namespace Japanese;

	try {

		Print("Loading Kana converter... ");
		KanaConverter converter;
		Println("Done.");

		Print("Loading Kanjidic... ");
		KanjiManager kanji;
		LoadKanji(kanji);
		Println("Done.");
		Println(wxString::Format(_T("%i kanji were loaded."),kanji.GetCount()));

		Print("Loading JMDict... ");
		WordManager words;
		LoadWords(words);
		Println("Done.");
		Println(wxString::Format(_T("%i words were loaded."),words.GetCount()));

		wchar_t* wordList[] = { L"海上保安庁", L"案内", L"禁煙", L"群馬県", L"西班牙",
			                    L"インターネット", L"東京", L"母さん", L"牛乳", L"設置",
								L"今日は", L"乎古止点", L"森の奥", L"森", L"新しい",
								L"色々", L"人々", L"今日", L"承る", L"志", L"恥ずかしい",
								L"青空", L"完璧", L"日本", L"一回", L"ぐんにゃり" };
		int len = sizeof(wordList)/sizeof(wchar_t*);
		for (int i=0;i<len;i++) {
			Print(wxString(_T("\nPicking word: "))+wordList[i]);
			Word word = words.GetWord(String(wordList[i]));
			if (word.IsValid()) {
				wxString kanjiString = word.GetKanjiString().GetWxString();
				wxString kanaString = word.GetKanaString().GetWxString();
				wxString _katakana = converter.KanaToKatakana(word.GetKanaString()).GetWxString();
				wxString _roomaji = converter.KanaToRoomaji(word.GetKanaString()).GetWxString();
				Println(wxString(_T(" Done. "))+kanjiString+_T(" is read ")+kanaString+_T(" (")+_roomaji+_T(") or, in katakana, ")+_katakana);
				Println(_T("Reading: ") + word.GetWordReading(kanji).GetFullReading().GetWxString());
				for (size_t i=0;false && i<kanjiString.Length();i++) {
					Kanji curKanji = kanji.GetKanji(kanjiString[i]);
					Println(_T("Got kanji: ")+curKanji.GetName().GetWxString());
				}
			} else {
				Println(" Failed.");
			}
		}

		Print("\nProcessing all words... [");
		int count = 0;
		for (std::list<String>::const_iterator i = words.GetWordList().begin(); i != words.GetWordList().end(); i++) {
			String kanjiString = *i;
			Word word = words.GetWord(kanjiString);
			String kanaString = word.GetKanaString();
			try {
				size_t len = kanjiString.Length();
				for (size_t pos=1;pos<len;pos++) {
					if (kanjiString[pos] == _T('は') && (!converter.IsKana(kanjiString[pos-1]) || (pos >= 2 && kanjiString[pos-1] == _T('に') && !converter.IsKana(kanjiString[pos-2])))) {
						if (pos+1 == len || !converter.IsKana(kanjiString[pos+1])) {
							//Println(kanjiString +_T(", read ")+kanaString);
						}
					}
				}
				String roomaji = converter.KanaToRoomaji(kanaString);
				
				if (kanjiString.Length() < 10) {
					//kanji.AddWordReference(word);
				}
			} catch (std::exception &e) {
				Println(wxString(_T("Failed to convert ")) + kanaString.GetWxString() + _T(" to roomaji with message:"));
				Println(e.what());
			} catch (...) {
				Println("FAIL");
			}

			if (count % 1000 == 0) Print(".");
			count++;
		}
		Println("] Done.");
	}
	catch (std::exception &e) {
		Println((std::string("EXCEPTION! ") + e.what()).c_str());
	}
	catch (...) {
		Println("\nEXCEPTION!");
	}

	//Println("Now looping forever.");
	//while (true);

	return 0;
}


/////////////////////////////

bool TestApp::OnInit()
{
	TestFrame* frame = new TestFrame();
	frame->Show(true);
	return true;
}

TestFrame::TestFrame()
: wxFrame(NULL,-1,_T("Japanese Library Test"),wxDefaultPosition,wxSize(800,600))
{
	text = new wxTextCtrl(this,-1,_T(""),wxDefaultPosition,wxDefaultSize,wxTE_READONLY | wxTE_MULTILINE);
	wxLog::SetActiveTarget(new wxLogTextCtrl(text));
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(text,1,wxEXPAND,0);
	SetSizer(sizer);
	Connect(0,wxEVT_MY_EVENT,wxCommandEventHandler(TestFrame::OnText));

	count = 0;
	wxThread* thread = new WorkThread(this,&mutex,&count);
	thread->Create();
	thread->Run();
}

TestFrame::~TestFrame()
{
}

void TestFrame::OnText(wxCommandEvent &evt)
{
	text->AppendText(evt.GetString());
	wxMutexLocker locker(mutex);
	count--;
}


WorkThread::WorkThread(wxEvtHandler *_target,wxMutex *_mutex,volatile int *_count)
: wxThread(wxTHREAD_DETACHED)
{
	target = _target;
	mutex = _mutex;
	count = _count;
}

void WorkThread::Print(wxString str)
{
	wxCommandEvent* evt = new wxCommandEvent(wxEVT_MY_EVENT,0);
	evt->SetString(str);

	bool wait = true;
	while (wait) {
		wxMutexLocker locker(*mutex);
		if ((*count) < 5) {
			wait = false;
			(*count)++;
		}
	}

	target->AddPendingEvent(*evt);
}

void WorkThread::Println(wxString str)
{
	Print(str+_T("\n"));
}

void WorkThread::Print(const char *str)
{
	Print(wxString(str,wxConvUTF8));
}

void WorkThread::Println(const char *str)
{
	Println(wxString(str,wxConvUTF8));
}
