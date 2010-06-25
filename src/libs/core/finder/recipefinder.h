#ifndef RECIPEFINDER_H
#define RECIPEFINDER_H

namespace Mamatkon { 

	class IRecipeFinder {
	public:
		enum SearchType {TitleInstructions, Ingredients, Tags};
		virtual ~IRecipeFinder() {};

		virtual void dispatchSearch(const QString& searchString, IRecipeFinder::SearchType searchType) = 0;
		virtual void dispatchClearSearch() = 0;
	};

}
#endif // RECIPEFINDER_H