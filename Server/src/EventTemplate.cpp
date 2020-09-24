#include "EventTemplate.h"

void EventTemplate::fromJSON(const JSON & json) {
    DbBaseObject::fromJSON(json);

    name = stringValue(json, "name");
}

void EventTemplate::toJSON(JSON & json) const {
    DbBaseObject::toJSON(json);

    json["name"] = name;
}
