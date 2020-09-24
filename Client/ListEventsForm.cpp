#include <QTableWidgetItem>

#include "ListEventsForm.h"
#include "ui_ListEventsForm.h"

#include "Event.h"

ListEventsForm::ListEventsForm(QWidget *parent) :
    BaseForm(parent),
    ui(new Ui::ListEventsForm)
{
    ui->setupUi(this);

    connect(&network, SIGNAL(gotEventTemplates(EventTemplate::Vector &)), this, SLOT(gotEventTemplates(EventTemplate::Vector &)) );
    connect(&network, SIGNAL(gotEvents(Event::Vector &)), this, SLOT(gotEvents(Event::Vector &)) );
}

ListEventsForm::~ListEventsForm()
{
    delete ui;
    if (eventTemplates != nullptr) {
        delete eventTemplates;
        eventTemplates = nullptr;
    }
    if (events != nullptr) {
        delete events;
        events = nullptr;
    }
}

/**
 * The ping completed.
 */
void
ListEventsForm::beginLoad() {
    network.getAllEventTemplates();
}

/**
 * list the events.
 */
void
ListEventsForm::gotEventTemplates(EventTemplate::Vector &vec) {
    emit setStatus("Got event templates");
    if (eventTemplates == nullptr) {
        eventTemplates = new EventTemplate::Vector();
    }
    *eventTemplates = vec;

    network.getAllEvents();
}

/**
 * list the events.
 */
void
ListEventsForm::gotEvents(Event::Vector &vec) {
    emit setStatus("Got events");

    if (events == nullptr) {
        events = new Event::Vector();
    }
    *events = vec;

    ui->eventsTable->setColumnCount(3);
    ui->eventsTable->setRowCount(events->size());
    ui->eventsTable->setHorizontalHeaderItem(0, new QTableWidgetItem("ID"));
    ui->eventsTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
    ui->eventsTable->setHorizontalHeaderItem(2, new QTableWidgetItem("Event Template Name"));

    for (int index = 0; index < events->size(); ++index) {
        const Event::Pointer &event = events->at(index);
        const EventTemplate::Pointer &et = findById(*eventTemplates, event->getEventTemplateId());

        int colIndex = 0;
        ui->eventsTable->setItem(index, colIndex++, new QTableWidgetItem(QString::number(event->getId())));
        ui->eventsTable->setItem(index, colIndex++, new QTableWidgetItem(event->getName()) );

        if (!et.isNull()) {
            ui->eventsTable->setItem(index, colIndex++, new QTableWidgetItem(et->getName()));
        }
    }

    ui->eventsTable->resizeColumnsToContents();
}

/**
 * They clicked Create Event. We do this by sending a signal and letting the parent window
 * handle it.
 */
void ListEventsForm::on_createNewPB_clicked()
{
    emit createNewEvent();
}
