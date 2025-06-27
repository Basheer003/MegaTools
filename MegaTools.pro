# MegaConverter.pro
# QMake project file for the MegaConverter application.

QT += core gui widgets

# The greatest major version of Qt supported.
GREATER_QT_MAJOR_VERSION = 5

# Default is C++11. For modern C++ features, you can set a higher standard.
CONFIG += c++17

# Project sources
SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

# User interface file
FORMS += \
    mainwindow.ui

# The application name that will be displayed
TARGET = MegaTools

# Default rules for deployment
#include(deployment.pri)
