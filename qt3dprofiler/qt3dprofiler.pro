TEMPLATE = app
TARGET = qt3dprofiler

QT += quick

SOURCES += main.cpp \
    jobstatsreader.cpp

HEADERS += \
    listmodel.h \
    jobstatsreader.h

RESOURCES += \
    qml.qrc
