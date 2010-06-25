#ifndef CONTROLBASE_H
#define CONTROLBASE_H
#include <core/eventlistener.h>
#include <core/ui/uibase.h>
#include <boost/shared_ptr.hpp>
#include <core/db/recipedao.h>
#include <core/eventmanager.h>
#include <core/db/ingredientdao.h>

namespace Mamatkon {

class CommandCommander;

class ControlBase : public IEventListener {

	IUi* m_ui;
	::boost::shared_ptr<IRecipeDAO> m_recipeDao;
	::boost::shared_ptr<IIngredientDao> m_ingredientDao;
	EventManager& m_eventManager;
	
public:
	virtual ~ControlBase() {}
	void setUi(IUi* ui) { 
		m_ui = ui; 
		m_ui->setEventListener(this);
	}
	void setRecipeDao(::boost::shared_ptr<IRecipeDAO> dao) {
		m_recipeDao = dao;
	}
	void setIngredientDao(::boost::shared_ptr<IIngredientDao> dao) {
		m_ingredientDao = dao;
	}
	
protected:
	ControlBase(EventManager& em) : m_eventManager(em) {}
	IUi* getUi() const { return m_ui; }
	::boost::shared_ptr<IRecipeDAO> recipeDao() const { return m_recipeDao; }
	::boost::shared_ptr<IIngredientDao> ingredientDao() const { return m_ingredientDao; }
	EventManager& eventManager() const { return m_eventManager; }
};

}
#endif // CONTROLBASE_H
