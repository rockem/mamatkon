#include <QApplication>
#include <QTranslator>
#include <QSqlDatabase>
#include <core/ui/qtmainwindowui.h>
#include <core/ui/qtuifactory.h>
#include <core/recipelist/recipelistcontrol.h>
#include <core/db/qtrecipedao.h>
#include <core/actionbuttons/actionbuttonscontrol.h>
#include <boost/shared_ptr.hpp>
#include <core/db/qtdbutils.h>
#include <core/recipegrabber/recipegrabbercontrol.h>
#include <core/db/qtingredientdao.h>
#include <core/recipeevent.h>
#include <core/recipeedit/recipeeditcontrol.h>
#include <QCleanlooksStyle>
#include <core/finder/findercontrol.h>
#include <core/nuritionaldata/nutritionaldatacontrol.h>

using namespace Mamatkon;
using ::boost::shared_ptr;


void initComponents(EventManager& em, QSqlDatabase& db) {
	QtRecipeDao dao(db);
	long id = dao.getFirstRecipeId();

	if(id != 0) {
		em.raiseEvent(shared_ptr<Event>(new RecipeEvent(Event::RecipeListChanged, 0)));
		em.raiseEvent(shared_ptr<Event>(new RecipeEvent(Event::SelectedRecipeChanged, id)));
	}
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(qtres);
    Q_INIT_RESOURCE(maimage);

    qApp->addLibraryPath("../plugins");

    QTranslator translator;
    translator.load("../resource/mamtkon_he");
    app.installTranslator(&translator);
    app.setLayoutDirection(Qt::RightToLeft);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../mamatkon.db");
    bool ok = db.open();
    if(!ok) {
        qDebug("Error");
		return 1;
    }

	QtDbUtils::createSchemaOn(db);
	QtDbUtils::fillInitialData(db);

	QtUiFactory uiFactory;
    EventManager eventManager;

    ActionButtonsControl abc(eventManager);
    abc.setUi(uiFactory.getActionButtonsUi());
    abc.setRecipeDao(::boost::shared_ptr<IRecipeDAO>(new QtRecipeDao(db)));

    RecipeListControl rlc(eventManager);
    rlc.setUi(uiFactory.getRecipeListUi());
    rlc.setRecipeDao(::boost::shared_ptr<IRecipeDAO>(new QtRecipeDao(db)));

	RecipeGrabberControl rgc(eventManager);
	rgc.setUi(uiFactory.getRecipeGrabberUi());
	rgc.setRecipeDao(::boost::shared_ptr<IRecipeDAO>(new QtRecipeDao(db)));
	rgc.setIngredientDao(::boost::shared_ptr<IIngredientDao>(new QtIngredientDao(db)));

	RecipeEditControl rec(eventManager);
	rec.setUi(uiFactory.getRecipeEditUi());
	rec.setRecipeDao(::boost::shared_ptr<IRecipeDAO>(new QtRecipeDao(db)));
	rec.setIngredientDao(::boost::shared_ptr<IIngredientDao>(new QtIngredientDao(db)));

	FinderControl fc(eventManager);
	fc.setUi(uiFactory.getFinderUi());
	fc.setRecipeDao(::boost::shared_ptr<IRecipeDAO>(new QtRecipeDao(db)));
	fc.setIngredientDao(::boost::shared_ptr<IIngredientDao>(new QtIngredientDao(db)));

	NutritionalDataControl ndc(eventManager);
	ndc.setUi(uiFactory.getNutritionalDataUi());
	ndc.setRecipeDao(::boost::shared_ptr<IRecipeDAO>(new QtRecipeDao(db)));
	ndc.setIngredientDao(::boost::shared_ptr<IIngredientDao>(new QtIngredientDao(db)));

    initComponents(eventManager, db);
    uiFactory.showMainWindow();
    return app.exec();

}



