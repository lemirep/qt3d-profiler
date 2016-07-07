TEMPLATE = app
TARGET = qt3dprofiler

QT += quick concurrent

SOURCES += main.cpp \
    jobstatsreader.cpp \
    debuggerconnection.cpp \
    commandresultreceiver.cpp \
    datamodels.cpp \
    rendercommandparser.cpp \
    profiler.cpp \
    jobtraceview.cpp

HEADERS += \
    listmodel.h \
    jobstatsreader.h \
    debuggerconnection.h \
    commandresultreceiver.h \
    datamodels.h \
    rendercommandparser.h \
    profiler.h \
    jobtraceview.h

RESOURCES += \
    qml.qrc
