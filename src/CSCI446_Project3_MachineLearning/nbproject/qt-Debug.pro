# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Debug/GNU-Linux
TARGET = CSCI446_Project3_MachineLearning
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += debug 
PKGCONFIG +=
QT = core gui widgets
SOURCES += cancer.cpp data.cpp glass.cpp graph.cpp id3.cpp iris.cpp learn.cpp main.cpp naive_bayes.cpp nearest_neighbor.cpp record.cpp soybean.cpp tan.cpp vote.cpp
HEADERS += cancer.h data.h glass.h graph.h id3.h iris.h learn.h main.h naive_bayes.h nearest_neighbor.h record.h soybean.h system.h tan.h typedef.h vote.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/GNU-Linux
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += 
LIBS += 
equals(QT_MAJOR_VERSION, 4) {
QMAKE_CXXFLAGS += -std=c++11
}
equals(QT_MAJOR_VERSION, 5) {
CONFIG += c++11
}
