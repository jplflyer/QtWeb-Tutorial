#include "EventTemplate.h"

void EventTemplate::fromJSON(const QJsonObject & json) {
    DbBaseObject::fromJSON(json);

    name = stringValue(json, "name");
}

void EventTemplate::toJSON(QJsonObject & json) const {
    DbBaseObject::toJSON(json);

    json["name"] = QJsonValue(name);
}

/**
 * Lookup based on ID. This doesn't presume order, alhtough we could have done a binary search.
 */
EventTemplate::Pointer
findById(EventTemplate::Vector &vec, int id) {
    EventTemplate::Pointer retVal;
    QVectorIterator<EventTemplate::Pointer> it(vec);

    while (it.hasNext()) {
        const EventTemplate::Pointer & ptr = it.next();
        if (ptr->getId() == id) {
            retVal = ptr;
            break;
        }
    }
    return retVal;
}

