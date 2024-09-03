#include "MyFrame.h"
#include "wx/datetime.h"
#include <wx/font.h>
#include "wx/wxprec.h"
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/window.h>
#include "stdlib.h"
#include <windows.h>

#include "Presenter.h"

enum
{
	TIMER_ID, EXIT_ID, SLEEP_ID, TIMERALARM_ID, ONSYSTEM_ID, ONALARM_ID, OFFALARM_ID, OP_ID, TIME_ID, PROG_ID, wxMY_ID, BT_CAL_ID, TIMERMUSIC_ID
};

//===============================================================================================================

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(OP_ID, OnOpen)
EVT_MENU(EXIT_ID, OnClose)
EVT_MENU(TIME_ID, OnTime)
EVT_MENU(PROG_ID, OnProgramm)
EVT_BUTTON(ONSYSTEM_ID, OnSystem)
EVT_BUTTON(ONALARM_ID, OnAlarm)
EVT_BUTTON(OFFALARM_ID, OffAlarm)
EVT_BUTTON(BT_CAL_ID, OnCalendar)
EVT_TIMER(TIMER_ID, OnTimer)
EVT_TIMER(TIMERMUSIC_ID, OnTimerMusic)
EVT_TIMER(TIMERALARM_ID, TimerAlarm)
//EVT_MEDIA_FINISHED(wxMY_ID, OnMediaFinished)
END_EVENT_TABLE()

//========================================================================================================================================

MyFrame::MyFrame(Presenter *presenter) :
	wxFrame(nullptr, wxID_ANY, "Электронный органайзер"),
	presenter(presenter)
{
	SetIcon(wxICON(Time));

	//m_timer = new wxTimer(this, TIMER_ID);
	timeralarm = new wxTimer(this, TIMERALARM_ID);
	timermusic = new wxTimer(this, TIMERMUSIC_ID);
	//m_timer->Start(1000, false);
	timeralarm->Start(1000, false);

	CreateWindowsPanel();

	//================================================================
	wxMenuBar* menubar = new wxMenuBar;
	SetMenuBar(menubar);
	menu1 = new wxMenu;
	menu2 = new wxMenu;
	menu3 = new wxMenu;
	menubar->Append(menu1, "Файл");
	menu1->Append(OP_ID, "Открыть");
	menu1->Append(EXIT_ID, "Выход");
	menubar->Append(menu2, "Параметры");
	menu2->Append(TIME_ID, "Время");
	menubar->Append(menu3, "Справка");
	menu3->Append(PROG_ID, "О программе ...");

	//================================================================	
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MyFrame::OnClose));
	//this ->Connect(wxEVT_MEDIA_FINISHED, wxMediaEventHandler(MyFrame::OnMediaFinished));

	CreateStatusBar();

	now = wxDateTime::Now();
	date1 = now.Format();

	std::fstream file("Alarmclock.txt", std::ios::out | std::ios::app);
	if (file.is_open())
	{
		file << "Программа была запущена в " << date1 << std::endl;
	}
	file.close();
}

//========================================================================================================================================

BOOL MySystemShutdown()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	// Get a token for this process. 

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return(FALSE);

	// Get the LUID for the shutdown privilege. 

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
		&tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Get the shutdown privilege for this process. 

	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
		(PTOKEN_PRIVILEGES)NULL, 0);

	if (GetLastError() != ERROR_SUCCESS)
		return FALSE;

	// Shut down the system and force all applications to close. 

	if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCE,
		SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
		SHTDN_REASON_MINOR_UPGRADE |
		SHTDN_REASON_FLAG_PLANNED))
		return FALSE;

	//shutdown was successful
	return TRUE;
}

//========================================================================================================================================

void MyFrame::TimerAlarm(wxTimerEvent& event)
{
	wxDateTime now = wxDateTime::Now();
	auto hour = wxAtoi(now.Format(wxT("%H")));
	auto minute = wxAtoi(now.Format(wxT("%M")));
	auto second = wxAtoi(now.Format(wxT("%S")));

	presenter->OnAlarmtTime();

	bool ringing = false;

	for (const auto &alarm : this->alarms)
	{
		if (alarm[1] == hour && alarm[2] == minute && alarm[3] == second)
		{
			ringing = true;
		}
	}

	date1 = now.Format();
	date2 = now.Format(wxT("%X"));
	date3 = now.Format(wxT("%x"));

	time_now_text->SetFont(font2);

	time_now_text->SetLabelText(date2);// вызываем ф-цию.

	file_path = sound_file_picker->GetPath();

	textmessage = list_text->GetLineText(0);

	if (ringing)
	{
		if (media == true)
		{
			timermusic->Start(1000, false);
		}
		if (textmessage.empty() || textmessage == "Надпись для будильника ...")
		{
			textmessage = "Вставай";
		}
		wxMessageDialog dialog(NULL, textmessage, wxT("Будильник"), wxOK | wxICON_INFORMATION);
		switch (dialog.ShowModal())
		{
		case wxID_OK:
			mediactrl->Stop();
			timermusic->Stop();
			break;
		}
	}

	if (time_now_text->GetLabelText() == he + ":" + me + ":" + se)
	{
		if (shutdown_list->GetValue() == "Перезагрузка компьютера")
		{
			//system("shutdown –r -f –t 0");
			//wxMessageBox(wxT("Спящий режим"));
			MySystemShutdown();
		}
		if (shutdown_list->GetValue() == "Выключение компьютера")
		{
			system("shutdown -s -f -t 0");
			//wxMessageBox(wxT("Выключение компьютера"));
		}
	}

	this->alarms.clear();
}

