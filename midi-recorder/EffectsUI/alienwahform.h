#ifndef ALIENWAHFORM_H
#define ALIENWAHFORM_H

#include <QWidget>
#include "Effects/EffectMgr.h"

namespace Ui {
class AlienWahForm;
}

class AlienWahForm : public QWidget
{
    Q_OBJECT

public:
    explicit AlienWahForm(EffectMgr* effect, QWidget *parent = 0);
    ~AlienWahForm();

    void EffectToUi();

public slots:
    void OnDialChanged(int value);
    void OnComboboxChanged(int value);

private:
    Ui::AlienWahForm *ui;
    EffectMgr* _effect;
};

#endif // ALIENWAHFORM_H
