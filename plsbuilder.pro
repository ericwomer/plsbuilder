TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    main/main.cpp \
    builder/builder.cpp

HEADERS += base/base.h \
    builder/builder.h

include(deployment.pri)

qtcAddDeployment()

