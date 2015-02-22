#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QGraphicsRectItem>
#include <QScrollBar>
#include <QMessageBox>
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
    connect(this->ui->btnPlay, SIGNAL(clicked()), this, SLOT(OnPlayback()));
    connect(this->ui->btnChangeInstrument, SIGNAL(clicked()), this, SLOT(OnChangeInstrument()));
    connect(this->ui->btnSelectInstrument, SIGNAL(clicked()), this, SLOT(OnSubmitChangeInstrument()));
    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(OnExit()));

    this->_clip = new MidiClip();

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

    this->ui->ww->hide();

    this->ui->mixer->SetMaster(&this->_master);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnExit()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Exiting midi-recorder...", "Are you sure?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
        this->close();
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
    if (this->ui->btnGetReady->isChecked())
    {
        this->_recorder.GetReadyToRecord(this->_clip);
        this->ui->btnPlay->setEnabled(false);
    }
    else
    {
        this->_recorder.StopRecording();
        this->ui->btnPlay->setEnabled(true);
    }
}

void MainWindow::OnPlayback()
{
    if (this->ui->btnPlay->isChecked())
    {
        this->_player.Start(this->_clip);
        this->ui->btnGetReady->setEnabled(false);
    }
    else
    {
        this->_player.Stop();
        this->ui->btnGetReady->setEnabled(true);
    }
}

void MainWindow::OnChangeInstrument()
{
    this->ui->ww->show();
    this->ui->instrumentselector->SetMaster(&this->_master);

}

void MainWindow::OnSubmitChangeInstrument()
{
    Instrument* i = this->_master.Instruments().front();
    if (this->ui->instrumentselector->selectedSlot() < 0)
    {
        pthread_mutex_lock(&i->load_mutex);
        i->defaultsinstrument();
        pthread_mutex_unlock(&i->load_mutex);
        this->ui->btnChangeInstrument->setText("[default]");
    }
    else
    {
        pthread_mutex_lock(&i->load_mutex);
        this->_master.bank.loadfromslot(this->ui->instrumentselector->selectedSlot(), i);
        i->applyparameters();
        pthread_mutex_unlock(&i->load_mutex);
        this->ui->btnChangeInstrument->setText(this->ui->instrumentselector->selectedInstrument());
    }
    this->ui->ww->hide();
}
