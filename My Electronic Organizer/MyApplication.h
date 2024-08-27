
#include <wx/app.h>

class View;

class Application : public wxApp
{
private:
	View *view;

public:
	Application();
	~Application();

	bool OnInit();
};