//========================================================================================================================================

void MyFrame::OnTimerMusic(wxTimerEvent& event)
{
	mediactrl->Play();
	count++;
	if (count == 600)
	{
		timermusic->Stop();
	}
	SetStatusText("Будильние играет " + std::to_string(count) + " sc", 0);
}

//========================================================================================================================================

void MyFrame::OnTimer(wxTimerEvent& event)
{
	//wxDateTime now = wxDateTime::Now();
	//date1 = now.Format();
	//date2 = now.Format(wxT("%X"));
	//date3 = now.Format(wxT("%x"));

	//label->SetFont(font2);

	//label->SetLabelText(date2);// вызываем ф-цию.
}

//========================================================================================================================================

MyFrame::~MyFrame()
{
}

//========================================================================================================================================

void MyFrame::OnSystem(wxCommandEvent& event)
{
	if (shutdown_list->GetValue() == "Выберите режим" && "")
	{
		wxMessageBox(wxT("Выберите режим"));
	}
	else
	{
		he = std::to_string(spin_hour->GetValue());
		me = std::to_string(spin_minute->GetValue());
		se = std::to_string(spin_second->GetValue());

		time_shutdown_text->SetFont(wxFont(35, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));

		for (int i = 0; i < 10; i++)
		{
			string n = std::to_string(i);
			if (he == n)
			{
				he = "0" + n;
			}
			if (me == n)
			{
				me = "0" + n;
			}
			if (se == n)
			{
				se = "0" + n;
			}
		}
		time_shutdown_text->SetLabelText(he + ":" + me + ":" + se);// вызываем ф-цию.
	}
	std::fstream file("Alarmclock.txt", std::ios::out | std::ios::app);
	if (file.is_open())
	{
		file << shutdown_list->GetValue() << " установлено/установлен в " << date1 << " на " << time_shutdown_text->GetLabelText() << std::endl;
	}
	file.close();
	SetStatusText("Установлен " + shutdown_list->GetValue() + " на время " + time_shutdown_text->GetLabelText(), 0);
}

//========================================================================================================================================

void MyFrame::OnAlarm(wxCommandEvent& event)
{
	if (!file_path.empty())
	{
		heA = std::to_string(spin_hour->GetValue());
		meA = std::to_string(spin_minute->GetValue());
		seA = std::to_string(spin_second->GetValue());

		media = mediactrl->Load(file_path);

		mediactrl->SetVolume(1);

		for (int i = 0; i < 10; i++)
		{
			string n = std::to_string(i);
			if (heA == n)
			{
				heA = "0" + n;
			}
			if (meA == n)
			{
				meA = "0" + n;
			}
			if (seA == n)
			{
				seA = "0" + n;
			}
		}
		time_alarm_text->SetLabelText(heA + ":" + meA + ":" + seA);// вызываем ф-цию.
	}
	else
	{
		wxMessageBox(wxT("Музыка не добавлена"));
	}

	std::fstream file("Alarmclock.txt", std::ios::out | std::ios::app);
	if (file.is_open())
	{
		file << "Будильник был установлен в " << date1 << " на " << time_alarm_text->GetLabelText() << "-" << textmessage << std::endl;
	}
	file.close();
}

//=======================================================================================================================================

void MyFrame::OffAlarm(wxCommandEvent& event)
{
	heA = "    ";
	meA = "    ";
	seA = "    ";
	time_alarm_text->SetLabelText(heA + ":" + meA + ":" + seA);// вызываем ф-цию.
}

//=======================================================================================================================================

