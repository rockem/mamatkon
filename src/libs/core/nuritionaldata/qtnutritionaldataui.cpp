#include "qtnutritionaldataui.h"
#include "ui_qtnutritionaldataui.h"

using namespace Mamatkon;

using ::std::map;

QtNutritionalDataUi::QtNutritionalDataUi(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::QtNutritionalDataUi) {
    m_ui->setupUi(this);
}

QtNutritionalDataUi::~QtNutritionalDataUi() {
    delete m_ui;
}

void QtNutritionalDataUi::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void QtNutritionalDataUi::setData(const ::std::map<NutritionalData::Types, float>& data) {
	m_ui->caloriesTxt->setText(getValueFrom(data, NutritionalData::Calories));
	m_ui->proteinTxt->setText(getValueFrom(data, NutritionalData::Protein));
	m_ui->carbohoydrateTxt->setText(getValueFrom(data, NutritionalData::Carbohydrate));
	m_ui->fatTxt->setText(getValueFrom(data, NutritionalData::Fat));
	m_ui->saturatedFatTxt->setText(getValueFrom(data, NutritionalData::SaturatedFat));
	m_ui->ironTxt->setText(getValueFrom(data, NutritionalData::Iron));
	m_ui->calciumTxt->setText(getValueFrom(data, NutritionalData::Calcium));
	m_ui->vitaminCTxt->setText(getValueFrom(data, NutritionalData::VitaminC));
	m_ui->potassiumTxt->setText(getValueFrom(data, NutritionalData::Potassium));
	m_ui->fiberTxt->setText(getValueFrom(data, NutritionalData::Fiber));
	m_ui->vitaminATxt->setText(getValueFrom(data, NutritionalData::VitaminA));
	m_ui->cholesterolTxt->setText(getValueFrom(data, NutritionalData::Cholesterol));
	m_ui->b12Txt->setText(getValueFrom(data, NutritionalData::B12));
	m_ui->sodiumTxt->setText(getValueFrom(data, NutritionalData::Sodium));
	m_ui->acidTxt->setText(getValueFrom(data, NutritionalData::PholicAcid));
	m_ui->omega3Txt->setText(getValueFrom(data, NutritionalData::Omega3));
}

QString QtNutritionalDataUi::getValueFrom( const ::std::map<NutritionalData::Types, float>& data, NutritionalData::Types type ) const {
	map<NutritionalData::Types, float>::const_iterator it = data.find(type);
	if(it != data.end()) {
		return QString::number(it->second);
	}
	return QString::number(0);
}