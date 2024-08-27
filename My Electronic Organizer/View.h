
#pragma once

//==============================================================================

#include "View.h"

//==============================================================================

class MyFrame;
class Presenter;

//==============================================================================

class View
{
private:
	Presenter *presenter;
	MyFrame *frame;

public:
	View();
	View(const View&) = delete;
	View(View&&) = delete;
	~View();

	void ShowFrame();

};