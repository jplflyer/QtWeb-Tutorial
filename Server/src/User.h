#pragma once

#include <string>
#include <vector>

#include "DbBaseObject.h"

class User: public DbBaseObject
{
private:
    std::string		username;
    std::string		cryptedPassword;
    std::string		nameFirst;
    std::string		nameLast;

public:
    User();

    void fromJSON(const JSON & json);
    void toJSON(JSON & json) const;

    const std::string &	getUsername() const { return username; }
    const std::string &	getCryptedPassword() const { return cryptedPassword; }
    const std::string &	getNameFirst() const { return nameFirst; }
    const std::string &	getNameLast() const { return nameLast; }

    User & setId(int value)	{ DbBaseObject::setId(value); return *this; }
    User & setUsername(const std::string &value) { username = value; return *this; }
    User & setCryptedPassword(const std::string &value) { cryptedPassword = value; return *this; }
    User & setNameFirst(const std::string &value) { nameFirst = value; return *this; }
    User & setNameLast(const std::string &value) { nameLast = value; return *this; }

    typedef std::shared_ptr<User> Pointer;
    typedef JSON_Serializable_Vector<User> Vector;

};
