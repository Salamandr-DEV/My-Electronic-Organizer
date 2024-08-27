
#pragma once

#include "Model.h"

class View;

class Presenter
{
public:
	explicit Presenter(View *view);
	Presenter(const Presenter&) = delete;
	Presenter(Presenter&&) = delete;
	~Presenter();

private:
	View *view;
	Model *model;
};