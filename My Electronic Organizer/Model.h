
#pragma once

#include <vector>

#include "AlarmClock.h"

class Model
{
public:
	Model();
	Model(const Model&) = delete;
	Model(Model&&) = delete;
	~Model();

private:
	std::vector<AlarmClock*> alarm_clocks;
};