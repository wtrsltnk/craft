#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Nio/Nio.h>
#include <Misc/Master.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Master master;

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
