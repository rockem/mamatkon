#include "qtrecipegrabberui.h"
#include "ui_qtrecipegrabberui.h"
#include <core/eventlistener.h>
#include <QTextCursor>

namespace Mamatkon {

QtRecipeGrabberUi::QtRecipeGrabberUi(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::QtRecipeGrabberUi)
{
    m_ui->setupUi(this);
	setLayoutDirection(QApplication::layoutDirection());

	modifyTextEditForRTLSupport();

	QObject::connect(m_ui->titleButton, SIGNAL(clicked()), this, SLOT(grabTitle()));
	QObject::connect(m_ui->ingredientsButton, SIGNAL(clicked()), this, SLOT(grabIngredients()));
	QObject::connect(m_ui->instructionsButton, SIGNAL(clicked()), this, SLOT(grabInstructions()));
	QObject::connect(m_ui->sourceButton, SIGNAL(clicked()), this, SLOT(grabSource()));
	QObject::connect(m_ui->servingsButton, SIGNAL(clicked()), this, SLOT(grabServings()));
}

QtRecipeGrabberUi::~QtRecipeGrabberUi() {
    delete m_ui;
}

void QtRecipeGrabberUi::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void QtRecipeGrabberUi::grabTitle() {
	dispatchGrabFor(IRecipeGrabberEventListener::Title);

}

void QtRecipeGrabberUi::grabIngredients() {
	dispatchGrabFor(IRecipeGrabberEventListener::Ingredients);
}

void QtRecipeGrabberUi::grabInstructions() {
	dispatchGrabFor(IRecipeGrabberEventListener::Instructions);
}

void QtRecipeGrabberUi::grabSource() {
	dispatchGrabFor(IRecipeGrabberEventListener::Source);
}

void QtRecipeGrabberUi::modifyTextEditForRTLSupport() {
	QTextOption o = m_ui->textEdit->document()->defaultTextOption();
	o.setAlignment(Qt::AlignRight);
	o.setTextDirection(Qt::RightToLeft);
	m_ui->textEdit->document()->setDefaultTextOption(o);

	/*QTextCursor tc = m_ui->textEdit->textCursor();
	QTextCharFormat tcf = tc.blockCharFormat();

	tcf.setLayoutDirection( Qt::RightToLeft );
	tc.setBlockCharFormat(tcf);
	tc.insertText( QLatin1String("2) Wrong direction on QTextEdit, wanted RTL!"), tcf );
	m_ui->textEdit->setTextCursor(tc);*/


	QTextCursor tc = m_ui->textEdit->textCursor();
	QTextBlockFormat tbf = tc.blockFormat();

	tbf.setLayoutDirection( Qt::RightToLeft );
	tc.setBlockFormat(tbf);
	m_ui->textEdit->setTextCursor(tc);
}

void QtRecipeGrabberUi::dispatchGrabFor(IRecipeGrabberEventListener::RecipeSection section) {
	QString text = m_ui->textEdit->textCursor().selectedText();
	text = text.replace(QChar::ParagraphSeparator, "\n");
	dynamic_cast<IRecipeGrabberEventListener*>(
		eventListener())->dispatchGrab(section, text);
}

void QtRecipeGrabberUi::grabServings() {
	dispatchGrabFor(IRecipeGrabberEventListener::Servings);
}
}