#ifndef MOCK_RECIPEDAO_H
#define MOCK_RECIPEDAO_H
#include <gmock/gmock.h>
#include <core/db/recipedao.h>

namespace MaTest {

class MockRecipeDAO : public Mamatkon::IRecipeDAO {
public:
	MOCK_CONST_METHOD0(getAllRecipeTitles, const std::vector<Mamatkon::Recipe>());
	MOCK_CONST_METHOD1(getRecipeById, Mamatkon::Recipe(long id));
	MOCK_METHOD0(createNewRecipe, long());
	MOCK_METHOD1(updateRecipe, void(const Mamatkon::Recipe& recipe));
	MOCK_CONST_METHOD1(findRecipeTitleAndInstructions, std::vector<long>(std::vector<QString> words));
	MOCK_CONST_METHOD1(findRecipeIdsByTags, std::vector<long>(std::vector<QString> tags));
};

}

#endif // MOCK_RECIPEDAO_H