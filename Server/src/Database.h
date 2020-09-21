#pragma once

#include <pqxx/pqxx>

#include "User.h"

/**
 * Interface to our database.
 */
class Database
{
private:
    pqxx::connection *	connection = nullptr;

    User::Pointer makeUser(pqxx::row &);

public:
    static std::string simpleSHA256(const std::string &str);

    Database() = default;
    virtual ~Database();

    void connect(const std::string &dbURL);


    void getUsers(User::Vector &);
    User::Pointer getUser(int id);
    User::Pointer getUser(const std::string &username);

    void createUser(User::Pointer user);
};
