#ifndef NUTRITIONALDATACONTROL_H
#define NUTRITIONALDATACONTROL_H
#include <core/controlbase.h>


namespace Mamatkon { 

	class NutritionalDataControl : public ControlBase {
		
	void onRecipeSelectionChanged(::boost::shared_ptr<Event> e);
	public:
		NutritionalDataControl(EventManager& em);
	};

}
#endif // NUTRITIONALDATACONTROL_H