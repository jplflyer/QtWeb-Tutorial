#pragma once

#include <QtCore/QtCore>

#include "DbBaseObject.h"

class Event: public DbBaseObject
{
private:
    QString		name;
    int			eventTemplateId;

public:
    Event() = default;
    Event(const QString & _name, int _eventTemplateId = 0): name(_name), eventTemplateId(_eventTemplateId) {}

    void fromJSON(const QJsonObject & json);
    void toJSON(QJsonObject & json) const;

    QString getName() const { return name; }
    int getEventTemplateId() const { return eventTemplateId; }

    Event & setId(int value) { DbBaseObject::setId(value); return *this; }
    Event & setName(const QString &value) { name = value; return *this; }
    Event & setEventTemplateId(int value) { eventTemplateId = value; return *this; }

    typedef QSharedPointer<Event> Pointer;
    typedef QVector<QSharedPointer<Event>> Vector;

};
