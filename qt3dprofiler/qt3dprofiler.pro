TEMPLATE = app
TARGET = qt3dprofiler

QT += quick

SOURCES += main.cpp \
    jobstatsreader.cpp \
    debuggerconnection.cpp \
    commandresultreceiver.cpp \
    datamodels.cpp \
    rendercommandparser.cpp \
    profiler.cpp

HEADERS += \
    listmodel.h \
    jobstatsreader.h \
    debuggerconnection.h \
    commandresultreceiver.h \
    datamodels.h \
    rendercommandparser.h \
    profiler.h

RESOURCES += \
    qml.qrc
