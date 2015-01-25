#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Nio/Nio.h>
#include <Misc/Master.h>
#include "midirecorder.h"

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

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
