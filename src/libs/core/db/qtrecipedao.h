#ifndef _QTRECIPEDAO_H
#define _QTRECIPEDAO_H
#include <core/db/recipedao.h>
#include <QSqlDatabase>
#include <core/db/qtbasedao.h>

namespace Mamatkon {

class QtRecipeDao : private QtBaseDao, public IRecipeDAO {

    // QSqlDatabase& m_database;

    Recipe createRecipeFrom(QSqlQuery& query) const;
public:
    QtRecipeDao(QSqlDatabase & db) : QtBaseDao(db) {}

	//from IRecipeDAO
    const std::vector<Recipe> getAllRecipeTitles() const;
	Recipe getRecipeById(long id) const;
	long createNewRecipe();
	long getFirstRecipeId() const;
	void updateRecipe(const Recipe& recipe);
	std::vector<long> findRecipeTitleAndInstructions(std::vector<QString> words) const;
	std::vector<long> findRecipeIdsByTags(std::vector<QString> tags) const;
	
};

}
#endif
