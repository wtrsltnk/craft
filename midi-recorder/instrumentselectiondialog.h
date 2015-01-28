#ifndef INSTRUMENTSELECTIONDIALOG_H
#define INSTRUMENTSELECTIONDIALOG_H

#include <QDialog>
#include "Misc/Master.h"

namespace Ui {
class InstrumentSelectionDialog;
}

class InstrumentSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InstrumentSelectionDialog(Master& master, QWidget *parent = 0);
    ~InstrumentSelectionDialog();

    QString& selectedInstrument() { return this->_selectedInstrument; }
    int selectedSlot() { return this->_selectedSlot; }

public slots:
    void OnSelectedBankChanged(int index);
    void OnSelectedInstrumentChanged();

private:
    Ui::InstrumentSelectionDialog *ui;
    QString _selectedInstrument;
    int _selectedSlot;
    Master& _master;
};

#endif // INSTRUMENTSELECTIONDIALOG_H
