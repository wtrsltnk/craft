#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <Params/LFOParams.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    pthread_mutex_init(&mutex, NULL);
    ui->setupUi(this);

    this->fft = new FFTwrapper(1024);
    this->instument = new Instrument(&this->microtonal, this->fft, &this->mutex);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AudioOut(float *outl, float *outr)
{
    if(!pthread_mutex_trylock(&this->instument->load_mutex)) {
        this->instument->ComputePartSmps();
        pthread_mutex_unlock(&this->instument->load_mutex);
    }

    for (int i = 0; i < synth->buffersize; i++)
    {
        outl[i] += this->instument->partoutl[i];
        outr[i] += this->instument->partoutr[i];
    }

    //update the LFO's time
    LFOParams::time++;
}

void MainWindow::NoteOn(char chan, char note, char velocity)
{
    std::cout << "NoteOn " << note << std::endl;
    this->instument->NoteOn(note, velocity, 0);
}

void MainWindow::NoteOff(char chan, char note)
{
    std::cout << "NoteOff " << note << std::endl;
    this->instument->NoteOff(note);
    this->instument->RelaseAllKeys();
}
