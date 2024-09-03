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

	wxComboBox *shutdown_list;

	wxCalendarCtrl *calendar;
	wxTextCtrl *textcalendar;
	wxButton *buttoncalendar;

	std::string stringdate;

	std::string he;
	std::string me;
	std::string se;

	std::string heA;
	std::string meA;
	std::string seA;

	wxSpinCtrl *spin_hour;
	wxSpinCtrl *spin_minute;
	wxSpinCtrl *spin_second;

	wxMenu* menu1;
	wxMenu* menu2;
	wxMenu* menu3;

	wxDateTime now;
	std::string date1;
	wxString datebar;
	wxString date2;
	wxString date3;

	wxFilePickerCtrl *sound_file_picker;

	wxTimer* m_timer;
	wxTimer* timeralarm;
	wxTimer* timermusic;

	bool media;
	int count;

	std::string file_path;
	std::string file_path_txt;
	std::string textmessage;

	wxTimer *ontimer;

	std::vector<std::vector<int>> alarms;

	wxNotebook *notebook;
	wxNotebook *notebook_c;
	wxNotebook *notebook_n;

	wxFont font1;
	wxFont font2;

	wxStaticText *hour_text;
	wxStaticText *minute_text;
	wxStaticText *second_text;

	wxPanel *panel_clock;
	wxPanel *panel_notes;

	wxSound *sound;
	wxMediaCtrl *mediactrl;

	wxTextCtrl *list_text;

	wxStaticText *time_now_text;
	wxStaticText *time_alarm_text;
	wxStaticText *time_shutdown_text;

	wxScrolledWindow *physics_panel;
	wxScrolledWindow *pattern_panel;

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

	void SetAlarmClock(std::vector<std::vector<int>> &alarms);

	void CreateWindowsPanel();
	void CreateMenu();

	wxNotebook *CreateNotebook();
	wxPanel *CreateClockPanel(wxWindow *parent);
	wxPanel *CreateNotesPanel(wxWindow* parent);

	//void Render(wxDC &dc);
private:
    // Этот класс обрабатывает сообщения
    DECLARE_EVENT_TABLE()
};
//===============================================================================================================
