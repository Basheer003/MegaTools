#-------------------------------------------------
#
# Project created by QtCreator 2025-06-26T22:29:18
#
#-------------------------------------------------
QT += core gui widgets

GREATER_QT_MAJOR_VERSION = 5

CONFIG += c++17

TARGET = MegaTools
VERSION = 1.1.0


# Project sources, headers, and UI
SOURCES += main.cpp mainwindow.cpp
HEADERS += mainwindow.h
FORMS += mainwindow.ui

# Resources
RESOURCES += resources.qrc

# For Windows: link the icon and manifest file
win32 {
    RC_FILE = app.rc
}

# --- Build Directory Configuration ---
# Get rid of auto-generated debug/release folders in project root
CONFIG -= debug_and_release debug_and_release_target

# Define a root build directory
BUILD_DIR = $$_PRO_FILE_PWD_/build

# Set custom output directories based on build type
CONFIG(debug, debug|release) {
    DESTDIR = $$BUILD_DIR/debug
} else {
    DESTDIR = $$BUILD_DIR/release
}

# Ensure the build directory exists
!exists($$BUILD_DIR) {
    message("Creating build directory")
    system(mkdir $$BUILD_DIR)
}

# Set intermediate file directories
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui
