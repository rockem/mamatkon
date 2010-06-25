#ifndef NUTRITIONALDATAUI_H
#define NUTRITIONALDATAUI_H
#include <core/ui/uibase.h>
#include <core/db/nutritionaldata.h>
#include <map>

namespace Mamatkon { 

	struct RecipeNutritionalData;

	class INutritionalDataUi : public UiBase {
	public:
		virtual ~INutritionalDataUi() {}
		virtual void setData(const ::std::map<NutritionalData::Types, float>& data) = 0;
	};
}
#endif // NUTRITIONALDATAUI_H