#include "mastermixerwidget.h"
#include "ui_mastermixerwidget.h"
#include "Nio/InMgr.h"
#include "Nio/MidiIn.h"
#include "Nio/RtEngine.h"
#include "Effects/EffectMgr.h"
#include "EffectsUI/alienwahform.h"
#include "EffectsUI/chorusform.h"
#include "EffectsUI/distorsionform.h"
#include "EffectsUI/dynfilterform.h"
#include "EffectsUI/echoform.h"
#include "EffectsUI/eqform.h"
#include "EffectsUI/phaserform.h"
#include "EffectsUI/reverbform.h"
#include <QMenu>
#include <QTimer>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>

MasterMixerWidget::MasterMixerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MasterMixerWidget),
    _master(0)
{
    ui->setupUi(this);

    if (InMgr::getInstance().GetCurrent() != 0)
    {
        std::vector<std::string> v = InMgr::getInstance().GetCurrent()->GetPorts();
        if (v.size() > 0)
        {
            QMenu *menu = new QMenu();
            connect(menu ,SIGNAL(triggered(QAction*)), this, SLOT(OnChangeInput(QAction*)));
            for (std::vector<std::string>::iterator i = v.begin(); i != v.end(); ++i)
            {
                QAction *a = new QAction(QString::fromStdString(*i), this);
                menu->addAction(a);
            }
            this->ui->btnChangeInput->setText(QString::fromStdString(v[0]));
            this->ui->btnChangeInput->setMenu(menu);
        }
    }

    QMenu* effects = new QMenu();
    connect(effects ,SIGNAL(triggered(QAction*)), this, SLOT(OnAddEffect(QAction*)));

    for (int i = 1; i < 9; i++)
    {
        QAction* a = new QAction(QString(EffectMgr::FilterTitles[i]), this);
        a->setData(QVariant(i));
        effects->addAction(a);
    }

    this->ui->btnAddEffect->setMenu(effects);

    connect(this->ui->sysmaster, SIGNAL(valueChanged(int)), this, SLOT(SetVolume(int)));

    this->ui->sysvuL->installEventFilter(this);
    this->ui->sysvuR->installEventFilter(this);
}

MasterMixerWidget::~MasterMixerWidget()
{
    delete ui;
}


void MasterMixerWidget::SetMaster(Master* master)
{
    if (this->_master != master)
    {
        this->_master = master;

        this->ui->sysmaster->setValue(this->_master->Pvolume);

        this->_vutimer = new QTimer();
        this->_vutimer->setInterval(1000/40);
        this->_vutimer->start();
        connect(this->_vutimer, SIGNAL(timeout()), this, SLOT(OnVuTimer()));
    }
}

bool MasterMixerWidget::eventFilter(QObject* watched, QEvent* event)
{
    QBrush highlight = this->palette().highlight();
    QBrush bg = this->palette().shadow();
    QBrush shadow = this->palette().midlight();

    highlight.setStyle(Qt::Dense1Pattern);

    if (watched == ui->sysvuL && event->type() == QEvent::Paint)
    {
        int lx = ui->sysvuL->width() * 1.0; int ly = ui->sysvuL->height();

        int idbl=(int) (this->dbl*ly);
        int irmsdbl=(int) (this->rmsdbl*ly);

        QPainter painter;
        painter.begin(ui->sysvuL);

        painter.fillRect(ui->sysvuL->width() * 0.0,ly-idbl,lx,idbl, highlight);
        painter.fillRect(ui->sysvuL->width() * 0.0,0,lx,ly-idbl, bg);
        painter.fillRect(ui->sysvuL->width() * 0.0,ly-irmsdbl-1,lx,3, shadow);

        painter.end();
        return true;
    }
    if (watched == ui->sysvuR && event->type() == QEvent::Paint)
    {
        int lx=ui->sysvuR->width() * 1.0; int ly=ui->sysvuR->height();

        int idbr=(int) (this->dbr*ly);
        int irmsdbr=(int) (this->rmsdbr*ly);

        QPainter painter;
        painter.begin(ui->sysvuR);

        painter.fillRect(ui->sysvuR->width() * 0.0,ly-idbr,lx,idbr, highlight);
        painter.fillRect(ui->sysvuR->width() * 0.0,0,lx,ly-idbr, bg);
        painter.fillRect(ui->sysvuR->width() * 0.0,ly-irmsdbr-1,lx,3, shadow);

        painter.end();
        return true;
    }
    return false;
}

