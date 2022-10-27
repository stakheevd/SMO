QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets	printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ProducerManager.cpp \
    Simulator.cpp \
    StatisticsManager.cpp \
    WaveformGenerator.cpp \
    buffer.cpp \
    buffermanager.cpp \
    consumer.cpp \
    consumer_manager.cpp \
    main.cpp \
    mainwindow.cpp \
    producer.cpp \
    producerlogger.cpp \
    qcustomplot.cpp \
    request.cpp

HEADERS += \
    ProducerManager.h \
    Simulator.h \
    StatisticsManager.h \
    TestClass.h \
    WaveformGenerator.h \
    buffer.h \
    buffermanager.h \
    consumer.h \
    consumer_manager.h \
    mainwindow.h \
    producer.h \
    producerlogger.h \
    qcustomplot.h \
    request.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
