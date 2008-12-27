#pragma once

#include <wx/wxprec.h>

DECLARE_EVENT_TYPE(wxEVT_MY_EVENT, -1)

class TestApp : public wxApp {
public:
	bool OnInit(); 
};

class TestFrame : public wxFrame {
public:
	TestFrame();
	~TestFrame();

private:
	wxTextCtrl* text;
	volatile int count;
	wxMutex mutex;
	void OnText(wxCommandEvent &evt);
};

class WorkThread : public wxThread {
public:
	WorkThread(wxEvtHandler *_target,wxMutex *mutex,volatile int *count);
	wxThread::ExitCode Entry();

private:
	wxMutex *mutex;
	volatile int *count;
	wxEvtHandler *target;
	void Print(wxString str);
	void Println(wxString str);
	void Print(const char *str);
	void Println(const char *str);
};
