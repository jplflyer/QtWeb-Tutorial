#pragma once

#include "DbBaseObject.h"

class EventTemplate: public DbBaseObject
{
private:
    QString		name;

public:
    EventTemplate() = default;
    EventTemplate(const QString _name) : name(_name) {}

    void fromJSON(const QJsonObject & json);
    void toJSON(QJsonObject & json) const;

    QString getName() const { return name; }

    EventTemplate & setId(int value) { DbBaseObject::setId(value); return *this; }
    EventTemplate & setName(const QString &value) { name = value; return *this; }

    typedef QSharedPointer<EventTemplate> Pointer;
    typedef QVector<Pointer> Vector;
    typedef bool (*Comparator)(const EventTemplate &);
};

EventTemplate::Pointer findById(EventTemplate::Vector &vec, int id);
