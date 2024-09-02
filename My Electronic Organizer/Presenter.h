
#pragma once

#include "Model.h"
#include <vector>

class View;

class Presenter
{
public:
	explicit Presenter(View *view);
	Presenter(const Presenter&) = delete;
	Presenter(Presenter&&) = delete;
	~Presenter();

	void OnAlarmtTime();
	
private:
	View *view;
	Model *model;
};