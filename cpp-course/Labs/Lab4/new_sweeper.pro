QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    buttongrid.cpp \
    game.cpp \
    language_dialog.cpp \
    main.cpp \
    mainwindow.cpp \
    settings.cpp \
    settingsdialog.cpp \
    squarebutton.cpp

HEADERS += \
    buttongrid.h \
    game.h \
    language_dialog.h \
    mainwindow.h \
    settings.h \
    settingsdialog.h \
    squarebutton.h

TRANSLATIONS += \
    new_sweeper_en_US.ts \
    new_sweeper_ru_RU.ts \
    new_sweeper_sr_RS.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
