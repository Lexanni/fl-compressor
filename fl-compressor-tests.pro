TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += O3

DEFINES += _CRT_SECURE_NO_WARNINGS

SOURCES += \
    fl_compressor.c \
    unit_tests.cpp \
    main.cpp

HEADERS += \
    fl_compressor.h

TARGET = fl-compressor-tests
