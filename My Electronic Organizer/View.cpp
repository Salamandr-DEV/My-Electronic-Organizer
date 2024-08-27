
#include "View.h"
#include "MyFrame.h"
#include "Presenter.h"

View::View() :
	presenter(nullptr),
	frame(nullptr)
{
	presenter = new Presenter(this);
	frame = new MyFrame(presenter);
}

View::~View()
{
	delete presenter;
	presenter = nullptr;
}

void View::ShowFrame()
{
	frame->Show();
}
