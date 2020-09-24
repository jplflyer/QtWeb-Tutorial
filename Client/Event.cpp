#include "Event.h"

void Event::fromJSON(const QJsonObject & json) {
    QJsonDocument doc(json);
    qDebug() << "Event::fromJSON from " << doc.toJson();

    DbBaseObject::fromJSON(json);

    name = stringValue(json, "name");
    eventTemplateId = intValue(json, "eventTemplateId");
}

void Event::toJSON(QJsonObject & json) const {
    DbBaseObject::toJSON(json);

    json["name"] = QJsonValue(name);
    json["eventTemplateId"] = eventTemplateId;
}
