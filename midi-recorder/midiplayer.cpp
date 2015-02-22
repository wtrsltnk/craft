#include "midiplayer.h"
#include <iostream>

MidiPlayer::MidiPlayer(Master* master)
    : _master(master), _timer(0), _nextNoteTimer(0), _clip(0)
{ }

MidiPlayer::~MidiPlayer()
{ }

qint64 MidiPlayer::CurrentTime()
{
    if (this->_timer != 0)
        return this->_timer->elapsed();

    return 0;
}

void MidiPlayer::Start(MidiClip* clip)
{
    this->_clip = clip;
    if (this->_clip != 0)
    {
        this->_midiActions.clear();
        MidiNote* note = this->_clip->_firstNote;
        while (note != 0)
        {
            tMidiAction start = { note->_note, note->_velocity };
            tMidiAction end = { note->_note, 0 };
            this->_midiActions.insert(std::make_pair(note->_start, start));
            this->_midiActions.insert(std::make_pair(note->_end, end));
            note = note->_nextNote;
        }
        this->_cursor = this->_midiActions.begin();
    }

    this->_timer = new QElapsedTimer();
    this->_timer->start();
    if (this->_cursor != this->_midiActions.end())
    {
        this->_nextNoteTimer = new QTimer();
        connect(this->_nextNoteTimer, SIGNAL(timeout()), this, SLOT(OnTimeout()));
        this->_nextNoteTimer->setSingleShot(true);
        this->_nextNoteTimer->start((*this->_cursor).first);
    }
}

void MidiPlayer::OnTimeout()
{
    qint64 time = (*this->_cursor).first;
    while ((*this->_cursor).first == time)
    {
        tMidiAction a = (*this->_cursor).second;
        if (a.velocity > 0)
            this->_master->NoteOn(0, a.note, a.velocity);
        else
            this->_master->NoteOff(0, a.note);

        this->_cursor++;
        if (this->_cursor == this->_midiActions.end())
            break;
    }
    if (this->_cursor != this->_midiActions.end())
        this->_nextNoteTimer->start((*this->_cursor).first - time);
}

void MidiPlayer::Stop()
{
    delete this->_timer;
    this->_timer = 0;
    if (this->_nextNoteTimer != 0)
    {
        delete this->_nextNoteTimer;
        this->_nextNoteTimer = 0;
    }
    this->_master->ShutUp();
}

PlayerState::eState MidiPlayer::GetState()
{
    if (this->_timer != 0)
        return PlayerState::Started;

    return PlayerState::Stopped;
}
