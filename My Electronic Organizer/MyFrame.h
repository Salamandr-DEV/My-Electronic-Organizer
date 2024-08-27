#pragma once

#include <wx/wx.h>
//===============================================================================================================
#include <stdlib.h>
#include <stdio.h>
#include <wx/image.h>
#include "wx/msgdlg.h"
#include <wx/utils.h>
#include "wx/tipdlg.h"
#include "wx/icon.h"
#include <wx/stattext.h>
#include <wx/notebook.h>
#include <wx/sound.h>
#include <wx/filepicker.h>
#include "wx/textfile.h"
#include <wx/string.h>
#include <wx/taskbar.h>
#include <wx/calctrl.h>
#include <wx/spinctrl.h>

#include <wx/dir.h>
#include <wx/mediactrl.h>
//===============================================================================================================
#include <fstream>   
#include <string>
#include <memory>
//===============================================================================================================
#include <iostream>
#include <chrono>
//===============================================================================================================
using namespace std;

class Presenter;

//===============================================================================================================
class MyFrame : public wxFrame
{
private:
	Presenter *presenter;
	wxImage image;
	wxBitmap bitmap;

	wxComboBox* System;

	wxCalendarCtrl* calendar;
	wxTextCtrl* textcalendar;
	wxButton* buttoncalendar;

	std::string stringdate;

	std::string he;
	std::string me;
	std::string se;

	std::string heA;
	std::string meA;
	std::string seA;

	wxSpinCtrl* spinh;
	wxSpinCtrl* spinm;
	wxSpinCtrl* spins;

	wxSpinCtrl* spinHAlarm;
	wxSpinCtrl* spinMAlarm;
	wxSpinCtrl* spinSAlarm;

	wxMenu* menu1;
	wxMenu* menu2;
	wxMenu* menu3;

	wxDateTime now;
	std::string date1;
	wxString datebar;
	wxString date2;
	wxString date3;

	wxStaticText* labelh;
	wxStaticText* labelm;
	wxStaticText* labels;

	wxStaticText* labelHAlarm;
	wxStaticText* labelMAlarm;
	wxStaticText* labelSAlarm;

	wxStaticText* label;
	wxStaticText* label2;
	wxStaticText* labelexit;
	wxStaticText* labelALARM;

	wxPanel* panel;
	wxPanel* panel2;
	wxPanel* panel3;
	wxNotebook* MyNotebook;
	wxSound* sound;
	wxMediaCtrl* mediactrl;

	wxFilePickerCtrl* FilePicker;

	wxTimer* m_timer;
	wxTimer* timeralarm;
	wxTimer* timermusic;
	wxFont font1;
	wxFont font2;
	wxTextCtrl* text;
	bool media;
	int count;

	std::string file_path;
	std::string file_path_txt;
	std::string textmessage;

public:
	MyFrame(Presenter *presenter);
	MyFrame(const MyFrame &) = delete;
	MyFrame(MyFrame &&) = delete;
	~MyFrame() override;
	
	//wxNotebook* CreateNotebook(wxWindow* parent);

	void OnSystem(wxCommandEvent& event);
	void OnAlarm(wxCommandEvent& event);
	void OffAlarm(wxCommandEvent& event);
	void OnCalendar(wxCommandEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnTimerMusic(wxTimerEvent& event);
	void TimerAlarm(wxTimerEvent& event);
	void OnClose(wxCloseEvent& event);

	void OnOpen(wxCommandEvent& event);
	void OnClose(wxCommandEvent& event);
	void OnTime(wxCommandEvent& event);
	void OnProgramm(wxCommandEvent& event);
	//void OnMediaFinished(wxMediaEvent & event);

	//void Render(wxDC &dc);
private:
    // Этот класс обрабатывает сообщения
    DECLARE_EVENT_TABLE()
};
//===============================================================================================================
