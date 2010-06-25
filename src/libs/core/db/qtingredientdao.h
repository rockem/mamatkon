#ifndef QTINGREDIENTDAO_H
#define QTINGREDIENTDAO_H
#include <core/db/ingredientdao.h>
#include <core/db/qtbasedao.h>
#include <vector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <core/db/nutritionaldata.h>

namespace Mamatkon {

	class QtIngredientDao : private QtBaseDao, public IIngredientDao {
		
		QString getValueFor(const ::std::map<NutritionalData::Types, float>& data, NutritionalData::Types type ) const;
	public:
		QtIngredientDao(QSqlDatabase& db) : QtBaseDao(db) {}

		void addIngredientsToRecipe(long recipeId, ::std::vector<Ingredient> ingredients);
		::std::vector<Ingredient> getIngredientsForRecipeId(long recipeId) const;
		::std::vector<Product> getProducts() const;
		::std::vector<Measure> getMeasures() const;
		::std::vector<Amount> getAmounts() const;
		::std::vector<long> findRecipeIdsByIngredients(::std::vector<QString> ingredients) const;
		void deleteIngredient(long recipeId, int ingredientId);
		int getMaxIdForRecipeId(long recipeId) const;
		void updateIngredient(long recipeId, const Ingredient& ingredient);
		NutritionalData getNutritionalDataFor(const QString& iName) const;
		void insertNutritionalData(const QString& product, const NutritionalData& data);
		float getRatioToGramFor(const QString& measure) const;
	};

}
#endif // QTINGREDIENTDAO_H