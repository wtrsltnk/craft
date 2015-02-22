#include "distorsionform.h"
#include "ui_distorsionform.h"

DistorsionForm::DistorsionForm(EffectMgr* effect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DistorsionForm),
    _effect(effect)
{
    ui->setupUi(this);
}

DistorsionForm::~DistorsionForm()
{
    delete ui;
}
