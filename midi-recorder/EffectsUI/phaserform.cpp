#include "phaserform.h"
#include "ui_phaserform.h"

PhaserForm::PhaserForm(EffectMgr* effect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhaserForm),
    _effect(effect)
{
    ui->setupUi(this);
}

PhaserForm::~PhaserForm()
{
    delete ui;
}
