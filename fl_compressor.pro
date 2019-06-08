TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += O3

DEFINES += _CRT_SECURE_NO_WARNINGS
# DEFINES += UNIT_TESTS

SOURCES += \
        main.c \
    fl_compressor.c

HEADERS += \
    fl_compressor.h

TARGET = fl-compressor
