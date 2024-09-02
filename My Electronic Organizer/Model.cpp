
#include "Model.h"

enum
{
	TIMERALARM_ID
};

Model::Model()
{
	alarm_clocks.clear();
	AddAlarmClock(28, 0, 26, 0, "Message");
}

Model::~Model()
{
}

void Model::AddAlarmClock(int day, int hour, int minute, int second, std::string message)
{
	AlarmClock *alarm = new AlarmClock();
	alarm->SetAnAlarm(day, hour, minute, second, message);
	alarm_clocks.push_back(alarm);
}

std::vector<std::vector<int>> Model::GetAlarms()
{
	std::vector<std::vector<int>> alarms;

	for (const auto &alarm : alarm_clocks)
	{
		std::vector<int> temp;
		temp.push_back(0);
		temp.push_back(alarm->GetHour());
		temp.push_back(alarm->GetMinute());
		temp.push_back(0);
		alarms.push_back(temp);
	}

	return alarms;
}
