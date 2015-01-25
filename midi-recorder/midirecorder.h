#ifndef MIDIRECORDER_H
#define MIDIRECORDER_H

#include "Nio/IMixer.h"
#include <pthread.h>

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

    bool _ready;
    bool _started;
    double _startTime;
};

#endif // MIDIRECORDER_H
