TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt


CONFIG += console c++11
CONFIG += optimize_full

QMAKE_CXXFLAGS += -pedantic-errors -Wall -Wextra -Weffc++


SOURCES += \
        main.cpp

HEADERS += \
    xray.h
