#include "dynfilterform.h"
#include "ui_dynfilterform.h"

DynFilterForm::DynFilterForm(EffectMgr* effect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DynFilterForm),
    _effect(effect)
{
    ui->setupUi(this);
}

DynFilterForm::~DynFilterForm()
{
    delete ui;
}
