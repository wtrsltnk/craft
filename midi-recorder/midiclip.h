#ifndef MIDICLIP_H
#define MIDICLIP_H

#include <vector>
#include <qglobal.h>

class MidiNote
{
public:
    MidiNote(MidiNote* prev) : _prevNote(prev), _nextNote(0) { if (this->_prevNote != 0) this->_prevNote->_nextNote = this; }
    virtual ~MidiNote() { }

    char _note;
    char _velocity;
    qint64 _start;
    qint64 _end;

    MidiNote* _prevNote;
    MidiNote* _nextNote;
};

class MidiClip
{
public:
    MidiClip();
    virtual ~MidiClip();

    MidiNote* _firstNote;
    MidiNote* _lastNote;
};

#endif // MIDICLIP_H
