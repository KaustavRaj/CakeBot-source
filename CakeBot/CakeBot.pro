QT += quick
QT += core network
QT += widgets
QT += gui
QT += sql
QT += qml quick
QT += qml quick sql
QT += printsupport
QT += widgets texttospeech

CONFIG += c++11

# Line below is for icon of the software
RC_ICONS = ../CakeBot/pic/logo_256x256.ico

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        WakeUpTheBot.cpp \
        botcontact.cpp \
        cancelorder.cpp \
        database.cpp \
        listcheck.cpp \
        main.cpp \
        printreceipt.cpp \
        think.cpp \
        voice.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

OTHER_FILES += \
    price table/candle.csv \
    price table/flavour.csv \
    price table/fruit.csv \
    price table/topping.csv

HEADERS += \
    WakeUpTheBot.h \
    botcontact.h \
    cancelorder.h \
    database.h \
    listcheck.h \
    printreceipt.h \
    think.h \
    voice.h

DISTFILES +=

