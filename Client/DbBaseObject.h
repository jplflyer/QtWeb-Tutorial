#pragma once

#include "JSON_Serializable.h"

/**
 * This is the base class for all database objects.
 */
class DbBaseObject: public JSON_Serializable
{
private:
    int		id = 0;

public:
    DbBaseObject() = default;

    int getId() const		{ return id; }
    void setId(int value)	{ id = value; }

    void fromJSON(const QJsonObject & json);
    void toJSON(QJsonObject & json) const;
};
