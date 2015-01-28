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

    if (this->_master.bank.bankfiletitle != std::string())
    {
        this->ui->cmbBank->setCurrentIndex(this->_master.bank.loadedbank() + 1);
        for (int i = 0; i < BANK_SIZE; i++)
        {
            std::string name = this->_master.bank.getname(i);
            if (name != " ")
            {
                QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(name));
                item->setData(45, QVariant(i));
                this->ui->lstInstruments->addItem(item);
            }
        }
    }

    connect(this->ui->cmbBank, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelectedBankChanged(int)));
    connect(this->ui->lstInstruments, SIGNAL(itemSelectionChanged()), this, SLOT(OnSelectedInstrumentChanged()));
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
        Bank::bankstruct bank = this->_master.bank.banks[index - 1];
        this->_master.bank.loadbank(bank.dir);

        for (int i = 0; i < BANK_SIZE; i++)
        {
            std::string name = this->_master.bank.getname(i);
            if (name != " ")
            {
                QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(name));
                item->setData(45, QVariant(i));
                this->ui->lstInstruments->addItem(item);
            }
        }
    }
}

void InstrumentSelectionDialog::OnSelectedInstrumentChanged()
{
    if (this->ui->lstInstruments->selectedItems().size() > 0)
    {
        this->_selectedInstrument = this->ui->lstInstruments->selectedItems()[0]->text();
        this->_selectedSlot = this->ui->lstInstruments->selectedItems()[0]->data(45).toInt();
    }
    else
    {
        this->_selectedInstrument = "";
        this->_selectedSlot = -1;
    }
}
