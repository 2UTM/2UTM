QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    change_device.cpp \
    install.cpp \
    installgui.cpp \
    loader.cpp \
    logging.cpp \
    main.cpp \
    mainwindow.cpp \
    reconnect.cpp \
    setting.cpp \
    start.cpp \
    stop.cpp \
    uninstall.cpp

HEADERS += \
    change_device.h \
    common.h \
    install.h \
    installgui.h \
    loader.h \
    logging.h \
    mainwindow.h \
    pki-core-cpp.h \
    reconnect.h \
    setting.h \
    start.h \
    stop.h \
    uninstall.h

FORMS += \
    change_device.ui \
    installgui.ui \
    loader.ui \
    mainwindow.ui \
    setting.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -lws2_32 \
        d:\kontrolnie_raboti\test_UTM\2UTM\pki-core.dll \
        d:\kontrolnie_raboti\test_UTM\2UTM\rtpkcs11ecp.dll \

win32: LIBS += -lcfgmgr32
win32 {
    RC_FILE = resource.rc
    CONFIG -= embed_manifest_exe # эта строка обязательна если у Вас MSVC
    # для того чтобы подавить генерацию манифеста по умолчанию (он подменяет,
    # указанный нами в ресурсах
}
