#include "DbBaseObject.h"

using JSON = nlohmann::json;

void DbBaseObject::fromJSON(const JSON & json) {
    id = intValue(json, "id");
}

void DbBaseObject::toJSON(JSON & json) const {
    json["id"] = id;
}
