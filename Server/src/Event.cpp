#include "Event.h"

void Event::fromJSON(const JSON & json) {
    DbBaseObject::fromJSON(json);

    name = stringValue(json, "name");
    eventTemplateId = intValue(json, "eventTemplateId");
}

void Event::toJSON(JSON & json) const {
    DbBaseObject::toJSON(json);

    json["name"] = name;
    json["eventTemplateId"] = eventTemplateId;
}
