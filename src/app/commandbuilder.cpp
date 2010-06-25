#include "commandbuilder.h"
#include <core/command/commandcommander.h>
#include <core/command/newrecipecommand.h>
#include <core/db/qtrecipedao.h>

using ::boost::shared_ptr;
using Mamatkon::ICommand;
using Mamatkon::NewRecipeCommand;
using Mamatkon::QtRecipeDao;
using Mamatkon::IRecipeDAO;

shared_ptr<ICommand> CommandBuilder::createNewRecipeCommand() {
	shared_ptr<NewRecipeCommand> command = shared_ptr<NewRecipeCommand>(new NewRecipeCommand);
	// command->setRecipeDao(shared_ptr<IRecipeDAO>(new QtRecipeDao(m_db)));
	return command;
}