#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QGraphicsRectItem>
#include <QScrollBar>
#include "Nio/InMgr.h"
#include "Nio/MidiIn.h"
#include "Nio/RtEngine.h"
#include "instrumentselectiondialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&this->_timer, SIGNAL(timeout()), this, SLOT(OnTimerOut()));
    connect(this->ui->btnGetReady, SIGNAL(clicked()), this, SLOT(OnGetReady()));
    connect(this->ui->btnStart, SIGNAL(clicked()), this, SLOT(OnStart()));
    connect(this->ui->btnStop, SIGNAL(clicked()), this, SLOT(OnStop()));
    connect(this->ui->btnChangeInstrument, SIGNAL(clicked()), this, SLOT(OnChangeInstrument()));

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

    this->SetRecorderState(this->_recorder.GetState());

    this->_timer.setInterval(100);
    this->_timer.start();

    this->ui->graphicsView->setScene(&this->_scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}
static const int NoteObject = 0;
void MainWindow::OnTimerOut()
{
    this->_scene.clear();
    MidiClip* clip = this->_recorder.GetCurrentClip();

    if (clip != 0)
    {
        int x, y, w, h;
        int maxx = this->ui->graphicsView->width();
        for (unsigned int i = 0; i < clip->_notes.size(); i++)
        {
            MidiNote* note = clip->_notes[i];
            x = note->_start / 10;
            y = int(note->_note) * 12;
            w = (note->_end - note->_start) / 10;
            h = 12;

            QGraphicsRectItem* r = new QGraphicsRectItem(x, y, w, h);
            if (maxx < x + w) maxx = x + w;
            r->setBrush(* new QBrush(Qt::red));
            r->setPen(* new QPen(Qt::transparent));
            r->setData(NoteObject, QVariant::fromValue((void*)note));
            this->_scene.addItem(r);
        }
        QRectF r = this->_scene.sceneRect();
        r.setWidth(maxx);
        this->_scene.setSceneRect(r);
    }
}

void MainWindow::OnGetReady()
{
    this->_recorder.GetReadyToRecord();
    this->SetRecorderState(this->_recorder.GetState());
}

void MainWindow::OnStart()
{
    this->_recorder.StartRecording();
    this->SetRecorderState(this->_recorder.GetState());
}

void MainWindow::OnStop()
{
    this->_recorder.StopRecording();
    this->SetRecorderState(this->_recorder.GetState());
}

void MainWindow::SetRecorderState(RecorderState::eState state)
{
    switch (state)
    {
    case RecorderState::Stopped:
    {
        this->ui->btnGetReady->setEnabled(true);
        this->ui->btnStart->setEnabled(true);
        this->ui->btnStop->setEnabled(false);
        break;
    }
    case RecorderState::Ready:
    {
        this->ui->btnGetReady->setEnabled(false);
        this->ui->btnStart->setEnabled(false);
        this->ui->btnStop->setEnabled(true);
        break;
    }
    case RecorderState::Started:
    {
        this->ui->btnGetReady->setEnabled(false);
        this->ui->btnStart->setEnabled(false);
        this->ui->btnStop->setEnabled(true);
        break;
    }
    }
}

void MainWindow::OnChangeInput(QAction* action)
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

void MainWindow::OnChangeInstrument()
{
    InstrumentSelectionDialog dlg(this->_master, this);
    if (dlg.exec() == QDialog::Accepted)
    {
        if (dlg.selectedInstrument() == "")
            this->ui->btnChangeInstrument->setText("[default]");
        else
            this->ui->btnChangeInstrument->setText(dlg.selectedInstrument());
    }
}
