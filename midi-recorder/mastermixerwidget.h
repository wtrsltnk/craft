#ifndef MASTERMIXERWIDGET_H
#define MASTERMIXERWIDGET_H

#include <QWidget>
#include "Misc/Master.h"

namespace Ui {
class MasterMixerWidget;
}

class MasterMixerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MasterMixerWidget(QWidget *parent = 0);
    ~MasterMixerWidget();

    void SetMaster(Master* master);

protected slots:
    void OnVuTimer();
    void OnChangeInput(QAction* action);
    void OnAddEffect(QAction* action);
    void OnEditEffect();
    void OnRemoveEffect();
    void SetVolume(int volume);

private:
    Ui::MasterMixerWidget *ui;
    Master* _master;
    QTimer* _vutimer;

    float dbl;
    float dbr;
    float rmsdbl;
    float rmsdbr;

    bool eventFilter(QObject* watched, QEvent* event);
};

#endif // MASTERMIXERWIDGET_H
