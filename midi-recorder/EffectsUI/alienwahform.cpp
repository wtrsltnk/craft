#include "alienwahform.h"
#include "ui_alienwahform.h"
#include "Effects/Alienwah.h"

AlienWahForm::AlienWahForm(EffectMgr* effect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlienWahForm),
    _effect(effect)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Tool | Qt::MSWindowsFixedSizeDialogHint);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    this->ui->cmbLFOType->addItem("Sine", QVariant(0));
    this->ui->cmbLFOType->addItem("Triangle", QVariant(1));

    this->EffectToUi();

    connect(this->ui->dialDelay, SIGNAL(valueChanged(int)), this, SLOT(OnDialChanged(int)));
    connect(this->ui->dialDepth, SIGNAL(valueChanged(int)), this, SLOT(OnDialChanged(int)));
    connect(this->ui->dialFeedback, SIGNAL(valueChanged(int)), this, SLOT(OnDialChanged(int)));
    connect(this->ui->dialLFOFrequency, SIGNAL(valueChanged(int)), this, SLOT(OnDialChanged(int)));
    connect(this->ui->dialLFORandomness, SIGNAL(valueChanged(int)), this, SLOT(OnDialChanged(int)));
    connect(this->ui->dialLFOStereo, SIGNAL(valueChanged(int)), this, SLOT(OnDialChanged(int)));
    connect(this->ui->dialLRCrossover, SIGNAL(valueChanged(int)), this, SLOT(OnDialChanged(int)));
    connect(this->ui->dialPan, SIGNAL(valueChanged(int)), this, SLOT(OnDialChanged(int)));
    connect(this->ui->dialPhase, SIGNAL(valueChanged(int)), this, SLOT(OnDialChanged(int)));
    connect(this->ui->dialVolume, SIGNAL(valueChanged(int)), this, SLOT(OnDialChanged(int)));

    connect(this->ui->cmbLFOType, SIGNAL(currentIndexChanged(int)), this, SLOT(OnComboboxChanged(int)));
}

AlienWahForm::~AlienWahForm()
{
    delete ui;
}

void AlienWahForm::EffectToUi()
{
    this->ui->dialVolume->setProperty("parindex", QVariant(AlienwahParameters::Volume));
    this->ui->dialVolume->setValue(this->_effect->geteffectpar(AlienwahParameters::Volume));

    this->ui->dialPan->setProperty("parindex", QVariant(AlienwahParameters::Panning));
    this->ui->dialPan->setValue(this->_effect->geteffectpar(AlienwahParameters::Panning));

    this->ui->dialDelay->setProperty("parindex", QVariant(AlienwahParameters::Delay));
    this->ui->dialDelay->setValue(this->_effect->geteffectpar(AlienwahParameters::Delay));

    this->ui->dialDepth->setProperty("parindex", QVariant(AlienwahParameters::Depth));
    this->ui->dialDepth->setValue(this->_effect->geteffectpar(AlienwahParameters::Depth));

    this->ui->dialLRCrossover->setProperty("parindex", QVariant(AlienwahParameters::LRCrossover));
    this->ui->dialLRCrossover->setValue(this->_effect->geteffectpar(AlienwahParameters::LRCrossover));

    this->ui->dialFeedback->setProperty("parindex", QVariant(AlienwahParameters::Feedback));
    this->ui->dialFeedback->setValue(this->_effect->geteffectpar(AlienwahParameters::Feedback));

    this->ui->dialLFOFrequency->setProperty("parindex", QVariant(AlienwahParameters::LFOFrequency));
    this->ui->dialLFOFrequency->setValue(this->_effect->geteffectpar(AlienwahParameters::LFOFrequency));

    this->ui->dialLFORandomness->setProperty("parindex", QVariant(AlienwahParameters::LFORandomness));
    this->ui->dialLFORandomness->setValue(this->_effect->geteffectpar(AlienwahParameters::LFORandomness));

    this->ui->dialLFOStereo->setProperty("parindex", QVariant(AlienwahParameters::LFOStereo));
    this->ui->dialLFOStereo->setValue(this->_effect->geteffectpar(AlienwahParameters::LFOStereo));

    this->ui->dialLFOFrequency->setProperty("parindex", QVariant(AlienwahParameters::LFOFrequency));
    this->ui->dialLFOFrequency->setValue(this->_effect->geteffectpar(AlienwahParameters::LFOFrequency));

    this->ui->cmbLFOType->setProperty("parindex", QVariant(AlienwahParameters::LFOType));
    this->ui->cmbLFOType->setCurrentIndex(this->_effect->geteffectpar(AlienwahParameters::LFOType));
}

void AlienWahForm::OnDialChanged(int value)
{
    QDial* dial = (QDial*)this->sender();
    int parindex = dial->property("parindex").toInt();
    this->_effect->seteffectpar(parindex, dial->value());
}

void AlienWahForm::OnComboboxChanged(int value)
{
    QComboBox* cmb = (QComboBox*)this->sender();
    int parindex = cmb->property("parindex").toInt();
    this->_effect->seteffectpar(parindex, cmb->currentIndex());
}
