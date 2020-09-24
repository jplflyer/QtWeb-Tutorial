#pragma once

#include "DbBaseObject.h"

class User: public DbBaseObject
{
private:
    QString		username;
    QString		cryptedPassword;
    QString		nameFirst;
    QString		nameLast;

public:
    User();

    void fromJSON(const QJsonObject & json);
    void toJSON(QJsonObject & json) const;

    const QString &	getUsername() const { return username; }
    const QString &	getCryptedPassword() const { return cryptedPassword; }
    const QString &	getNameFirst() const { return nameFirst; }
    const QString &	getNameLast() const { return nameLast; }

    User & setId(int value)	{ DbBaseObject::setId(value); return *this; }
    User & setUsername(const QString &value) { username = value; return *this; }
    User & setCryptedPassword(const QString &value) { cryptedPassword = value; return *this; }
    User & setNameFirst(const QString &value) { nameFirst = value; return *this; }
    User & setNameLast(const QString &value) { nameLast = value; return *this; }

    typedef QSharedPointer<User> Pointer;
    typedef QVector<QSharedPointer<User>> Vector;
};
