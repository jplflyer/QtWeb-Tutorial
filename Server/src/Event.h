#pragma once

#include <string>
#include "DbBaseObject.h"

class Event: public DbBaseObject
{
private:
    std::string		name;
    int				eventTemplateId;

public:
    Event() = default;
    Event(const std::string & _name, int _eventTemplateId = 0): name(_name), eventTemplateId(_eventTemplateId) {}

    void fromJSON(const JSON & json);
    void toJSON(JSON & json) const;

    std::string getName() const { return name; }
    int getEventTemplateId() const { return eventTemplateId; }

    Event & setId(int value) { DbBaseObject::setId(value); return *this; }
    Event & setName(const std::string &value) { name = value; return *this; }
    Event & setEventTemplateId(int value) { eventTemplateId = value; return *this; }

    typedef std::shared_ptr<Event> Pointer;
    typedef JSON_Serializable_Vector<Event> Vector;

};