void MyFrame::OnClose(wxCloseEvent& event)
{
	wxMessageDialog* dial = new wxMessageDialog(NULL, wxT("Вы точно хотите выйти?"), wxT("Выход"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
	int ret = dial->ShowModal();
	dial->Destroy();
	if (ret == wxID_YES)
	{
		std::fstream file("Alarmclock.txt", std::ios::out | std::ios::app);
		if (file.is_open())
		{
			file << "Программа была выключена в " << date1 << std::endl;
		}
		file.close();

		Destroy();
	}
	else
	{

	}
}

//=======================================================================================================================================

void MyFrame::OnOpen(wxCommandEvent& event)
{
	wxMessageDialog dialog(NULL, wxT("Открой этот мир для себя"), wxT("Открыть"), wxOK | wxICON_INFORMATION);
	switch (dialog.ShowModal())
	{
	case wxID_OK:
		break;
	}
}

//=======================================================================================================================================

void MyFrame::OnClose(wxCommandEvent& event)
{
	Close(true);
}
//=======================================================================================================================================

void MyFrame::OnTime(wxCommandEvent& event)

{
	wxMessageDialog dialog(NULL, wxT("Нельзя завести 500 миллионов друзей не нажив ни одного врага"), wxT("Время"), wxOK | wxICON_INFORMATION);
	switch (dialog.ShowModal())
	{
	case wxID_OK:
		break;
	}
}

//=======================================================================================================================================

void MyFrame::OnProgramm(wxCommandEvent& event)
{
	wxMessageDialog dialog(NULL, wxT("Органайзер для выключения компьютера и установления будильника \nВасиленко Ярослав 2018 год ©"), wxT("Справка о программе"), wxOK | wxICON_INFORMATION);
	switch (dialog.ShowModal())
	{
	case wxID_OK:
		break;
	}
}

//=======================================================================================================================================

void MyFrame::SetAlarmClock(std::vector<std::vector<int>> &alarms)
{
	for (const auto &alarm : alarms)
	{
		this->alarms.push_back(alarm);
	}
}

//=======================================================================================================================================

void MyFrame::CreateWindowsPanel()
{
	notebook = CreateNotebook();
}

//=======================================================================================================================================

void MyFrame::CreateMenu()
{
}

//=======================================================================================================================================

wxNotebook *MyFrame::CreateNotebook()
{
	notebook = new wxNotebook(this, -1, wxDefaultPosition, wxDefaultSize);

	notebook_c = new wxNotebook(notebook, wxID_ANY);
	notebook_n = new wxNotebook(notebook, wxID_ANY);

	const auto physics_parameters = CreateClockPanel(notebook_c);
	const auto pattern_parameters = CreateNotesPanel(notebook_n);

	notebook->AddPage(notebook_c, L"Время", true);
	notebook->AddPage(notebook_n, L"Заметки", true);

	notebook->SetSelection(0);

	return notebook;
}

//=======================================================================================================================================

wxPanel *MyFrame::CreateClockPanel(wxWindow *parent)
{
	//	physics_panel = new wxScrolledWindow(parent);
	//physics_panel->Fit();
	//physics_panel->SetScrollRate(0, 38);

	panel_clock = new wxPanel(parent, -1);
	panel_clock->SetBackgroundColour(wxColour(255, 255, 255));

	wxButton *onsystem = new wxButton(panel_clock, ONSYSTEM_ID, wxT("Установить время для режима"));
	wxButton *onalarm = new wxButton(panel_clock, ONALARM_ID, wxT("Установить будильник"));
	wxButton *offalarm = new wxButton(panel_clock, OFFALARM_ID, wxT("Удалить будильник"));

	list_text = new wxTextCtrl(panel_clock, -1, wxT("..."), wxDefaultPosition, wxDefaultSize, /*wxTE_MULTILINE */ 0);

	shutdown_list = new wxComboBox(panel_clock, -1, wxString("Выберите режим"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	shutdown_list->AppendString("Спящий режим");
	shutdown_list->AppendString("Выключение компьютера");

	time_now_text = new wxStaticText(panel_clock, -1, wxT(""));
	time_now_text->SetFont(wxFont(35, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));

	time_alarm_text = new wxStaticText(panel_clock, -1, wxT("    :    :    "));
	time_alarm_text->SetFont(wxFont(35, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));

	time_shutdown_text = new wxStaticText(panel_clock, -1, wxT("    :    :    "));
	time_shutdown_text->SetFont(wxFont(35, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));

	spin_hour = new wxSpinCtrl(panel_clock, 10, "0");
	spin_minute = new wxSpinCtrl(panel_clock, 10, "0");
	spin_second = new wxSpinCtrl(panel_clock, 10, "0");

	hour_text = new wxStaticText(panel_clock, -1, wxT("Часы"));
	minute_text = new wxStaticText(panel_clock, -1, wxT("Минуты"));
	second_text = new wxStaticText(panel_clock, -1, wxT("Секунды"));

	font1 = wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
	font2 = wxFont(35, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	hour_text->SetFont(font1);
	minute_text->SetFont(font1);
	second_text->SetFont(font1);

	sound_file_picker = new wxFilePickerCtrl(panel_clock, wxID_ANY, wxEmptyString, wxT("Музыку для будильника"), wxT("*.wav;*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE);

	spin_hour->SetMax(23);
	spin_minute->SetMax(23);
	spin_second->SetMax(23);

	mediactrl = new wxMediaCtrl(panel_clock, wxMY_ID, file_path);

	//mediactrl ->ShowPlayerControls(wxMEDIACTRLPLAYERCONTROLS_STEP);	
	media = mediactrl->Load(file_path);
	count = 0;

	wxBoxSizer* box1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* boxh = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* boxm = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* boxs = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* boxHAlarm = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* boxMAlarm = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* boxSAlarm = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* boxsystem = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* box11 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* box2 = new wxBoxSizer(wxVERTICAL);

	wxStaticBox* Box = new wxStaticBox(panel_clock, wxID_ANY, "Время сейчас");
	wxStaticBoxSizer* box3 = new wxStaticBoxSizer(Box, wxHORIZONTAL);

	wxStaticBox* Boxexit = new wxStaticBox(panel_clock, wxID_ANY, "Время выключения");
	wxStaticBoxSizer* boxexit = new wxStaticBoxSizer(Boxexit, wxHORIZONTAL);

	wxStaticBox* BoxAlarmClock = new wxStaticBox(panel_clock, wxID_ANY, "Время будильника");
	wxStaticBoxSizer* boxalarm = new wxStaticBoxSizer(BoxAlarmClock, wxHORIZONTAL);

	//================================================================
	boxh->Add(spin_hour, 0, wxALL, 10);
	boxh->Add(hour_text, 0, wxALL, 10);
	boxm->Add(spin_minute, 0, wxALL, 10);
	boxm->Add(minute_text, 0, wxALL, 10);
	boxs->Add(spin_second, 0, wxALL, 10);
	boxs->Add(second_text, 0, wxALL, 10);

	spin_hour->SetMinSize(wxSize(200, 20));
	spin_minute->SetMinSize(wxSize(200, 20));
	spin_second->SetMinSize(wxSize(200, 20));

	box1->Add(boxh, 0, wxALL);
	box1->Add(boxm, 0, wxALL);
	box1->Add(boxs, 0, wxALL);

	wxBoxSizer* boxbutton = new wxBoxSizer(wxHORIZONTAL);
	boxbutton->Add(onalarm, 0, wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	boxbutton->Add(offalarm, 0, wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	box2->Add(boxbutton, 0);
	box2->Add(sound_file_picker, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	box2->Add(list_text, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);

	//================================================================
	boxsystem->Add(shutdown_list, 0, wxALL, 5);
	boxsystem->Add(onsystem, 0, wxALL, 5);
	//================================================================
	box1->Add(boxsystem, 0, wxALL, 2);
	//================================================================
	box3->Add(time_now_text, 0, wxLEFT | wxRIGHT | wxTOP | wxDOWN, 2);
	box1->Add(box3, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	//================================================================
	boxexit->Add(time_alarm_text, 0, wxLEFT | wxRIGHT | wxTOP | wxDOWN, 2);
	box1->Add(boxexit, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	//================================================================
	boxalarm->Add(time_shutdown_text, 0, wxLEFT | wxRIGHT | wxTOP | wxDOWN, 2);
	box2->Add(boxalarm, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	//================================================================
	auto sizer_1 = new wxBoxSizer(wxVERTICAL);
	//================================================================
	sizer_1->Add(box1, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 10);
	sizer_1->Add(box2, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 10);

	panel_clock->SetSizer(sizer_1);

	sizer_1->Fit(panel_clock);

	return panel_clock;
}

//=======================================================================================================================================

wxPanel *MyFrame::CreateNotesPanel(wxWindow *parent)
{
	panel_notes = new wxPanel(parent, -1);

	//================================================================
	calendar = new wxCalendarCtrl(panel_notes, 999, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxCAL_SHOW_HOLIDAYS);
	textcalendar = new wxTextCtrl(panel_notes, -1, wxT("Заметка ..."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	buttoncalendar = new wxButton(panel_notes, BT_CAL_ID, wxT("Сохранить заметку "));

	wxBoxSizer* box33 = new wxBoxSizer(wxVERTICAL);

	box33->Add(calendar, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	box33->Add(textcalendar, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	box33->Add(buttoncalendar, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);

	panel_notes->SetSizer(box33);

	box33->Fit(panel_notes);

	return panel_notes;
}

//=======================================================================================================================================

void MyFrame::OnCalendar(wxCommandEvent& event)
{
	std::fstream file("Заметка.txt", std::ios::out);
	if (file.is_open())
	{
		auto text = textcalendar->GetValue();
		file << text << "   " << date2 << std::endl;
	}
	file.close();
}
//=======================================================================================================================================