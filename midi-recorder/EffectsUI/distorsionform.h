#ifndef DISTORSIONFORM_H
#define DISTORSIONFORM_H

#include <QWidget>
#include "Effects/EffectMgr.h"

namespace Ui {
class DistorsionForm;
}

class DistorsionForm : public QWidget
{
    Q_OBJECT

public:
    explicit DistorsionForm(EffectMgr* effect, QWidget *parent = 0);
    ~DistorsionForm();

private:
    Ui::DistorsionForm *ui;
    EffectMgr* _effect;
};

#endif // DISTORSIONFORM_H
