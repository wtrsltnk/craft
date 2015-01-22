#include <Nio/Nio.h>
#include <Misc/Util.h>
#include <globals.h>
#include <signal.h>
#include <iostream>
#include <QApplication>
#include "mainwindow.h"
#include "mixer.h"

void sigterm_exit(int /*sig*/)
{
    delete [] denormalkillbuf;
    Nio::Stop();
}

int main(int argc, char* argv[])
{
    synth = new Settings();

    //produce denormal buf
    denormalkillbuf = new float [synth->buffersize];
    for(int i = 0; i < synth->buffersize; ++i)
        denormalkillbuf[i] = (RND - 0.5f) * 1e-16;

    signal(SIGINT, sigterm_exit);
    signal(SIGTERM, sigterm_exit);

    QApplication app(argc, argv);

    MainWindow wnd;
    Nio::Start(&wnd, synth);

    Nio::SetSink("PA");
    Nio::SetSource("RT");

    wnd.show();

    int result = app.exec();

    delete [] denormalkillbuf;
    Nio::Stop();
    return result;
}
