#include <QApplication>
#include <QTranslator>
#include <QSqlDatabase>
#include <iostream>
#include <mamatkon/ui/qtmainwindowui.h>
#include <stdio.h>
#include <mamatkon/ui/qtuifactory.h>
#include <mamatkon/control/recipelistcontrol.h>
#include <mamatkon/db/qtrecipedao.h>
#include <mamatkon/control/actionbuttonscontrol.h>
#include <mamatkon/command/commandcommander.h>
#include <mamatkon/serviceprovider.h>
#include <boost/shared_ptr.hpp>
#include <mamatkon/command/publishrecipecommand.h>
#include <mamatkon/control/servicefactory.h>

using Mamatkon::QtUiFactory;
using Mamatkon::QtMainWindowUi;
using Mamatkon::QtRecipeDao;
using Mamatkon::RecipeListControl;
using Mamatkon::CommandCommander;
using Mamatkon::ServicesProvider;
using ::boost::shared_ptr;
using Mamatkon::IServiceFactory;
using Mamatkon::PublishRecipeCommand;
using Mamatkon::ICommand;

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QTranslator translator;
    translator.load("../resource/mamatkon_he");
    // app.installTranslator(&translator);
    // app.setLayoutDirection(Qt::RightToLeft);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../mamatkon.db");
    bool ok = db.open();
    if(!ok) {
        qDebug("Error");
    }

    QtUiFactory uiFactory;
    CommandCommander commander;
    shared_ptr<IServiceFactory> services(new ServicesProvider(&uiFactory, &commander, db));

    commander.setServiceFactory(services);
    shared_ptr<ICommand> command = shared_ptr<ICommand>(new PublishRecipeCommand);
    commander.registerCommand(Mamatkon::PublishSelectedRecipe, command);

    uiFactory.showMainWindow();
    return app.exec();

}



