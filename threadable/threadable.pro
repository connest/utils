QT -= gui


CONFIG += console
CONFIG += c++11
#CONFIG += optimize_full

QMAKE_CXXFLAGS += -pedantic-errors -Wall -Wextra

SOURCES += \
        main.cpp

HEADERS += \
    can_used_in_slot.h \
    in_other_thread.h \
    worker.h
