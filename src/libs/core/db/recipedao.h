#ifndef RECIPEDAO_H
#define RECIPEDAO_H
#include <core/db/recipe.h>
#include <vector>
#include <QString>

namespace Mamatkon {

class IRecipeDAO {
public:
    virtual ~IRecipeDAO() {}
    virtual const std::vector<Recipe> getAllRecipeTitles() const = 0;
	virtual Recipe getRecipeById(long id) const = 0;
	virtual long createNewRecipe() = 0;
	virtual void updateRecipe(const Recipe& recipe) = 0;
	virtual std::vector<long> findRecipeTitleAndInstructions(std::vector<QString> words) const = 0;
	virtual std::vector<long> findRecipeIdsByTags(std::vector<QString> tags) const = 0;
};

}
#endif // RECIPEDAO_H
