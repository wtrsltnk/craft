#include <Nio/Nio.h>
#include <globals.h>
#include <signal.h>
#include <pthread.h>
#include <iostream>

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

void sigterm_exit(int /*sig*/)
{
    Nio::Stop();
}

int main(int argc, char* argv[])
{
    synth = new Settings();

    signal(SIGINT, sigterm_exit);
    signal(SIGTERM, sigterm_exit);

    Nio::Start(synth);

    Nio::SetSink("PA");
    Nio::SetSource("PA");

    Mixer mixer;
    Nio::AddMixer(&mixer);

    while (true)
    { }

    Nio::Stop();
    return 0;
}
