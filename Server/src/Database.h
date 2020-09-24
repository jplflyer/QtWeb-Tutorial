#pragma once

#include <pqxx/pqxx>

#include "User.h"
#include "EventTemplate.h"
#include "Event.h"

/**
 * Interface to our database.
 */
class Database
{
private:
    pqxx::connection *	connection = nullptr;

    User::Pointer makeUser(pqxx::row &);
    EventTemplate::Pointer makeEventTemplate(pqxx::row &);
    Event::Pointer makeEvent(pqxx::row &);

public:
    static std::string simpleSHA256(const std::string &str);

    Database() = default;
    virtual ~Database();

    void connect(const std::string &dbURL);

    //--------------------------------------------------
    // Users.
    //--------------------------------------------------
    void getUsers(User::Vector &);
    User::Pointer getUser(int id);
    User::Pointer getUser(const std::string &username);

    void createUser(User::Pointer user);

    //--------------------------------------------------
    // Event templates.
    //--------------------------------------------------
    void getEventTemplates(EventTemplate::Vector &);
    EventTemplate::Pointer getEventTemplate(int id);
    EventTemplate::Pointer getEventTemplate(const std::string &name);

    void createEventTemplate(EventTemplate::Pointer eventTemplate);

    //--------------------------------------------------
    // Events.
    //--------------------------------------------------
    void getEvents(Event::Vector &);
    Event::Pointer getEvent(int id);
    Event::Pointer getEvent(const std::string &name);

    void createEvent(Event::Pointer event);
};
