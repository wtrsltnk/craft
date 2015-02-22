#ifndef ECHOFORM_H
#define ECHOFORM_H

#include <QWidget>
#include "Effects/EffectMgr.h"

namespace Ui {
class EchoForm;
}

class EchoForm : public QWidget
{
    Q_OBJECT

public:
    explicit EchoForm(EffectMgr* effect, QWidget *parent = 0);
    ~EchoForm();

    void EffectToUi();

public slots:
    void OnDialChanged(int value);

private:
    Ui::EchoForm *ui;
    EffectMgr* _effect;
};

#endif // ECHOFORM_H
