TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += _CRT_SECURE_NO_WARNINGS

SOURCES += \
        main.cpp \
    functions.cpp \
    unit_tests.cpp

HEADERS += \
    functions.h
