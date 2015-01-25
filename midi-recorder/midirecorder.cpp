#include "midirecorder.h"
#include
MidiRecorder::MidiRecorder()
    : _ready(false), _started(false)
{ }

MidiRecorder::~MidiRecorder()
{ }

void MidiRecorder::NoteOn(char chan, char note, char velocity)
{ }

void MidiRecorder::NoteOff(char chan, char note)
{ }

void MidiRecorder::GetReadyToRecord()
{
    this->_ready = true;
    this->_started = false;
}

void MidiRecorder::StartRecording()
{
    this->_ready = this->_started = true;
}

void MidiRecorder::StopRecording()
{
    this->_ready = this->_started = false;
}
