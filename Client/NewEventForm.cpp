#include <QVectorIterator>

#include "NewEventForm.h"
#include "ui_NewEventForm.h"

/**
 * Constructor.
 */
NewEventForm::NewEventForm(QWidget *parent) :
    BaseForm(parent),
    ui(new Ui::NewEventForm)
{
    ui->setupUi(this);

    connect(&network, SIGNAL(gotEventTemplates(EventTemplate::Vector &)), this, SLOT(gotEventTemplates(EventTemplate::Vector &)) );
    connect(&network, &Network::saveEventComplete, this, &NewEventForm::saveEventComplete);
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
 * The event saved successfully.
 */
void
NewEventForm::saveEventComplete() {
    setStatus("Event Saved");
    emit switchToEventsWindow();
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
    emit switchToEventsWindow();
}

/**
 * They clicked Save.
 */
void NewEventForm::on_saveBtn_clicked()
{
    QString name = ui->nameTF->text().trimmed();

    if (name.length() == 0) {
        emit setStatus(QString("Please enter a name"));
        return;
    }

    Event::Pointer event = Event::create();
    event->setName(name);

    int index = ui->eventTemplateCB->currentIndex();
    if (allEventTemplates != nullptr && index >= 0 && index < allEventTemplates->size()) {
        EventTemplate::Pointer et = allEventTemplates->value(index);
        event->setEventTemplateId(et->getId());
    }

    network.saveNewEvent(event);
}
