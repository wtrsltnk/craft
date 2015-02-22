#ifndef PHASERFORM_H
#define PHASERFORM_H

#include <QWidget>
#include "Effects/EffectMgr.h"

namespace Ui {
class PhaserForm;
}

class PhaserForm : public QWidget
{
    Q_OBJECT

public:
    explicit PhaserForm(EffectMgr* effect, QWidget *parent = 0);
    ~PhaserForm();

private:
    Ui::PhaserForm *ui;
    EffectMgr* _effect;
};

#endif // PHASERFORM_H
