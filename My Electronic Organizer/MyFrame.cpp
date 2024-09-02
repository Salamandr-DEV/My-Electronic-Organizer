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

//===============================================================================================================
MyFrame::MyFrame(Presenter *presenter) :
	wxFrame(nullptr, wxID_ANY, "Электронный органайзер"),
	presenter(presenter)
{
	SetIcon(wxICON(Time));
	//================================================================
	MyNotebook = new wxNotebook(this, -1, wxDefaultPosition, wxDefaultSize);
	//================================================================
	panel3 = new wxPanel(MyNotebook, -1);
	panel2 = new wxPanel(MyNotebook, -1);
	panel = new wxPanel(MyNotebook, -1);
	//================================================================
	wxButton* onsystem = new wxButton(panel, ONSYSTEM_ID, wxT("Установить время"));
	wxButton* onalarm = new wxButton(panel2, ONALARM_ID, wxT("Установить будильник"));
	wxButton* offalarm = new wxButton(panel2, OFFALARM_ID, wxT("Удалить будильник"));
	text = new wxTextCtrl(panel2, -1, wxT("Надпись для будильника ..."), wxDefaultPosition, wxDefaultSize, /*wxTE_MULTILINE */ 0);
	System = new wxComboBox(panel, -1, wxString("Выберите режим"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	System->AppendString("Спящий режим");
	System->AppendString("Выключение компьютера");
	//================================================================
	label = new wxStaticText(panel, -1, wxT(""));
	//================================================================
	label2 = new wxStaticText(panel2, -1, wxT(""));
	//================================================================
	labelexit = new wxStaticText(panel, -1, wxT("    :    :    "));
	labelexit->SetFont(wxFont(35, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));
	//================================================================
	labelALARM = new wxStaticText(panel2, -1, wxT("    :    :    "));
	labelALARM->SetFont(wxFont(35, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));
	//================================================================
	m_timer = new wxTimer(this, TIMER_ID);
	timeralarm = new wxTimer(this, TIMERALARM_ID);
	timermusic = new wxTimer(this, TIMERMUSIC_ID);
	m_timer->Start(1000, false);
	timeralarm->Start(1000, false);
	//timermusic->Start(1000, false);
	//================================================================
	spinh = new wxSpinCtrl(panel, 10, "0");
	spinm = new wxSpinCtrl(panel, 10, "0");
	spins = new wxSpinCtrl(panel, 10, "0");
	//================================================================
	spinHAlarm = new wxSpinCtrl(panel2, 10, "0");
	spinMAlarm = new wxSpinCtrl(panel2, 10, "0");
	spinSAlarm = new wxSpinCtrl(panel2, 10, "0");
	//================================================================
	labelh = new wxStaticText(panel, -1, wxT("Часы"));
	labelm = new wxStaticText(panel, -1, wxT("Минуты"));
	labels = new wxStaticText(panel, -1, wxT("Секунды"));
	//================================================================
	labelHAlarm = new wxStaticText(panel2, -1, wxT("Часы"));
	labelMAlarm = new wxStaticText(panel2, -1, wxT("Минуты"));
	labelSAlarm = new wxStaticText(panel2, -1, wxT("Секунды"));
	//================================================================
	font1 = wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
	font2 = wxFont(35, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
	labelh->SetFont(font1);
	labelm->SetFont(font1);
	labels->SetFont(font1);

	labelHAlarm->SetFont(font1);
	labelMAlarm->SetFont(font1);
	labelSAlarm->SetFont(font1);
	//================================================================
	wxBoxSizer* box1 = new wxBoxSizer(wxVERTICAL);
	//================================================================
	wxBoxSizer* boxh = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* boxm = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* boxs = new wxBoxSizer(wxHORIZONTAL);
	//================================================================
	wxBoxSizer* boxHAlarm = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* boxMAlarm = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* boxSAlarm = new wxBoxSizer(wxHORIZONTAL);
	//================================================================
	wxBoxSizer* boxsystem = new wxBoxSizer(wxHORIZONTAL);
	//================================================================
	wxBoxSizer* box11 = new wxBoxSizer(wxVERTICAL);
	//================================================================
	wxBoxSizer* box2 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* box33 = new wxBoxSizer(wxVERTICAL);
	//================================================================
	FilePicker = new wxFilePickerCtrl(panel2, wxID_ANY, wxEmptyString, wxT("Музыку для будильника"), wxT("*.wav;*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE);
	//================================================================
	wxStaticBox* Box = new wxStaticBox(panel, wxID_ANY, "Время сейчас");
	wxStaticBoxSizer* box3 = new wxStaticBoxSizer(Box, wxHORIZONTAL);
	//================================================================
	wxStaticBox* Boxexit = new wxStaticBox(panel, wxID_ANY, "Время выключения");
	wxStaticBoxSizer* boxexit = new wxStaticBoxSizer(Boxexit, wxHORIZONTAL);
	//================================================================
	//================================================================
	wxStaticBox* BoxAlarm = new wxStaticBox(panel2, wxID_ANY, "Время сейчас");
	wxStaticBoxSizer* box4 = new wxStaticBoxSizer(BoxAlarm, wxHORIZONTAL);
	//================================================================
	wxStaticBox* BoxAlarmClock = new wxStaticBox(panel2, wxID_ANY, "Время будильника");
	wxStaticBoxSizer* boxalarm = new wxStaticBoxSizer(BoxAlarmClock, wxHORIZONTAL);
	////================================================================
	boxh->Add(spinh, 0, wxALL, 10);
	boxh->Add(labelh, 0, wxALL, 10);
	boxm->Add(spinm, 0, wxALL, 10);
	boxm->Add(labelm, 0, wxALL, 10);
	boxs->Add(spins, 0, wxALL, 10);
	boxs->Add(labels, 0, wxALL, 10);
	//================================================================
	boxHAlarm->Add(spinHAlarm, 0, wxALL, 10);
	boxHAlarm->Add(labelHAlarm, 0, wxALL, 10);
	boxMAlarm->Add(spinMAlarm, 0, wxALL, 10);
	boxMAlarm->Add(labelMAlarm, 0, wxALL, 10);
	boxSAlarm->Add(spinSAlarm, 0, wxALL, 10);
	boxSAlarm->Add(labelSAlarm, 0, wxALL, 10);
	//================================================================
	box1->Add(boxh, 0, wxALL);
	box1->Add(boxm, 0, wxALL);
	box1->Add(boxs, 0, wxALL);
	//================================================================
	box2->Add(boxHAlarm, 0, wxALL);
	box2->Add(boxMAlarm, 0, wxALL);
	box2->Add(boxSAlarm, 0, wxALL);
	//================================================================
	wxBoxSizer* boxbutton = new wxBoxSizer(wxHORIZONTAL);
	boxbutton->Add(onalarm, 0, wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	boxbutton->Add(offalarm, 0, wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	box2->Add(boxbutton, 0);
	box2->Add(FilePicker, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	box2->Add(text, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	//================================================================
	boxsystem->Add(System, 0, wxALL, 5);
	boxsystem->Add(onsystem, 0, wxALL, 5);
	//================================================================
	box1->Add(boxsystem, 0, wxALL, 2);
	//================================================================
	box3->Add(label, 0, wxLEFT | wxRIGHT | wxTOP | wxDOWN, 2);
	box1->Add(box3, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	//================================================================
	box4->Add(label2, 0, wxLEFT | wxRIGHT | wxTOP | wxDOWN, 2);
	box2->Add(box4, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	//================================================================
	boxexit->Add(labelexit, 0, wxLEFT | wxRIGHT | wxTOP | wxDOWN, 2);
	box1->Add(boxexit, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	//================================================================
	boxalarm->Add(labelALARM, 0, wxLEFT | wxRIGHT | wxTOP | wxDOWN, 2);
	box2->Add(boxalarm, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	//================================================================
	wxGridSizer* sizer1 = new wxGridSizer(1, 1, 0, 0);
	wxGridSizer* sizer2 = new wxGridSizer(1, 1, 0, 0);
	wxGridSizer* sizer3 = new wxGridSizer(1, 1, 0, 0);
	//================================================================
	sizer1->Add(panel, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 10);
	sizer2->Add(panel2, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 10);
	sizer3->Add(panel3, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 10);
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
	calendar = new wxCalendarCtrl(panel3, 999, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxCAL_SHOW_HOLIDAYS);
	textcalendar = new wxTextCtrl(panel3, -1, wxT("Заметка ..."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	buttoncalendar = new wxButton(panel3, BT_CAL_ID, wxT("Сохранить заметку "));
	box33->Add(calendar, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	box33->Add(textcalendar, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);
	box33->Add(buttoncalendar, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxDOWN, 5);

	panel->SetSizer(box1);
	panel2->SetSizer(box2);
	panel3->SetSizer(box33);
	//================================================================
	box11->Add(MyNotebook, 1, wxEXPAND);
	SetSizer(box11);
	//================================================================
	MyNotebook->AddPage(panel, wxT("Время"));
	MyNotebook->AddPage(panel2, wxT("Будильник"));
	MyNotebook->AddPage(panel3, wxT("Заметки"));
	//================================================================	
	spinh->SetMax(23);
	spinm->SetMax(59);
	spins->SetMax(59);
	//================================================================	
	spinHAlarm->SetMax(23);
	spinMAlarm->SetMax(59);
	spinSAlarm->SetMax(59);
	//================================================================
	wxSize k = wxSize(374, 576);
	this->SetSize(k);
	this->SetMinSize(k);
	this->SetMaxSize(k);
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

	mediactrl = new wxMediaCtrl(panel3, wxMY_ID, file_path);

	//mediactrl ->ShowPlayerControls(wxMEDIACTRLPLAYERCONTROLS_STEP);	
	media = mediactrl->Load(file_path);
	count = 0;

	//wxMessageBox(now.Format());
	//wxMessageBox(now.Format(wxT("%H")));
	//wxMessageBox(now.Format(wxT("%M")));

	/*wxImage::AddHandler(new wxBMPHandler);
	bitmap = wxBITMAP(back);
	image = bitmap.ConvertToImage();*/
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

	//if (ringing)
	//{
	//	alarm_media->Play();
	//}

	//wxMessageDialog dialog(NULL, "", wxT("Будильник"), wxOK | wxICON_INFORMATION);
	//switch (dialog.ShowModal())
	//{
	//case wxID_OK:
	//	alarm_media->Stop();
	//	break;
	//}

	date1 = now.Format();
	date2 = now.Format(wxT("%X"));
	date3 = now.Format(wxT("%x"));

	label2->SetFont(font2);

	label2->SetLabelText(date2);// вызываем ф-цию.

	file_path = FilePicker->GetPath();

	textmessage = text->GetLineText(0);

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
	wxDateTime now = wxDateTime::Now();
	date1 = now.Format();
	date2 = now.Format(wxT("%X"));
	date3 = now.Format(wxT("%x"));

	label->SetFont(font2);

	label->SetLabelText(date2);// вызываем ф-цию.

	if (label->GetLabelText() == he + ":" + me + ":" + se)
	{
		if (System->GetValue() == "Перезагрузка компьютера")
		{
			//system("shutdown –r -f –t 0");
			//wxMessageBox(wxT("Спящий режим"));
			MySystemShutdown();
		}
		if (System->GetValue() == "Выключение компьютера")
		{
			system("shutdown -s -f -t 0");
			//wxMessageBox(wxT("Выключение компьютера"));
		}
	}
}

//========================================================================================================================================

MyFrame::~MyFrame()
{
}

//========================================================================================================================================

void MyFrame::OnSystem(wxCommandEvent& event)
{
	if (System->GetValue() == "Выберите режим" && "")
	{
		wxMessageBox(wxT("Выберите режим"));
	}
	else
	{
		he = std::to_string(spinh->GetValue());
		me = std::to_string(spinm->GetValue());
		se = std::to_string(spins->GetValue());

		labelexit->SetFont(wxFont(35, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));

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
		labelexit->SetLabelText(he + ":" + me + ":" + se);// вызываем ф-цию.
	}
	std::fstream file("Alarmclock.txt", std::ios::out | std::ios::app);
	if (file.is_open())
	{
		file << System->GetValue() << " установлено/установлен в " << date1 << " на " << labelexit->GetLabelText() << std::endl;
	}
	file.close();
	SetStatusText("Установлен " + System->GetValue() + " на время " + labelexit->GetLabelText(), 0);
}
//========================================================================================================================================
void MyFrame::OnAlarm(wxCommandEvent& event)
{
	if (!file_path.empty())
	{
		heA = std::to_string(spinHAlarm->GetValue());
		meA = std::to_string(spinMAlarm->GetValue());
		seA = std::to_string(spinSAlarm->GetValue());

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
		labelALARM->SetLabelText(heA + ":" + meA + ":" + seA);// вызываем ф-цию.
	}
	else
	{
		wxMessageBox(wxT("Музыка не добавлена"));
	}

	std::fstream file("Alarmclock.txt", std::ios::out | std::ios::app);
	if (file.is_open())
	{
		file << "Будильник был установлен в " << date1 << " на " << labelALARM->GetLabelText() << "-" << textmessage << std::endl;
	}
	file.close();
}
//=======================================================================================================================================
void MyFrame::OffAlarm(wxCommandEvent& event)
{
	heA = "    ";
	meA = "    ";
	seA = "    ";
	labelALARM->SetLabelText(heA + ":" + meA + ":" + seA);// вызываем ф-цию.
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