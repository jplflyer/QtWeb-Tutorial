#pragma once

#include <QObject>
#include <QtNetwork>

#include "User.h"
#include "EventTemplate.h"
#include "Event.h"

/**
 * This clas implements our network connections.
 */
class Network: public QObject
{
Q_OBJECT

public:
    Network();
    virtual ~Network();

    void ping();
    void getAllEvents();
    void getAllEventTemplates();
    void saveNewEvent(Event::Pointer);

signals:
    void pingCompleted(QNetworkReply *);
    void gotEvents(Event::Vector &);
    void gotEventTemplates(EventTemplate::Vector &);
    void saveEventComplete();

public slots:
    void gotPing();
    void gotAllEventTemplates();
    void gotAllEvents();
    void gotSaveEvent();

private:
    void setAuthorization(QNetworkRequest &request, const QString &username, const QString &pw);

};

