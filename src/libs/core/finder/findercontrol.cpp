#include "findercontrol.h"
#include <core/recipesearchevent.h>
#include <QStringList>

using namespace Mamatkon;

using ::boost::shared_ptr;
using ::std::vector;

void FinderControl::dispatchSearch(const QString& searchString, IRecipeFinder::SearchType searchType) {
	vector<long> ids;
	switch(searchType) {
		case IRecipeFinder::TitleInstructions:
			ids = recipeDao()->findRecipeTitleAndInstructions(convertStringToList(searchString, " "));
			break;
		case IRecipeFinder::Ingredients:
			ids = ingredientDao()->findRecipeIdsByIngredients(convertStringToList(searchString));
			break;
		case IRecipeFinder::Tags:
			ids = recipeDao()->findRecipeIdsByTags(convertStringToList(searchString));
			break; 
	}
	eventManager().raiseEvent(shared_ptr<RecipeSearchEvent>(new RecipeSearchEvent(ids)));
}

std::vector<QString> FinderControl::convertStringToList(const QString &str, const QString& seperator)  {
	QStringList parts = str.split(seperator, QString::SkipEmptyParts);
	vector<QString> newList;
	for (int i = 0; i < parts.size(); ++i) {
		newList.push_back(parts[i].trimmed());
	}
	return newList;
}

void Mamatkon::FinderControl::dispatchClearSearch() {
	eventManager().raiseEvent(shared_ptr<Event>(new Event(Event::RecipeSearchClear)));
}