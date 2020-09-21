TEMPLATE = app

CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += /std:c++17

INCLUDEPATH += src includes /usr/local/include

SOURCES += \
    src/Database.cpp \
    src/DbBaseObject.cpp \
    src/DemoServer.cpp \
    src/JSON_Serializable.cpp \
    src/OptionHandler.cpp \
    src/RESTServer.cpp \
    src/StringMethods.cpp \
    src/URI.cpp \
    src/User.cpp \
    src/main.cpp

HEADERS += \
    src/Database.h \
    src/DbBaseObject.h \
    src/DemoServer.h \
    src/JSON_Serializable.h \
    src/OptionHandler.h \
    src/PointerVector.h \
    src/RESTServer.h \
    src/StringMethods.h \
    src/URI.h \
    src/UnitTesting.h \
    src/User.h \
    src/json.hpp
