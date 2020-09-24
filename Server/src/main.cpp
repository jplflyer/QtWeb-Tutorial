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

    //----------------------------------------------------------------------
    // Create a demo user.
    //----------------------------------------------------------------------
    User::Vector allUsers;

    cout << "Load users." << endl;
    cout.flush();
    db.getUsers(allUsers);

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

    //----------------------------------------------------------------------
    // Create event templates.
    //----------------------------------------------------------------------
    EventTemplate::Vector allTemplates;
    EventTemplate::Pointer etICSCloudOnly = nullptr;
    EventTemplate::Pointer etICS = nullptr;
    EventTemplate::Pointer etZoom = nullptr;

    cout << "Load Event Templates." << endl;
    cout.flush();
    db.getEventTemplates(allTemplates);

    if (allTemplates.size() == 0) {
        cout << "Must create templates." << endl;
        cout.flush();

        etICSCloudOnly = std::make_shared<EventTemplate>("ICS Cloud Only");
        db.createEventTemplate(etICSCloudOnly);

        etICS = std::make_shared<EventTemplate>("ICS Cloud and Classic");
        db.createEventTemplate(etICS);

        etZoom = std::make_shared<EventTemplate>("Zoom Meeting");
        db.createEventTemplate(etZoom);
    }
    else {
        for (EventTemplate::Pointer &ptr: allTemplates) {
            string name = ptr->getName();
            if (name == "ICS Cloud Only") {
                etICSCloudOnly = ptr;
            }
            else if (name == "ICS Cloud and Classic") {
                etICS = ptr;
            }
            else if (name == "Zoom Meeting") {
                etZoom = ptr;
            }
        }
    }

    //----------------------------------------------------------------------
    // Create some sample events.
    //----------------------------------------------------------------------
    Event::Vector existingEvents;

    if (existingEvents.size() == 0) {
        Event::Pointer icsCloudMeeting = std::make_shared<Event>("Cloudy Ideas", etICSCloudOnly->getId());
        Event::Pointer icsZoomMeeting = std::make_shared<Event>("Zoom Ahead", etZoom->getId());
        Event::Pointer icsTeslaMeeting = std::make_shared<Event>("Tesla Beats Mazda!", etZoom->getId());

        db.createEvent(icsCloudMeeting);
        db.createEvent(icsZoomMeeting);
        db.createEvent(icsTeslaMeeting);
    }
}
