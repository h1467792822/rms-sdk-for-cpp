REPO_ROOT = $$PWD/../../../..
DESTDIR   = $$REPO_ROOT/bin/rms/platform
TARGET    = platformxml

QT       += xml xmlpatterns
QT       -= gui

TEMPLATE = lib
CONFIG += staticlib c++17 debug_and_release

DEFINES += QTFRAMEWORK

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
}

SOURCES += \
    DomDocumentQt.cpp \
    DomElementQt.cpp

HEADERS += \
    IDomDocument.h \
    IDomNode.h \
    IDomElement.h \
    DomNamedNodeMap.h \
    DomNodeList.h \
    DomDocumentQt.h \
    DomElementQt.h
