#ifndef DYNFILTERFORM_H
#define DYNFILTERFORM_H

#include <QWidget>
#include "Effects/EffectMgr.h"

namespace Ui {
class DynFilterForm;
}

class DynFilterForm : public QWidget
{
    Q_OBJECT

public:
    explicit DynFilterForm(EffectMgr* effect, QWidget *parent = 0);
    ~DynFilterForm();

private:
    Ui::DynFilterForm *ui;
    EffectMgr* _effect;
};

#endif // DYNFILTERFORM_H
