#ifndef INSTRUMENTSELECTIONDIALOG_H
#define INSTRUMENTSELECTIONDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QTimer>
#include "Misc/Master.h"

namespace Ui {
class InstrumentSelectionDialog;
}

class InstrumentSelectionDialog : public QWidget
{
    Q_OBJECT

public:
    explicit InstrumentSelectionDialog(QWidget *parent = 0);
    ~InstrumentSelectionDialog();

    void SetMaster(Master* master);

    QString& selectedInstrument() { return this->_selectedInstrument; }
    int selectedSlot() { return this->_selectedSlot; }

public slots:
    void OnSelectedBankChanged(int index);
    void OnSelectedInstrumentChanged();
    void OnInstrumentClicked(QListWidgetItem* item);
    void OnPreviewTimeout();

private:
    Ui::InstrumentSelectionDialog *ui;
    QString _selectedInstrument;
    int _selectedSlot;
    Master* _master;
    QTimer _previewTimeout;
    std::vector<Instrument*> _previewInstruments;
};

#endif // INSTRUMENTSELECTIONDIALOG_H
