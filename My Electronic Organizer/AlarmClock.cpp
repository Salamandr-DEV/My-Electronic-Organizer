
#include "AlarmClock.h"

AlarmClock::AlarmClock() :
	day(0),
	hour(0),
	minute(0),
	second(0),
	message("")
{
}

AlarmClock::~AlarmClock()
{
}

void AlarmClock::SetAnAlarm(int &day, int &hour, int &minute, int &second, 
							std::string &message)
{
	this->day = day;

	this->hour = hour;
	this->minute = minute;
	this->second = second;

	this->message = message;
}

void AlarmClock::DeleteAnAlarmClock()
{
	this->hour = 0;
	this->minute = 0;
	this->second = 0;

	this->message = "";
}

void AlarmClock::SetOffTheAlarm()
{
	this->minute = this->minute + 5;

	if (this->minute > 60)
	{
		this->hour = hour + 1;
		this->minute = this->minute - 60;
	}
	if (this->hour > 24)
	{
		this->hour = hour - 24;
	}
}

void AlarmClock::TurnOffTheAlarm()
{

}

void AlarmClock::Ringing()
{
	wxMessageBox(message);
}

int AlarmClock::GetHour()
{
	return hour;
}

int AlarmClock::GetMinute()
{
	return minute;
}
