TEMPLATE = app
CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle
QT+= core gui opengl
TARGET=little_planet_converter


SRC = ../src

HEADERS += \
    $$SRC/imageconverter.h

SOURCES += \
    $$SRC/main.cpp \
    $$SRC/imageconverter.cpp

RESOURCES += \
    $$SRC/shaders.qrc

