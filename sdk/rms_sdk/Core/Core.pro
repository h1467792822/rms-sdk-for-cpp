REPO_ROOT = $$PWD/../../..
DESTDIR   = $$REPO_ROOT/bin/rms
TARGET    = modcore

TEMPLATE = lib
CONFIG  += staticlib warn_on c++17 debug_and_release
QT      += core
QT      -= gui

INCLUDEPATH += $$REPO_ROOT/sdk/rmscrypto_sdk/CryptoAPI

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
}

SOURCES += ProtectionPolicy.cpp

HEADERS += ProtectionPolicy.h \
    FeatureControl.h
