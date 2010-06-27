#ifndef RECIPEEDITCONTROL_H
#define RECIPEEDITCONTROL_H
#include <core/controlbase.h>
#include <boost/shared_ptr.hpp>
#include <core/recipeupdater.h>

class QVariant;

namespace Mamatkon {

class IIngredientDao;
struct Ingredient;
class IngredientItem;

class RecipeEditControl: public ControlBase, public IRecipeUpdater
{
	long m_currentRecipeId;
	QString m_scaleRatio;

	void onRecipeSelectionChanged(::boost::shared_ptr<Event> e);
	void updateUiIngredientsData();

	void addAlternateIngrridientsToTree(::std::vector<Ingredient> &ingredients,
			::boost::shared_ptr<IngredientItem> rootItem);
	void addIngridientsToTree(::std::vector<Ingredient> &ingredients,
			::boost::shared_ptr<IngredientItem> &rootItem);
	void addIngrerdientRowTo(::std::vector<Ingredient>::const_iterator it,
			IngredientItem* rootItem);
	void updateUiRecipeData();
	QList<QVariant> createIngredientsHeader();
	QString scaleAmount(const QString& amount);

public:
	RecipeEditControl(EventManager& em);

	void dispatchRecipeUpdate(const Recipe& recipe);
	void dispatchDeleteIngredient(long id);
	void dispatchIngredientsUpdate(IngredientItem* item);
};

}
#endif // RECIPEEDITCONTROL_H
