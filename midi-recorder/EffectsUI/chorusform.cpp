#include "chorusform.h"
#include "ui_chorusform.h"

ChorusForm::ChorusForm(EffectMgr* effect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChorusForm),
    _effect(effect)
{
    ui->setupUi(this);
}

ChorusForm::~ChorusForm()
{
    delete ui;
}
