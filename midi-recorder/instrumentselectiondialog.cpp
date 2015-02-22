#include "instrumentselectiondialog.h"
#include "ui_instrumentselectiondialog.h"
#include <iostream>

InstrumentSelectionDialog::InstrumentSelectionDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstrumentSelectionDialog)
{
    ui->setupUi(this);
}

void InstrumentSelectionDialog::SetMaster(Master* master)
{
    if (this->_master != master)
    {
        this->_master = master;

        this->ui->cmbBank->addItem("Select a bank");
        this->_master->bank.rescanforbanks();
        for (std::vector<Bank::bankstruct>::iterator i = this->_master->bank.banks.begin(); i != this->_master->bank.banks.end(); ++i)
        {
            Bank::bankstruct bank = *i;
            this->ui->cmbBank->addItem(QString::fromStdString(bank.name));
        }

        if (this->_master->bank.bankfiletitle != std::string())
        {
            this->ui->cmbBank->setCurrentIndex(this->_master->bank.loadedbank() + 1);
            for (int i = 0; i < BANK_SIZE; i++)
            {
                std::string name = this->_master->bank.getname(i);
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
        connect(this->ui->lstInstruments, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(OnInstrumentClicked(QListWidgetItem*)));
        connect(&this->_previewTimeout, SIGNAL(timeout()), this, SLOT(OnPreviewTimeout()));
    }
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
        Bank::bankstruct bank = this->_master->bank.banks[index - 1];
        this->_master->bank.loadbank(bank.dir);

        for (int i = 0; i < BANK_SIZE; i++)
        {
            std::string name = this->_master->bank.getname(i);
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

void InstrumentSelectionDialog::OnInstrumentClicked(QListWidgetItem *item)
{
    // First all other notes off
    this->_master->NoteOff(15, 64);

    // Change all the other preview instrument their channels so they don;t get triggered again
    for (std::vector<Instrument*>::iterator itr = this->_previewInstruments.begin(); itr != this->_previewInstruments.end(); ++itr)
    {
        Instrument* i = *itr;
        i->Prcvchn = 14;
    }


    int slot = item->data(45).toInt();
    Instrument* i = this->_master->addInstrument();
    this->_master->bank.loadfromslot(slot, i);
    i->Prcvchn = 15;
    i->applyparameters();
    this->_master->NoteOn(15, 64, 90);
    this->_previewTimeout.start(2000);
    this->_previewInstruments.push_back(i);
}

void InstrumentSelectionDialog::OnPreviewTimeout()
{
    this->_master->NoteOff(15, 64);
    for (std::vector<Instrument*>::iterator itr = this->_previewInstruments.begin(); itr != this->_previewInstruments.end(); ++itr)
    {
        Instrument* i = *itr;
        if (i->HasNotesPlaying() == false)
        {
            i->AllNotesOff();
            this->_master->removeInstrument(i);
            this->_previewInstruments.erase(itr);
            if (this->_previewInstruments.size() == 0)
                break;
            else
                itr = this->_previewInstruments.begin();
        }
    }
}
