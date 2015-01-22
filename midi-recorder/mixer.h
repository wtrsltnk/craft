#ifndef MIXER
#define MIXER

#include <Nio/Nio.h>
#include <pthread.h>

class Mixer : public IMixer
{
    pthread_mutex_t mutex;
public:
    Mixer()
    {
        pthread_mutex_init(&mutex, NULL);
    }

    virtual void AudioOut(float *outl, float *outr) { }
    virtual void Lock() { pthread_mutex_lock(&(this->mutex)); }
    virtual void UnLock() { pthread_mutex_unlock(&(this->mutex)); }

    virtual void NoteOn(char chan, char note, char velocity) { }
    virtual void NoteOff(char chan, char note) { }
    virtual void PolyphonicAftertouch(char chan, char note, char velocity) { }
    virtual void SetController(char chan, int type, int par) { }
    virtual void SetProgram(char chan, unsigned int pgm) { }
};

#endif // MIXER

