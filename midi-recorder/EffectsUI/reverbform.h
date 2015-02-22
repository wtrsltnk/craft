#ifndef REVERBFORM_H
#define REVERBFORM_H

#include <QWidget>
#include "Effects/EffectMgr.h"

namespace Ui {
class ReverbForm;
}

class ReverbForm : public QWidget
{
    Q_OBJECT

public:
    explicit ReverbForm(EffectMgr* effect, QWidget *parent = 0);
    ~ReverbForm();

private:
    Ui::ReverbForm *ui;
    EffectMgr* _effect;
};

#endif // REVERBFORM_H
