#ifndef SERVERCOMMSERVICE_H
#define SERVERCOMMSERVICE_H
#include <core/eventlistener.h>

namespace Mamatkon {

struct Recipe; 

struct ICommService : TargetEventListener {
	virtual ~ICommService() {}
	virtual void sendRecipe(const Recipe& recipe) = 0;
};

}
#endif // SERVERCOMMSERVICE_H
