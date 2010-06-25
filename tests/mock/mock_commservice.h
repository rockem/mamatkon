#ifndef MOCK_COMMSERVICE_H
#define MOCK_COMMSERVICE_H
#include <core/db/recipe.h>
#include <core/control/commservice.h>

namespace MaTest {

class MockCommService : public Mamatkon::ICommService {
public:
	MOCK_METHOD1(sendRecipe, void(const Mamatkon::Recipe& recipe));
};

}

#endif // MOCK_COMMSERVICE_H