#ifndef EQFORM_H
#define EQFORM_H

#include <QWidget>
#include "Effects/EffectMgr.h"

namespace Ui {
class EQForm;
}

class EQForm : public QWidget
{
    Q_OBJECT

public:
    explicit EQForm(EffectMgr* effect, QWidget *parent = 0);
    ~EQForm();

private:
    Ui::EQForm *ui;
    EffectMgr* _effect;
};

#endif // EQFORM_H
