#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pthread.h>
#include <Nio/Nio.h>
#include <Misc/Instrument.h>
#include <Misc/Microtonal.h>
#include <DSP/FFTwrapper.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public IMixer
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public: // IMixer implementation
    virtual void AudioOut(float *outl, float *outr);
    virtual void Lock() { pthread_mutex_lock(&(this->mutex)); }
    virtual void UnLock() { pthread_mutex_unlock(&(this->mutex)); }

    virtual void NoteOn(char chan, char note, char velocity);
    virtual void NoteOff(char chan, char note);
    virtual void PolyphonicAftertouch(char chan, char note, char velocity) { }
    virtual void SetController(char chan, int type, int par) { }
    virtual void SetProgram(char chan, unsigned int pgm) { }

private:
    Ui::MainWindow *ui;
    pthread_mutex_t mutex;

    FFTwrapper * fft;
    Microtonal microtonal;
    Instrument* instument;

};

#endif // MAINWINDOW_H
