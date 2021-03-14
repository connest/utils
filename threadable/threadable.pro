QT -= gui

CONFIG -= app_bundle

CONFIG += console c++11
#CONFIG += optimize_full

QMAKE_CXXFLAGS += -pedantic-errors -Wall -Wextra -Weffc++

SOURCES += \
        main.cpp

HEADERS += \
    can_used_in_slot.h \
    in_other_thread.h \
    worker.h
