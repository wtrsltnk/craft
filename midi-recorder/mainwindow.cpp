#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QGraphicsRectItem>
#include <QScrollBar>
#include "Nio/InMgr.h"
#include "Nio/MidiIn.h"
#include "Nio/RtEngine.h"
#include "Misc/Instrument.h"
#include "instrumentselectiondialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _player(MidiPlayer(&this->_master)),
    _clip(0)
{
    ui->setupUi(this);

    connect(&this->_timer, SIGNAL(timeout()), this, SLOT(OnTimerOut()));
    connect(this->ui->btnGetReady, SIGNAL(clicked()), this, SLOT(OnGetReady()));
    connect(this->ui->btnStart, SIGNAL(clicked()), this, SLOT(OnStartRecording()));
    connect(this->ui->btnStop, SIGNAL(clicked()), this, SLOT(OnStop()));
    connect(this->ui->btnPlay, SIGNAL(clicked()), this, SLOT(OnPlayback()));
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

    this->_clip = new MidiClip();
    this->SetRecorderState(this->_recorder.GetState());

    this->_timer.setInterval(100);
    this->_timer.start();

    this->ui->graphicsView->setScene(&this->_scene);
    this->ui->graphicsView->setTransformationAnchor(QGraphicsView::NoAnchor);
    this->ui->graphicsView->setAlignment (Qt::AlignLeft);
    this->_cursor = this->_scene.addLine(0, 0, 0, 12 * 256, QPen(Qt::green));
    this->_scene.sceneRect().setLeft(this->_scene.sceneRect().left() - 15);
    QRectF r = this->_scene.sceneRect();
    r.setX(-15);
    r.setY(0);
    r.setWidth(this->ui->graphicsView->width()-15);
    this->ui->graphicsView->setSceneRect(r);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnTimerOut()
{
    MidiClip* clip = this->_recorder.GetCurrentClip();

    if (this->_recorder.GetState() != RecorderState::Stopped)
    {
        this->_cursor->setX(this->_recorder.CurrentTime() / 10);

        int x, y, w, h;
        int maxx = this->ui->graphicsView->width() - 50;
        NoteItem* noteItem = 0;
        MidiNote* note = clip->_firstNote;
        while (note != 0)
        {
            x = note->_start / 10;
            y = int(note->_note) * 12;
            w = (note->_end - note->_start) / 10;
            h = 12;
            if (maxx < x + w) maxx = x + w;

            QList<QGraphicsItem*> items = this->_scene.items(x, y, w, h, Qt::IntersectsItemBoundingRect, Qt::AscendingOrder);

            bool found = false;
            for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i)
            {
                NoteItem* localNote = (NoteItem*)*i;
                if (localNote->_note == note)
                {
                    found = true;
                    localNote->setRect(x, y, w, h);
                }
            }

            if (found == false)
            {
                noteItem = new NoteItem(note);
                this->_scene.addItem(noteItem);
            }
            note = note->_nextNote;
        }
        QRectF r = this->_scene.sceneRect();
        r.setX(-15);
        r.setWidth(maxx + 30);
        this->ui->graphicsView->setSceneRect(r);
        if (noteItem != 0)
            this->ui->graphicsView->ensureVisible(noteItem, 100, 50);
    }
    else if (this->_player.GetState() == PlayerState::Started)
    {
        this->_cursor->setX(this->_player.CurrentTime() / 10);

    }
}

void MainWindow::OnGetReady()
{
    this->_recorder.GetReadyToRecord(this->_clip);
    this->SetRecorderState(this->_recorder.GetState());
}

void MainWindow::OnStartRecording()
{
    this->_recorder.StartRecording(this->_clip);
    this->SetRecorderState(this->_recorder.GetState());
}

void MainWindow::OnStop()
{
    this->_recorder.StopRecording();
    this->_player.Stop();
    this->SetRecorderState(this->_recorder.GetState());
}

void MainWindow::OnPlayback()
{
    if (this->_recorder.GetState() == RecorderState::Stopped)
    {
        this->_player.Start(this->_clip);
        this->SetRecorderState(this->_recorder.GetState());
    }
}

void MainWindow::SetRecorderState(RecorderState::eState state)
{
    switch (state)
    {
    case RecorderState::Stopped:
    {
        if (this->_player.GetState() == PlayerState::Started)
        {
            this->ui->btnGetReady->setEnabled(false);
            this->ui->btnStart->setEnabled(false);
            this->ui->btnStop->setEnabled(true);
            this->ui->btnPlay->setEnabled(false);
        }
        else
        {
            this->ui->btnGetReady->setEnabled(this->_clip != 0 && this->_clip->_firstNote == 0);
            this->ui->btnStart->setEnabled(this->_clip != 0 && this->_clip->_firstNote == 0);
            this->ui->btnStop->setEnabled(false);
            this->ui->btnPlay->setEnabled(this->_clip != 0 && this->_clip->_firstNote != 0);
        }
        break;
    }
    case RecorderState::Ready:
    {
        this->ui->btnGetReady->setEnabled(false);
        this->ui->btnStart->setEnabled(false);
        this->ui->btnStop->setEnabled(true);
        this->ui->btnPlay->setEnabled(false);
        break;
    }
    case RecorderState::Started:
    {
        this->ui->btnGetReady->setEnabled(false);
        this->ui->btnStart->setEnabled(false);
        this->ui->btnStop->setEnabled(true);
        this->ui->btnPlay->setEnabled(false);
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
        Instrument* i = this->_master.Instruments().front();
        if (dlg.selectedSlot() < 0)
        {
            i->defaultsinstrument();
            this->ui->btnChangeInstrument->setText("[default]");
        }
        else
        {
            this->_master.bank.loadfromslot(dlg.selectedSlot(), i);
            this->ui->btnChangeInstrument->setText(dlg.selectedInstrument());
        }
    }
}
