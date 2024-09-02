
#include "Presenter.h"
#include "Model.h"
#include "View.h"

Presenter::Presenter(View* view) :
	view(view)
{
	model = new Model;

}

Presenter::~Presenter()
{
	delete model;
	model = nullptr;
}

void Presenter::OnAlarmtTime()
{
	auto alarms = model->GetAlarms();

	view->SetAlarmClock(alarms);
}
