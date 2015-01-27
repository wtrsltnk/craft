#include "midirecorder.h"
#include <iostream>

MidiRecorder::MidiRecorder()
    : _ready(false), _timer(0), _clip(0)
{
    pthread_mutex_init(&this->mutex, NULL);
    for (int i = 0; i < 256; i++)
        this->_runningNotes[i] = 0;
}

MidiRecorder::~MidiRecorder()
{ }

void MidiRecorder::NoteOn(char chan, char note, char velocity)
{
    if (this->_ready)
    {
        if (this->_timer == 0)
        {
            this->_timer = new QElapsedTimer();
            this->_timer->start();
            this->_clip = new MidiClip();
        }

        if (this->_runningNotes[note] == 0)
        {
            this->_runningNotes[note] = new MidiNote();
            this->_clip->_notes.push_back(this->_runningNotes[note]);
            this->_runningNotes[note]->_note = note;
            this->_runningNotes[note]->_velocity = velocity;
            this->_runningNotes[note]->_start = this->_timer->elapsed();
        }
    }
}

void MidiRecorder::NoteOff(char chan, char note)
{
    if (this->_runningNotes[note] != 0)
    {
        if (this->_ready && this->_timer != 0)
        {
            this->_runningNotes[note]->_end = this->_timer->elapsed();
            this->_runningNotes[note] = 0;
        }
    }
}

RecorderState::eState MidiRecorder::GetState()
{
    if (this->_ready == false)
        return RecorderState::Stopped;

    if (this->_ready && this->_timer == 0)
        return RecorderState::Ready;

    return RecorderState::Started;
}

void MidiRecorder::GetReadyToRecord()
{
    if (this->_timer == 0)
    {
        this->_ready = true;
    }
    else
    {
        std::cerr << "Timer already started" << std::endl;
    }
}

void MidiRecorder::StartRecording()
{
    this->_ready = true;
    this->_timer = new QElapsedTimer();
    this->_timer->start();
    this->_clip = new MidiClip();
}

void MidiRecorder::StopRecording()
{
    for (int i = 0; i < 256; i++)
    {
        if (this->_runningNotes[i] != 0)
            this->_runningNotes[i]->_end = this->_timer->elapsed();

    }
    this->_ready = false;
    delete this->_timer;
    this->_timer = 0;
}
