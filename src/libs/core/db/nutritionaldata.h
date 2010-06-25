#ifndef NUTRITIONALDATA_H
#define NUTRITIONALDATA_H
#include <QString>
#include <map>

namespace Mamatkon { 

	struct NutritionalData {
		enum Types {
			Calories,
			Protein,
			Carbohydrate,
			Fat,
			Iron,
			Calcium,
			VitaminC,
			Potassium,
			Fiber,
			VitaminA,
			Cholesterol,
			SaturatedFat,
			B12,
			Sodium,
			PholicAcid,
			Omega3
		};

		QString _Product;
		int _UnitWeight;

		/*float _Calories;
		float _Protein;
		float _Carbohydrate;
		float _Fat;
		float _Iron;
		float _Calcium;
		float _VitaminC;
		float _Potassium;
		float _Fiber;
		float _VitaminA;
		float _Cholesterol;
		float _SaturatedFat;
		float _B12;
		float _Sodium;
		float _PholicAcid;
		float _Omega3;*/

		::std::map<NutritionalData::Types, float> _NutritionalValues;

	public:
		NutritionalData() :  _UnitWeight(0) {
			_NutritionalValues[Calories    ] = 0;
			_NutritionalValues[Protein     ] = 0;
			_NutritionalValues[Carbohydrate] = 0;
			_NutritionalValues[Fat         ] = 0;
			_NutritionalValues[Iron        ] = 0;
			_NutritionalValues[Calcium     ] = 0;
			_NutritionalValues[VitaminC    ] = 0;
			_NutritionalValues[Potassium   ] = 0;
			_NutritionalValues[Fiber       ] = 0;
			_NutritionalValues[VitaminA    ] = 0;
			_NutritionalValues[Cholesterol ] = 0;
			_NutritionalValues[SaturatedFat] = 0;
			_NutritionalValues[B12         ] = 0;
			_NutritionalValues[Sodium      ] = 0;
			_NutritionalValues[PholicAcid  ] = 0;
			_NutritionalValues[Omega3      ] = 0;
		}
		/*NutritionalData() :
			_UnitWeight(0),
			_Calories(0),
			_Protein(0),
			_Carbohydrate(0),
			_Fat(0),
			_Iron(0),
			_Calcium(0),
			_VitaminC(0),
			_Potassium(0),
			_Fiber(0),
			_VitaminA(0),
			_Cholesterol(0),
			_SaturatedFat(0),
			_B12(0),
			_Sodium(0),
			_PholicAcid(0),
			_Omega3(0) {}*/

		NutritionalData(int unitWeight, ::std::map<NutritionalData::Types, float> values) :
		_UnitWeight(unitWeight), _NutritionalValues(values) {}
		NutritionalData(
			int unitWeight,
			float calories,
			float protein,
			float carbohydrate,
			float fat,
			float iron,
			float calcium,
			float vitaminc,
			float potassium,
			float fiber,
			float vitamina,
			float cholesterol,
			float saturatedFat,
			float b12,
			float sodium,
			float pholicacid,
			float omega3) :
		_UnitWeight(unitWeight) {
			_NutritionalValues[Calories    ] = calories;
			_NutritionalValues[Protein     ] = protein;
			_NutritionalValues[Carbohydrate] = carbohydrate;
			_NutritionalValues[Fat         ] = fat;
			_NutritionalValues[Iron        ] = iron;
			_NutritionalValues[Calcium     ] = calcium;
			_NutritionalValues[VitaminC    ] = vitaminc;
			_NutritionalValues[Potassium   ] = potassium;
			_NutritionalValues[Fiber       ] = fiber;
			_NutritionalValues[VitaminA    ] = vitamina;
			_NutritionalValues[Cholesterol ] = cholesterol;
			_NutritionalValues[SaturatedFat] = saturatedFat;
			_NutritionalValues[B12         ] = b12;
			_NutritionalValues[Sodium      ] = sodium;
			_NutritionalValues[PholicAcid  ] = pholicacid;
			_NutritionalValues[Omega3      ] = omega3;

		}

		bool operator==(const NutritionalData& other) const {
			return _UnitWeight == other._UnitWeight &&
				_NutritionalValues == other._NutritionalValues;
		}
			/*_Calories(calories),
			_Protein(protein),
			_Carbohydrate(carbohydrate),
			_Fat(fat),
			_Iron(iron),
			_Calcium(calcium),
			_VitaminC(vitaminc),
			_Potassium(potassium),
			_Fiber(fiber),
			_VitaminA(vitamina),
			_Cholesterol(cholesterol),
			_SaturatedFat(saturatedFat),
			_B12(b12),
			_Sodium(sodium),
			_PholicAcid(pholicacid),
			_Omega3(omega3) {}*/
	};

}
#endif // NUTRITIONALDATA_H