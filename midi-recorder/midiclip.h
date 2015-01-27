#ifndef MIDICLIP_H
#define MIDICLIP_H

#include <vector>
#include <qglobal.h>

class MidiNote
{
public:
    MidiNote() { }
    virtual ~MidiNote() { }

    char _note;
    char _velocity;
    qint64 _start;
    qint64 _end;
};

class MidiClip
{
public:
    MidiClip();
    virtual ~MidiClip();

    std::vector<MidiNote*> _notes;
};

#endif // MIDICLIP_H
