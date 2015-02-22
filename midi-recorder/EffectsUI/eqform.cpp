#include "eqform.h"
#include "ui_eqform.h"

EQForm::EQForm(EffectMgr* effect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EQForm),
    _effect(effect)
{
    ui->setupUi(this);
}

EQForm::~EQForm()
{
    delete ui;
}
