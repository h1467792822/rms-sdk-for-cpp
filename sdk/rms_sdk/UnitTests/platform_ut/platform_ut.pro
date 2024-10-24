REPO_ROOT = $$PWD/../../../..
DESTDIR   = $$REPO_ROOT/bin/tests
TARGET    = rmsplatformUnitTests

TEMPLATE  = app

QT       += network xml xmlpatterns testlib
QT       -= gui

CONFIG   += console c++17 debug_and_release
CONFIG   -= app_bundle

INCLUDEPATH       += $$REPO_ROOT/sdk/rmscrypto_sdk/CryptoAPI
INCLUDEPATH       += /usr/include/libxml2
win32:INCLUDEPATH += $$REPO_ROOT/third_party/include

LIBS       += -L$$REPO_ROOT/bin -L$$REPO_ROOT/bin/rms -L$$REPO_ROOT/bin/rms/platform

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
    LIBS += -lplatformhttpd -lplatformjsond -lplatformfilesystemd -lplatformsettingsd -lrmscryptod -lrms -ludns -lcurl
} else {
    LIBS += -lplatformhttp -lplatformjson -lplatformfilesystem -lplatformsettings -lrmscrypto -lrms -ludns -lcurl
}

win32:LIBS += -L$$REPO_ROOT/third_party/lib/eay/ -lssleay32 -llibeay32 -lGdi32 -lUser32 -lAdvapi32
else:LIBS  += -lssl -lcrypto -lxml2

DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += \
    main.cpp \
    PlatformHttpClientTest.cpp \
    PlatformXmlTest.cpp \
    PlatformJsonArrayTest.cpp \
    PlatformJsonObjectTest.cpp \
    PlatformFileSystemTest.cpp \
    PlatformFileTest.cpp

HEADERS += \
    PlatformHttpClientTest.h \
    PlatformXmlTest.h \
    PlatformJsonArrayTest.h \
    PlatformJsonObjectTest.h \
    PlatformFileSystemTest.h \
    PlatformFileTest.h \
    TestHelpers.h
