
#include "Presenter.h"
#include "Model.h"

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
