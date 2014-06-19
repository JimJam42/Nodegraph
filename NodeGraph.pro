TEMPLATE= lib
CONFIG += staticlib
TARGET=lib/NodeGraph

MOC_DIR=moc
OBJECTS_DIR = obj

UI_DIR=ui
UI_HEADERS_DIR=ui
QT+= opengl gui core

INC_DIR = include
SRC_DIR = src
FORM_DIR = ui
RESOURCE_DIR = res

DEBUG = 1
UNIBUILD = 1

isEqual(DEBUG,0){
    DEFINES += _RELEASE
}
isEqual(DEBUG,1){
    DEFINES += _DEBUG
}

INCLUDEPATH +=. $$INC_DIR $$FORM_DIR

unix:!macx{
    DEFINES += LINUX
}
macx:{
    DEFINES += DARWIN
}

QMAKE_CXXFLAGS_WARN_ON += "-Wno-unused-parameter"
QMAKE_CXXFLAGS_WARN_ON += "-Wno-unused-variable"
QMAKE_CXXFLAGS_WARN_ON += "-Wno-ignored-qualifiers"
QMAKE_CXXFLAGS_WARN_ON += "-Wno-overloaded-virtual"
QMAKE_CXXFLAGS_WARN_ON += "-Wno-reorder"
QMAKE_CXXFLAGS_WARN_ON += "-Wno-switch"
QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64

HEADERS+=   $$INC_DIR/GraphScene.h \
            $$INC_DIR/GraphNode.h \
            $$INC_DIR/GraphEdge.h \
            $$INC_DIR/NodeSocket.h \
            $$INC_DIR/Utilities.h \
            $$INC_DIR/NodeEdit.h

SOURCES +=  $$SRC_DIR/GraphScene.cpp \
            $$SRC_DIR/GraphNode.cpp \
            $$SRC_DIR/GraphEdge.cpp \
            $$SRC_DIR/NodeSocket.cpp \
            $$SRC_DIR/Utilities.cpp \
            $$SRC_DIR/NodeEdit.cpp
            
FORMS +=    $$FORM_DIR/NodeEdit.ui

macx:{
    QMAKE_BUNDLE_DATA += 
}

