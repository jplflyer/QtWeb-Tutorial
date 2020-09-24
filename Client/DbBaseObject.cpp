#include "DbBaseObject.h"

void DbBaseObject::fromJSON(const QJsonObject & json) {
    id = intValue(json, "id");
}

void DbBaseObject::toJSON(QJsonObject & json) const {
    json["id"] = id;
}
