#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H

#include <Misc/Master.h>
#include <QElapsedTimer>
#include <QTimer>
#include "midiclip.h"
#include <map>

namespace PlayerState
{
enum eState
{
    Stopped,
    Started
};
}

typedef struct {
    char note, velocity;
} tMidiAction;

class MidiPlayer : public QObject
{
    Q_OBJECT
public:
    MidiPlayer(Master* master);
    virtual ~MidiPlayer();

    void Start(MidiClip* clip);
    void Stop();

    PlayerState::eState GetState();

    qint64 CurrentTime();

public slots:
    void OnTimeout();

private:
    QElapsedTimer* _timer;
    QTimer* _nextNoteTimer;
    double _startTime;
    Master* _master;
    MidiClip* _clip;
    std::multimap<qint64, tMidiAction> _midiActions;
    std::multimap<qint64, tMidiAction>::iterator _cursor;
};

#endif // MIDIPLAYER_H
