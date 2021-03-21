TEMPLATE = app
CONFIG += console c++17


QMAKE_CXXFLAGS += -pedantic-errors -Wall -Wextra -Weffc++


CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    MySingletonClass.h \
    singleton.h
