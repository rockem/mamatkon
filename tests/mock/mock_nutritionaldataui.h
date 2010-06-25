#ifndef MOCK_NUTRITIONALDATAUI_H
#define MOCK_NUTRITIONALDATAUI_H
#include <core/nuritionaldata/nutritionaldataui.h>

namespace MaTest { 

	class MockNutritionalDataUi : public Mamatkon::INutritionalDataUi {
	public:
		MOCK_METHOD1(setData, void(const ::std::map<Mamatkon::NutritionalData::Types, float>& data));
	};

}
#endif // MOCK_NUTRITIONALDATAUI_H