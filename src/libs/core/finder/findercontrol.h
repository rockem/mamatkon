#ifndef FINDERCONTROL_H
#define FINDERCONTROL_H
#include <core/controlbase.h>
#include "recipefinder.h"

namespace Mamatkon { 

	class FinderControl : public ControlBase, public IRecipeFinder {

		std::vector<QString> convertStringToList(const QString &str, const QString& seperator = ",");
	public:
		FinderControl(EventManager& em) : ControlBase(em) {}

		void dispatchSearch(const QString& searchString, IRecipeFinder::SearchType searchType);
		void dispatchClearSearch();
	};

}
#endif // FINDERCONTROL_H