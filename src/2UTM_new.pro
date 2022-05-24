QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    control.cpp \
    deleteutm.cpp \
    installgui.cpp \
    installutm.cpp \
    loader.cpp \
    logging.cpp \
    main.cpp \
    mainwindow.cpp \
    showreadernotcontext.cpp \
    startutm.cpp \
    stoputm.cpp

HEADERS += \
    common.h \
    control.h \
    deleteutm.h \
    installgui.h \
    installutm.h \
    loader.h \
    logging.h \
    mainwindow.h \
    pki-core-cpp.h \
    showreadernotcontext.h \
    startutm.h \
    stoputm.h

FORMS += \
    installgui.ui \
    loader.ui \
    mainwindow.ui \
    showreadernotcontext.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -lWinscard

LIBS += -lws2_32 \
        d:\2UTM_new\pki-core.dll \
        d:\2UTM_new\rtpkcs11ecp.dll \

win32 {
    RC_FILE = resource.rc
    CONFIG -= embed_manifest_exe # эта строка обязательна если у Вас MSVC
    # для того чтобы подавить генерацию манифеста по умолчанию (он подменяет,
    # указанный нами в ресурсах)
}

RESOURCES +=
    RC_FILE = file.rc