void MasterMixerWidget::OnVuTimer()
{
    // This part is copied from MasterUI.fl by Nasca Octavian Paul
    static float olddbl = 0;
    static float olddbr = 0;
    static float oldrmsdbl = 0;
    static float oldrmsdbr = 0;
#define MIN_DB (-48)

    vuData data = this->_master->getVuData();

    this->dbl=rap2dB(data.outpeakl);
    this->dbr=rap2dB(data.outpeakr);
    this->dbl=(MIN_DB-this->dbl)/MIN_DB;
    if (this->dbl<0.0) this->dbl=0.0;
    else if (this->dbl>1.0)this->dbl=1.0;

    this->dbr=(MIN_DB-this->dbr)/MIN_DB;
    if (this->dbr<0.0) this->dbr=0.0;
    else if (this->dbr>1.0) this->dbr=1.0;

    this->dbl=this->dbl*0.4+olddbl*0.6;
    this->dbr=this->dbr*0.4+olddbr*0.6;

    olddbl=this->dbl;
    olddbr=this->dbr;

    //compute RMS - start
    this->rmsdbl=rap2dB(data.rmspeakl);
    this->rmsdbr=rap2dB(data.rmspeakr);
    this->rmsdbl=(MIN_DB-this->rmsdbl)/MIN_DB;
    if (this->rmsdbl<0.0) this->rmsdbl=0.0;
    else if (this->rmsdbl>1.0) this->rmsdbl=1.0;

    this->rmsdbr=(MIN_DB-this->rmsdbr)/MIN_DB;
    if (this->rmsdbr<0.0) this->rmsdbr=0.0;
    else if (this->rmsdbr>1.0) this->rmsdbr=1.0;

    this->rmsdbl=this->rmsdbl*0.4+oldrmsdbl*0.6;
    this->rmsdbr=this->rmsdbr*0.4+oldrmsdbr*0.6;

    oldrmsdbl=rmsdbl;
    oldrmsdbr=rmsdbr;

    this->ui->sysvuL->repaint();
    this->ui->sysvuR->repaint();
}

void MasterMixerWidget::OnChangeInput(QAction* action)
{
    std::vector<std::string> v = InMgr::getInstance().GetCurrent()->GetPorts();
    for (unsigned int i = 0; i < v.size(); i++)
    {
        if (action->text() == QString::fromStdString(v[i]))
        {
            InMgr::getInstance().GetCurrent()->SetPort(i);
            this->ui->btnChangeInput->setText(action->text());
            break;
        }
    }
}

void MasterMixerWidget::OnAddEffect(QAction* action)
{
    int effectIndex = 0;
    for (effectIndex = 0; effectIndex < NUM_SYS_EFX; effectIndex++)
        if (this->_master->sysefx[effectIndex]->geteffect() == 0)
            break;

    if (effectIndex >= NUM_SYS_EFX)
    {
        QMessageBox msgBox;
        msgBox.setText("You can only have 4 system effects.");
        msgBox.exec();
        return;
    }

    this->_master->sysefx[effectIndex]->changeeffect(action->data().toInt());

    QWidget* w = new QWidget();
    w->setLayout(new QHBoxLayout());
    w->layout()->setMargin(0);
    w->layout()->setSpacing(0);

    QPushButton* edit = new QPushButton();
    edit->setProperty("slotindex", QVariant(effectIndex));
    edit->setText(action->text());
    edit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    connect(edit, SIGNAL(clicked()), this, SLOT(OnEditEffect()));
    w->layout()->addWidget(edit);

    QPushButton* remove = new QPushButton();
    remove->setProperty("slotindex", QVariant(effectIndex));
    remove->setIcon(QIcon(":/images/18_Close-32.png"));
    remove->setIconSize(QSize(20, 20));
    remove->setMaximumSize(24, 24);
    remove->setFlat(true);
    remove->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(remove, SIGNAL(clicked()), this, SLOT(OnRemoveEffect()));
    w->layout()->addWidget(remove);

    QVBoxLayout* l = (QVBoxLayout*)this->layout();
    l->insertWidget(l->indexOf(this->ui->btnAddEffect), w);

    for (effectIndex = 0; effectIndex < NUM_SYS_EFX; effectIndex++)
        if (this->_master->sysefx[effectIndex]->geteffect() == 0)
            break;

    this->ui->btnAddEffect->setEnabled(effectIndex < NUM_SYS_EFX);
    this->OnEditEffect();
}

void MasterMixerWidget::OnEditEffect()
{
    int slotindex = sender()->property("slotindex").toInt();
    QWidget* form = 0;
    switch (this->_master->sysefx[slotindex]->geteffect())
    {
        case 1: //  Reverb
        {
            form = new ReverbForm(this->_master->sysefx[slotindex]);
            break;
        }
        case 2: //  Echo
        {
            form = new EchoForm(this->_master->sysefx[slotindex]);
            break;
        }
        case 3: //  Chorus
        {
            form = new ChorusForm(this->_master->sysefx[slotindex]);
            break;
        }
        case 4: //  Phaser
        {
            form = new PhaserForm(this->_master->sysefx[slotindex]);
            break;
        }
        case 5: //  AlienWah
        {
            form = new AlienWahForm(this->_master->sysefx[slotindex]);
            break;
        }
        case 6: //  Distorsion
        {
            form = new DistorsionForm(this->_master->sysefx[slotindex]);
            break;
        }
        case 7: //  EQ
        {
            form = new EQForm(this->_master->sysefx[slotindex]);
            break;
        }
        case 8: //  DynFilter
        {
            form = new DynFilterForm(this->_master->sysefx[slotindex]);
            break;
        }
    }
    form->show();
}

void MasterMixerWidget::OnRemoveEffect()
{
    QPushButton* senderObj = (QPushButton*)sender();
    this->_master->sysefx[senderObj->property("slotindex").toInt()]->changeeffect(0);
    senderObj->parent()->deleteLater();

    this->ui->btnAddEffect->setEnabled(true);
}

void MasterMixerWidget::SetVolume(int volume)
{
    if (this->_master != 0)
        this->_master->setPvolume(volume);
}
