#ifndef COMMANDBUILDER_H
#define COMMANDBUILDER_H
#include <boost/shared_ptr.hpp>

class QSqlDatabase;

namespace Mamatkon {
	class ICommand;
}

extern struct Services;

class CommandBuilder {

	const Services& m_services;
public:
	CommandBuilder(const Services& services) : m_services(services) {}

	::boost::shared_ptr<Mamatkon::ICommand> createNewRecipeCommand();
};
#endif // COMMANDBUILDER_H