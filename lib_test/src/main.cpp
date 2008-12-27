#include <iostream>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include "main.h"
#include "../../japanese_lib/include/japanese.h"

IMPLEMENT_APP(TestApp)
DEFINE_EVENT_TYPE(wxEVT_MY_EVENT)


/////////////////////////////


wxThread::ExitCode WorkThread::Entry()
{
	using namespace Japanese;

	try {

		Print("Loading Kana converter... ");
		KanaConverter converter;
		Println("Done.");

		Print("Loading Kanjidic... ");
		KanjiManager kanji;
		{
			wxFileInputStream input(_T("E:/Projects/japanese_tools/kanjidic2.zip"));
			wxBufferedInputStream buffer(input);
			wxZipInputStream zip(buffer);
			wxZipEntry *entry = zip.GetNextEntry();
			if (entry) {
				kanji.LoadFromKanjidic2(zip);
				delete entry;
			}
			zip.CloseEntry();
		}
		Println("Done.");
		Println(wxString::Format(_T("%i kanji were loaded."),kanji.GetCount()));

		Print("Loading JMDict... ");
		WordManager words;
		{
			wxFileInputStream input(_T("E:/Projects/japanese_tools/jmdict.zip"));
			wxBufferedInputStream buffer(input);
			wxZipInputStream zip(buffer);
			wxZipEntry *entry = zip.GetNextEntry();
			if (entry) {
				words.LoadFromJMDict(zip);
				delete entry;
			}
			zip.CloseEntry();
		}
		Println("Done.");
		Println(wxString::Format(_T("%i words were loaded."),words.GetCount()));

		wxString wordList[] = { L"海上保安庁", L"案内", L"禁煙", L"群馬県", L"西班牙",
			                    L"インターネット", L"東京", L"母さん", L"牛乳", L"設置",
								L"今日は", L"乎古止点", L"森の奥", L"森", L"新しい",
								L"色々", L"人々", L"今日", L"承る", L"志", L"恥ずかしい",
								L"青空", L"完璧", L"日本", L"一回" };
		int len = sizeof(wordList)/sizeof(wxString);
		for (int i=0;i<len;i++) {
			Print(_T("\nPicking word: ")+wordList[i]);
			Word word = words.GetWord(wordList[i]);
			if (word.IsValid()) {
				String kanjiString = word.GetKanjiString();
				String kanaString = word.GetKanaString();
				Println(wxString(_T(" Done. "))+kanjiString+_T(" is read ")+kanaString+_T(" (")+converter.KanaToRoomaji(kanaString)+_T(") or, in katakana, ")+converter.KanaToKatakana(kanaString));
				Println(_T("Reading: ") + word.GetWordReading(kanji).GetFullReading());
				for (size_t i=0;false && i<kanjiString.Length();i++) {
					Kanji curKanji = kanji.GetKanji(kanjiString[i]);
					Println(_T("Got kanji: ")+curKanji.GetName());
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
				Println(wxString(_T("Failed to convert ")) + kanaString + _T(" to roomaji with message:"));
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
