#pragma once

#include <QWidget>
#include <QVector>
#include <QtNetwork/QNetworkReply>

#include "BaseForm.h"
#include "Network.h"
#include "Event.h"
#include "EventTemplate.h"

namespace Ui {
class ListEventsForm;
}

class ListEventsForm : public BaseForm
{
    Q_OBJECT

public:
    explicit ListEventsForm(QWidget *parent = nullptr);
    ~ListEventsForm();

    void beginLoad();

public slots:
    void gotEventTemplates(EventTemplate::Vector &);
    void gotEvents(Event::Vector &);
    void on_createNewPB_clicked();

signals:
    void createNewEvent();

private:
    Ui::ListEventsForm *ui;

    Network network;
    EventTemplate::Vector *eventTemplates = nullptr;
    Event::Vector *events = nullptr;

};

