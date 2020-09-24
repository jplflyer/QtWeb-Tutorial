#pragma once

#include <string>

#include "DbBaseObject.h"

class EventTemplate: public DbBaseObject
{
private:
    std::string		name;

public:
    EventTemplate() = default;
    EventTemplate(const std::string _name) : name(_name) {}

    void fromJSON(const JSON & json);
    void toJSON(JSON & json) const;

    std::string getName() const { return name; }

    EventTemplate & setId(int value) { DbBaseObject::setId(value); return *this; }
    EventTemplate & setName(const std::string &value) { name = value; return *this; }

    typedef std::shared_ptr<EventTemplate> Pointer;
    typedef JSON_Serializable_Vector<EventTemplate> Vector;
};
