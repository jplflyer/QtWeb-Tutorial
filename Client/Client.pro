QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

SOURCES += \
    BaseForm.cpp \
    DbBaseObject.cpp \
    Event.cpp \
    EventTemplate.cpp \
    JSON_Serializable.cpp \
    ListEventsForm.cpp \
    LoginForm.cpp \
    Network.cpp \
    NewEventForm.cpp \
    User.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    BaseForm.h \
    DbBaseObject.h \
    Event.h \
    EventTemplate.h \
    JSON_Serializable.h \
    ListEventsForm.h \
    LoginForm.h \
    MainWindow.h \
    Network.h \
    NewEventForm.h \
    User.h \
    date/chrono_io.h \
    date/date.h \
    date/ios.h \
    date/islamic.h \
    date/iso_week.h \
    date/julian.h \
    date/ptz.h \
    date/solar_hijri.h \
    date/tz.h \
    date/tz_private.h

FORMS += \
    ListEventsForm.ui \
    LoginForm.ui \
    MainWindow.ui \
    NewEventForm.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
