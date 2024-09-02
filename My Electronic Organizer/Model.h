
#pragma once

#include <vector>

#include <wx/wx.h>
#include <wx/mediactrl.h>
#include "wx/datetime.h"

#include <iostream>
#include <chrono>

#include "AlarmClock.h"

class Model
{
public:
	Model();
	Model(const Model&) = delete;
	Model(Model&&) = delete;
	~Model();

	void AddAlarmClock(int day, int hour, int minute, int second, 
						std::string message);

	std::vector<std::vector<int>> GetAlarms();

private:
	DECLARE_EVENT_TABLE()

	std::vector<AlarmClock*> alarm_clocks;
};