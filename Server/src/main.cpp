#include <iostream>
#include <sstream>

#include <string>

#include "OptionHandler.h"
#include "DemoServer.h"
#include "Database.h"

using std::cout;
using std::endl;
using std::string;

static void seedDb();

int main(int argc, char **argv)
{
    bool doSeed = false;

    OptionHandler::ArgumentVector vec;
    vec.addNoArg("create",     [&](const char *)     { doSeed = true; }, "Seed the database.");

    if (!OptionHandler::handleOptions(argc, argv, vec)) {
        exit(0);
    }

    if (doSeed) {
        seedDb();
        exit(0);
    }

    DemoServer server(8500);

    server.setupRoutes();
    server.run();

    return 0;
}


/**
 * Seed the database.
 */
void seedDb() {
    Database db;
    string url = "postgres://webdemo:webdemo-asdf@localhost:5432/webdemo";

    cout << "Connect to the DB." << endl;
    db.connect(url);
    cout.flush();

    User::Vector allUsers;

    cout << "Load users." << endl;
    db.getUsers(allUsers);
    cout.flush();

    if (allUsers.size() == 0) {
        cout << "Must create users." << endl;
        cout.flush();

        string pw{"demo"};
        string cryptedString = Database::simpleSHA256(pw);

        cout << "Encrypted passwd: " << cryptedString << endl;
        cout.flush();

        User::Pointer user = std::make_shared<User>();
        user->setUsername("demo")
                .setCryptedPassword(cryptedString)
                .setNameFirst("Friendly")
                .setNameLast("Demo");

        db.createUser(user);
    }
}
