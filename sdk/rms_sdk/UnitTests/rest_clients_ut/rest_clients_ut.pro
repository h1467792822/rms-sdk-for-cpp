REPO_ROOT = $$PWD/../../../..
DESTDIR   = $$REPO_ROOT/bin/tests
TARGET    = RestClientsUnitTests

TEMPLATE  = app

QT       += core network xml xmlpatterns testlib
QT       -= gui

CONFIG   += console c++17 debug_and_release
CONFIG   -= app_bundle

INCLUDEPATH       += $$REPO_ROOT/sdk/rmscrypto_sdk/CryptoAPI
INCLUDEPATH       += /usr/include/libxml2
win32:INCLUDEPATH += $$REPO_ROOT/third_party/include

LIBS       += -L$$REPO_ROOT/bin -L$$REPO_ROOT/bin/rms -L$$REPO_ROOT/bin/rms/platform 

CONFIG(debug, debug|release) {
   TARGET = $$join(TARGET,,,d)
    LIBS += -lmodprotectedfiled -lmodcored -lmodrestclientsd -lmodconsentd -lmodcommond -lmodjsond
    LIBS += -lplatformhttpd -lplatformjsond -lplatformfilesystemd -lplatformsettingsd
    LIBS += -lrmscryptod
    LIBS += -lrmsd -lxml2
} else {
    LIBS += -lmodprotectedfile -lmodcore -lmodrestclients -lmodconsent -lmodcommon -lmodjson    
    LIBS += -lplatformhttp -lplatformjson -lplatformfilesystem -lplatformsettings 
    LIBS += -lrmscrypto
    LIBS += -lrms -lxml2
}

win32:LIBS += -L$$REPO_ROOT/third_party/lib/eay/ -lssleay32 -llibeay32 -lGdi32 -lUser32 -lAdvapi32
else:LIBS  += -lssl -lcrypto

DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += \
    main.cpp \
    LicenseParserTest.cpp \
    LicenseParserTestConstants.cpp \

HEADERS += \
    LicenseParserTest.h \
    LicenseParserTestConstants.h \
    
