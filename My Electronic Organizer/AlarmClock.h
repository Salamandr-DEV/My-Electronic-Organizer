
#pragma once

#include <string>

enum
{
	MONDAY = 1,
	TUESDAY = 2,
	WEDNESDAY = 3,
	THURSDAY = 4,
	FRIDAY = 5,
	SATURDAY = 6,
	SUNDAY = 7

};

class AlarmClock
{
public:
	AlarmClock();
	~AlarmClock();

	void SetAnAlarm(int &day, int &hour, int &minute, int &second, 
					std::string &message);
	void DeleteAnAlarmClock();

	void SetOffTheAlarm();
	void TurnOffTheAlarm();

	void Ringing();

private:
	int hour;
	int minute;
	int second;

	int day;

	std::string message;
};