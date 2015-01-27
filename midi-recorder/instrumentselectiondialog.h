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

public slots:
    void OnSelectedBankChanged(int index);

private:
    Ui::InstrumentSelectionDialog *ui;
    QString _selectedInstrument;
    Master& _master;
};

#endif // INSTRUMENTSELECTIONDIALOG_H
