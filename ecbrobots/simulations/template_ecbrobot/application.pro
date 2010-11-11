
TEMPLATE = app
TARGET = start
DEPENDPATH += . ../.. ../../../selforg/include ../../messagedispatcher
INCLUDEPATH += /$(HOME)/include ../.. ../../../selforg/include  ../../messagedispatcher
LIBS += -L$(HOME)/lib  -lusb -lftdi -lselforg -L../../../selforg
CONFIG += debug 
QMAKE_CXXFLAGS += -Wno-deprecated -Wno-unused-parameter

OBJECTS_DIR = obj
MOC_DIR = moc


#Qt used libraries/functionalities
QT = core gui xml

# Input - Header

# Input - Sources
SOURCES 	+= main.cpp

#include all ecbrobots core files
include(../../ecbrobots.pro.inc)

#include all ecbrobots core resources
RESOURCES   += ecbrobots.qrc