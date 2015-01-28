#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <Nio/Nio.h>
#include <Misc/Master.h>
#include "midirecorder.h"
#include "midiplayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Master _master;
    MidiRecorder _recorder;
    MidiPlayer _player;

    void SetRecorderState(RecorderState::eState state);

public slots:
    void OnTimerOut();
    void OnGetReady();
    void OnStartRecording();
    void OnStop();
    void OnPlayback();
    void OnChangeInput(QAction* action);
    void OnChangeInstrument();

private:
    Ui::MainWindow *ui;
    QTimer _timer;
    QGraphicsScene _scene;
    QGraphicsLineItem* _cursor;
    MidiClip* _clip;

};

#endif // MAINWINDOW_H
