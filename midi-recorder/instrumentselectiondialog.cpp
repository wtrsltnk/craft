#include "instrumentselectiondialog.h"
#include "ui_instrumentselectiondialog.h"

InstrumentSelectionDialog::InstrumentSelectionDialog(Master& master, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InstrumentSelectionDialog),
    _master(master)
{
    ui->setupUi(this);

    this->ui->cmbBank->addItem("Select a bank");
    this->_master.bank.rescanforbanks();
    for (std::vector<Bank::bankstruct>::iterator i = this->_master.bank.banks.begin(); i != this->_master.bank.banks.end(); ++i)
    {
        Bank::bankstruct bank = *i;
        this->ui->cmbBank->addItem(QString::fromStdString(bank.name));
    }

    connect(this->ui->cmbBank, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelectedBankChanged(int)));
}

InstrumentSelectionDialog::~InstrumentSelectionDialog()
{
    delete ui;
}

void InstrumentSelectionDialog::OnSelectedBankChanged(int index)
{
    this->ui->lstInstruments->clear();
    if (index > 0)
    {
        Bank::bankstruct bank = this->_master.bank.banks[index];
        this->_master.bank.loadbank(bank.dir);

        for (int i = 0; i < BANK_SIZE; i++)
        {
            std::string name = this->_master.bank.getname(i);
            if (name != " ")
                this->ui->lstInstruments->addItem(QString::fromStdString(name));
        }
    }
}
