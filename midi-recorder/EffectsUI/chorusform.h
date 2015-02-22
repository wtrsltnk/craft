#ifndef CHORUSFORM_H
#define CHORUSFORM_H

#include <QWidget>
#include "Effects/EffectMgr.h"

namespace Ui {
class ChorusForm;
}

class ChorusForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChorusForm(EffectMgr* effect, QWidget *parent = 0);
    ~ChorusForm();

private:
    Ui::ChorusForm *ui;
    EffectMgr* _effect;
};

#endif // CHORUSFORM_H
