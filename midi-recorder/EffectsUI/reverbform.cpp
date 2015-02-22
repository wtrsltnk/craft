#include "reverbform.h"
#include "ui_reverbform.h"

ReverbForm::ReverbForm(EffectMgr* effect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReverbForm),
    _effect(effect)
{
    ui->setupUi(this);
}

ReverbForm::~ReverbForm()
{
    delete ui;
}
