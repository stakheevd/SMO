QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets	printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Buffer.cpp \
    BufferManager.cpp \
    Consumer.cpp \
    ConsumerManager.cpp \
    Logger.cpp \
    LoggerManager.cpp \
    Producer.cpp \
    ProducerManager.cpp \
    Request.cpp \
    Simulator.cpp \
    WaveformGenerator.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    Buffer.h \
    BufferManager.h \
    Consumer.h \
    ConsumerManager.h \
    Logger.h \
    LoggerManager.h \
    Producer.h \
    ProducerManager.h \
    Request.h \
    Simulator.h \
    TestClass.h \
    WaveformGenerator.h \
    mainwindow.h \
    qcustomplot.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
