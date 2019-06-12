TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += _CRT_SECURE_NO_WARNINGS

SOURCES += \
        main.c \
    fl_compressor.c

HEADERS += \
    fl_compressor.h

TARGET = fl-compressor
