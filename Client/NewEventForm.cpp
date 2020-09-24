#include <QVectorIterator>

#include "NewEventForm.h"
#include "ui_NewEventForm.h"

/**
 * Constructor.
 */
NewEventForm::NewEventForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewEventForm)
{
    ui->setupUi(this);

    connect(&network, SIGNAL(gotEventTemplates(EventTemplate::Vector &)), this, SLOT(gotEventTemplates(EventTemplate::Vector &)) );
    network.getAllEventTemplates();
}

/**
 * Destructor.
 */
NewEventForm::~NewEventForm()
{
    if (allEventTemplates != nullptr) {
        delete allEventTemplates;
        allEventTemplates = nullptr;
    }
    delete ui;
}

/**
 * We've retrieved the list of event templates.
 */
void
NewEventForm::gotEventTemplates(EventTemplate::Vector &vec)
{
    if (allEventTemplates == nullptr) {
        allEventTemplates = new EventTemplate::Vector();
    }
    *allEventTemplates = vec;

    QVectorIterator<EventTemplate::Pointer> it(*allEventTemplates);
    while (it.hasNext()) {
        const EventTemplate::Pointer &ptr = it.next();
        ui->eventTemplateCB->addItem(ptr->getName());
    }
}

/**
 * Reset.
 */
void
NewEventForm::reset() {
}

/**
 * They clicked cancel.
 */
void NewEventForm::on_cancelBtn_clicked()
{
}

/**
 * They clicked Save.
 */
void NewEventForm::on_saveTbn_clicked()
{
}
