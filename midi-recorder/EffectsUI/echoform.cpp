#include "echoform.h"
#include "ui_echoform.h"
#include "Effects/Echo.h"

EchoForm::EchoForm(EffectMgr* effect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EchoForm),
    _effect(effect)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Tool | Qt::MSWindowsFixedSizeDialogHint);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    this->EffectToUi();

    connect(this->ui->dialDampening, SIGNAL(valueChanged(int)), this, SLOT(OnDialChanged(int)));
    connect(this->ui->dialDelay, SIGNAL(valueChanged(int)), this, SLOT(OnDialChanged(int)));
    connect(this->ui->dialFeedback, SIGNAL(valueChanged(int)), this, SLOT(OnDialChanged(int)));
    connect(this->ui->dialLRCrossover, SIGNAL(valueChanged(int)), this, SLOT(OnDialChanged(int)));
    connect(this->ui->dialLRDelay, SIGNAL(valueChanged(int)), this, SLOT(OnDialChanged(int)));
    connect(this->ui->dialPan, SIGNAL(valueChanged(int)), this, SLOT(OnDialChanged(int)));
    connect(this->ui->dialVolume, SIGNAL(valueChanged(int)), this, SLOT(OnDialChanged(int)));
}

EchoForm::~EchoForm()
{
    delete ui;
}

void EchoForm::EffectToUi()
{
    this->ui->dialVolume->setProperty("parindex", QVariant(EchoParameters::Volume));
    this->ui->dialVolume->setValue(this->_effect->geteffectpar(EchoParameters::Volume));

    this->ui->dialPan->setProperty("parindex", QVariant(EchoParameters::Panning));
    this->ui->dialPan->setValue(this->_effect->geteffectpar(EchoParameters::Panning));

    this->ui->dialDelay->setProperty("parindex", QVariant(EchoParameters::Delay));
    this->ui->dialDelay->setValue(this->_effect->geteffectpar(EchoParameters::Delay));

    this->ui->dialLRDelay->setProperty("parindex", QVariant(EchoParameters::LRDelay));
    this->ui->dialLRDelay->setValue(this->_effect->geteffectpar(EchoParameters::LRDelay));

    this->ui->dialLRCrossover->setProperty("parindex", QVariant(EchoParameters::LRCrossover));
    this->ui->dialLRCrossover->setValue(this->_effect->geteffectpar(EchoParameters::LRCrossover));

    this->ui->dialFeedback->setProperty("parindex", QVariant(EchoParameters::Feedback));
    this->ui->dialFeedback->setValue(this->_effect->geteffectpar(EchoParameters::Feedback));

    this->ui->dialDampening->setProperty("parindex", QVariant(EchoParameters::Dampening));
    this->ui->dialDampening->setValue(this->_effect->geteffectpar(EchoParameters::Dampening));
}

void EchoForm::OnDialChanged(int value)
{
    QDial* dial = (QDial*)this->sender();
    int parindex = dial->property("parindex").toInt();
    this->_effect->seteffectpar(parindex, dial->value());
}
