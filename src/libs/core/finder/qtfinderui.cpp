#include "qtfinderui.h"
#include "ui_qtfinderui.h"
#include <boost/assign.hpp>

namespace Mamatkon {

using ::std::map;
using boost::assign::map_list_of;

map<int, IRecipeFinder::SearchType> QtFinderUi::m_intToSearchType = map_list_of 
(0, IRecipeFinder::TitleInstructions)
(1, IRecipeFinder::Ingredients)
(2, IRecipeFinder::Tags);


QtFinderUi::QtFinderUi(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::QtFinderUi) {
    m_ui->setupUi(this);
	connect(m_ui->m_findBtn, SIGNAL(clicked()), this, SLOT(onFind()));
	connect(m_ui->m_clearButton, SIGNAL(clicked()), this, SLOT(onClear()));
}

QtFinderUi::~QtFinderUi() {
    delete m_ui;
}

void QtFinderUi::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void QtFinderUi::onFind() {
	QString searchStr = m_ui->m_searchText->text();
	int option = m_ui->m_searchOption->currentIndex();
	if(!searchStr.isEmpty()) {
		dynamic_cast<IRecipeFinder*>(eventListener())->dispatchSearch(searchStr, m_intToSearchType[option]);
	}
}

void QtFinderUi::onClear() {
	m_ui->m_searchText->setText("");
	dynamic_cast<IRecipeFinder*>(eventListener())->dispatchClearSearch();
}

}