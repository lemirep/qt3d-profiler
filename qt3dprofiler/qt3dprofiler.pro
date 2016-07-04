TEMPLATE = app
TARGET = qt3dprofiler

QT += quick

SOURCES += main.cpp \
    jobstatsreader.cpp \
    debuggerconnection.cpp \
    commandresultreceiver.cpp \
    backendinterfacer.cpp \
    datamodels.cpp \
    rendercommandparser.cpp

HEADERS += \
    listmodel.h \
    jobstatsreader.h \
    debuggerconnection.h \
    commandresultreceiver.h \
    backendinterfacer.h \
    datamodels.h \
    rendercommandparser.h

RESOURCES += \
    qml.qrc
