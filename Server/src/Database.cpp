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
    }
    catch (std::exception e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

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
    User::Pointer user = nullptr;

    pqxx::read_transaction transaction{*connection};

    pqxx::result results{ transaction.exec_prepared("user_by_id", id) };
    for (pqxx::row row: results) {
        user = makeUser(row);
    }

    return user;
}

/**
 * Get the user with this username.
 */
User::Pointer Database::getUser(const std::string &username) {
    User::Pointer user = nullptr;

    pqxx::read_transaction transaction{*connection};

    pqxx::result results{ transaction.exec_prepared("user_by_username", username) };
    for (pqxx::row row: results) {
        user = makeUser(row);
    }

    return user;
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
