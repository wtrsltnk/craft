#ifndef MIDIRECORDER_H
#define MIDIRECORDER_H

#include "midiclip.h"
#include "Nio/IMixer.h"
#include <pthread.h>
#include <QElapsedTimer>

namespace RecorderState
{
enum eState
{
    Stopped,
    Ready,
    Started
};
}
class MidiRecorder : public IMixer
{
    pthread_mutex_t mutex;
public:
    MidiRecorder();
    virtual ~MidiRecorder();

    virtual void AudioOut(float *outl, float *outr) { }
    virtual void Lock() { pthread_mutex_lock(&(this->mutex)); }
    virtual void UnLock() { pthread_mutex_unlock(&(this->mutex)); }

    virtual void NoteOn(char chan, char note, char velocity);
    virtual void NoteOff(char chan, char note);
    virtual void PolyphonicAftertouch(char chan, char note, char velocity) { }
    virtual void SetController(char chan, int type, int par) { }
    virtual void SetProgram(char chan, unsigned int pgm) { }

    void GetReadyToRecord();
    void StartRecording();
    void StopRecording();

    RecorderState::eState GetState();
    MidiClip* GetCurrentClip() { return this->_clip; }

private:
    bool _ready;
    QElapsedTimer* _timer;
    double _startTime;
    MidiNote* _runningNotes[256];
    MidiClip* _clip;
};

#endif // MIDIRECORDER_H
