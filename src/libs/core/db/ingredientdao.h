#ifndef INGREDIENTDAO_H
#define INGREDIENTDAO_H
#include <vector>

class QString;

namespace Mamatkon {

struct Ingredient;
struct Product;
struct Measure;
struct Amount;
struct NutritionalData;

class IIngredientDao {
public:
	virtual ~IIngredientDao() {}
	virtual void addIngredientsToRecipe(long recipeId, ::std::vector<Ingredient> ingredients) = 0;
	virtual ::std::vector<Ingredient> getIngredientsForRecipeId(long recipeId) const = 0;
	virtual ::std::vector<Product> getProducts() const = 0;
	virtual ::std::vector<Measure> getMeasures() const = 0;
	virtual ::std::vector<Amount> getAmounts() const = 0;
	virtual ::std::vector<long>findRecipeIdsByIngredients(::std::vector<QString> ingredients) const = 0;
	virtual void deleteIngredient(long recipeId, int ingredientId) = 0;
	virtual int getMaxIdForRecipeId(long recipeId) const = 0;
	virtual void updateIngredient(long recipeId, const Ingredient& ingredient) = 0;
	virtual NutritionalData getNutritionalDataFor(const QString& iName) const = 0;
	virtual void insertNutritionalData(const QString& product, const NutritionalData& data) = 0;
	virtual float getRatioToGramFor(const QString& measure) const = 0;
};

}
#endif // INGREDIENTDAO_H
