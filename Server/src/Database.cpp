#include <iostream>
#include <iomanip>
#include <openssl/sha.h>

#include "Database.h"

using std::cout;
using std::endl;

/**
 * This is unsalted, but for now, that's fine.
 */
std::string Database::simpleSHA256(const std::string &str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX context;

    if ( !SHA256_Init(&context) ) {
        return "";
    }

    if (!SHA256_Update(&context, str.c_str(), str.size()) ) {
        return "";
    }

    if (!SHA256_Final(hash, &context)) {
        return "";
    }

    std::stringstream ss;
    for (unsigned char value: hash) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)value;
    }

    return ss.str();
}

/**
 * Destructor.
 */
Database::~Database()
{
    if (connection != nullptr) {
        delete connection;
        connection = nullptr;
    }
}

/**
 * Connect to the database.
 */
void Database::connect(const std::string &dbURL) {
    connection = new pqxx::connection(dbURL);

    //----------------------------------------------------------------------
    // Set set up prepared statements for the various SELECT statements we
    // do. There should be no direct SQL anywhere else.
    //
    // Note: user is a reserved word, so I named the table member.
    //----------------------------------------------------------------------
    try {
        connection->prepare("user_all", "SELECT id, username, crypted_password, name_first, name_list FROM member ORDER BY id");
        connection->prepare("user_by_id", "SELECT id, username, crypted_password, name_first, name_list FROM member WHERE id = $1");
        connection->prepare("user_by_username", "SELECT id, username, crypted_password, name_first, name_list FROM member WHERE username = $1");
        connection->prepare("user_seq", "SELECT nextval('member_seq')");
        connection->prepare("user_create", "INSERT INTO member (id, username, crypted_password, name_first, name_list) VALUES ($1, $2, $3, $4, $5)");

        connection->prepare("event_template_all", "SELECT id, name FROM event_template ORDER BY id");
        connection->prepare("event_template_by_id", "SELECT id, name FROM event_template WHERE id = $1");
        connection->prepare("event_template_by_name", "SELECT id, name FROM event_template WHERE name = $1");
        connection->prepare("event_template_seq", "SELECT nextval('event_template_seq')");
        connection->prepare("event_template_create", "INSERT INTO event_template (id, name) VALUES ($1, $2)");

        connection->prepare("event_all", "SELECT id, name, event_template_id FROM event ORDER BY id");
        connection->prepare("event_by_id", "SELECT id, name, event_template_id FROM event WHERE id = $1");
        connection->prepare("event_by_name", "SELECT id, name, event_template_id FROM event WHERE name = $1");
        connection->prepare("event_seq", "SELECT nextval('event_seq')");
        connection->prepare("event_create", "INSERT INTO event (id, name, event_template_id) VALUES ($1, $2, $3)");
    }
    catch (std::exception e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

//======================================================================
// Methods related to users.
//======================================================================

/**
 * Create a user from this row.
 */
User::Pointer Database::makeUser(pqxx::row &row) {
    User::Pointer user = std::make_shared<User>();

    user->setId(				row[0].as<int>() )
        .setUsername(			row[1].c_str() )
        .setCryptedPassword(	row[2].c_str() )
        .setNameFirst(			row[3].c_str() )
        .setNameLast(			row[4].c_str() )
        ;

    return user;
}

/**
 * Get all users.
 */
void Database::getUsers(User::Vector &vec) {
    pqxx::read_transaction transaction{*connection};

    pqxx::result results{ transaction.exec_prepared("user_all") };
    for (pqxx::row row: results) {
        vec.push_back(makeUser(row));
    }
}

/**
 * Get the user with this PK.
 */
User::Pointer Database::getUser(int id) {
    User::Pointer retVal = nullptr;

    pqxx::read_transaction transaction{*connection};

    pqxx::result results{ transaction.exec_prepared("user_by_id", id) };
    for (pqxx::row row: results) {
        retVal = makeUser(row);
    }

    return retVal;
}

/**
 * Get the user with this username.
 */
User::Pointer Database::getUser(const std::string &username) {
    User::Pointer retVal = nullptr;

    pqxx::read_transaction transaction{*connection};

    pqxx::result results{ transaction.exec_prepared("user_by_username", username) };
    for (pqxx::row row: results) {
        retVal = makeUser(row);
    }

    return retVal;
}

void Database::createUser(User::Pointer user) {
    pqxx::work transaction{*connection};

    {
        pqxx::result results{ transaction.exec_prepared("user_seq") };
        for (pqxx::row row: results) {
            user->setId( row[0].as<int>() );
        }
        transaction.exec_prepared("user_create",
              user->getId(), user->getUsername(), user->getCryptedPassword(), user->getNameFirst(), user->getNameLast());
    }
    transaction.commit();
}

//======================================================================
// Methods related to Event Templates.
//======================================================================

/**
 * Create a Event Template object from this DB row.
 */
EventTemplate::Pointer Database::makeEventTemplate(pqxx::row &row) {
    EventTemplate::Pointer retVal = std::make_shared<EventTemplate>();

    retVal->setId(	row[0].as<int>() )
        .setName(	row[1].c_str() )
        ;

    return retVal;
}

/**
 * Get all Event Templates.
 */
void Database::getEventTemplates(EventTemplate::Vector &vec) {
    pqxx::read_transaction transaction{*connection};

    pqxx::result results{ transaction.exec_prepared("event_template_all") };
    for (pqxx::row row: results) {
        vec.push_back(makeEventTemplate(row));
    }
}

/**
 * Get the Event Template with this PK.
 */
EventTemplate::Pointer Database::getEventTemplate(int id) {
    EventTemplate::Pointer retVal = nullptr;

    pqxx::read_transaction transaction{*connection};

    pqxx::result results{ transaction.exec_prepared("event_template_by_id", id) };
    for (pqxx::row row: results) {
        retVal = makeEventTemplate(row);
    }

    return retVal;
}

/**
 * Get the Event Template with this name.
 */
EventTemplate::Pointer Database::getEventTemplate(const std::string &name) {
    EventTemplate::Pointer retVal = nullptr;

    pqxx::read_transaction transaction{*connection};

    pqxx::result results{ transaction.exec_prepared("event_template_by_name", name) };
    for (pqxx::row row: results) {
        retVal = makeEventTemplate(row);
    }

    return retVal;
}

/**
 * Create a new Event Template. Updates the record with the new PK.
 */
void Database::createEventTemplate(EventTemplate::Pointer et) {
    pqxx::work transaction{*connection};

    {
        pqxx::result results{ transaction.exec_prepared("event_template_seq") };
        for (pqxx::row row: results) {
            et->setId( row[0].as<int>() );
        }
        transaction.exec_prepared("event_template_create", et->getId(), et->getName());
    }
    transaction.commit();
}



//======================================================================
// Methods related to Events.
//======================================================================

/**
 * Create a Event object from this DB row.
 */
Event::Pointer Database::makeEvent(pqxx::row &row) {
    Event::Pointer retVal = std::make_shared<Event>();

    retVal->setId(	         row[0].as<int>() )
        .setName(	         row[1].c_str() )
        .setEventTemplateId( row[2].as<int>())
        ;

    return retVal;
}

/**
 * Get all Events.
 */
void Database::getEvents(Event::Vector &vec) {
    pqxx::read_transaction transaction{*connection};

    pqxx::result results{ transaction.exec_prepared("event_all") };
    for (pqxx::row row: results) {
        vec.push_back(makeEvent(row));
    }
}

/**
 * Get the Event  with this PK.
 */
Event::Pointer Database::getEvent(int id) {
    Event::Pointer retVal = nullptr;

    pqxx::read_transaction transaction{*connection};

    pqxx::result results{ transaction.exec_prepared("event_by_id", id) };
    for (pqxx::row row: results) {
        retVal = makeEvent(row);
    }

    return retVal;
}

/**
 * Get the Event  with this name.
 */
Event::Pointer Database::getEvent(const std::string &name) {
    Event::Pointer retVal = nullptr;

    pqxx::read_transaction transaction{*connection};

    pqxx::result results{ transaction.exec_prepared("event_by_name", name) };
    for (pqxx::row row: results) {
        retVal = makeEvent(row);
    }

    return retVal;
}

/**
 * Create a new Event . Updates the record with the new PK.
 */
void Database::createEvent(Event::Pointer et) {
    pqxx::work transaction{*connection};

    {
        pqxx::result results{ transaction.exec_prepared("event_seq") };
        for (pqxx::row row: results) {
            et->setId( row[0].as<int>() );
            break;
        }

        if (et->getEventTemplateId() == 0) {
            transaction.exec_prepared("event_create", et->getId(), et->getName(), nullptr);
        }
        else {
            transaction.exec_prepared("event_create", et->getId(), et->getName(), et->getEventTemplateId());
        }
    }
    transaction.commit();
}
