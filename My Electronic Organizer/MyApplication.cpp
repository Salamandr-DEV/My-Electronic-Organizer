
#include "MyApplication.h"
#include "View.h"

IMPLEMENT_APP(Application)

Application::Application() :
	wxApp(),
	view(nullptr)
{

}

Application::~Application()
{
	delete view;
	view = nullptr;
}

bool Application::OnInit()
{
	view = new View;
	view->ShowFrame();

	return true;
}
