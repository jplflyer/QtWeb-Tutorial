#include "User.h"

User::User()
{
}

void User::fromJSON(const JSON & json) {
    DbBaseObject::fromJSON(json);

    username = stringValue(json, "username");
    cryptedPassword = stringValue(json, "cryptedPassword");
    nameFirst = stringValue(json, "nameFirst");
    nameLast = stringValue(json, "nameLast");
}

void User::toJSON(JSON & json) const {
    DbBaseObject::toJSON(json);

    json["username"] = username;
    json["cryptedPassword"] = cryptedPassword;
    json["nameFirst"] = nameFirst;
    json["nameLast"] = nameLast;
}
