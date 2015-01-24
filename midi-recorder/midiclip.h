#ifndef MIDICLIP_H
#define MIDICLIP_H

#include <vector>

class MidiNote
{
public:
    MidiNote();
    virtual ~MidiNote();

    char _note;
    char _velocity;
    double _start;
    double _end;
};

class MidiClip
{
public:
    MidiClip();
    virtual ~MidiClip();

    std::vector<MidiNote*> _notes;
};

#endif // MIDICLIP_H
