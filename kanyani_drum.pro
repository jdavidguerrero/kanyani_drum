QT += gui core qml quick quickcontrols2
CONFIG += c++11
TARGET = kanyani_drum
DEFINES += QT_DEPRECATED_WARNINGS

# Configuración de los directorios de salida
DESTDIR = output
OBJECTS_DIR = output/obj
MOC_DIR = output/moc
RCC_DIR = output/rcc
UI_DIR = output/ui

# Configuración de los archivos fuente y de cabecera
SOURCES += \
    src/main.cpp \
    src/controller/SensorController.cpp \
    src/utils/ads1115/ads1115.cpp \
    src/model/SensorData.cpp
    src/audio/AudioModule.cpp

HEADERS += \
    src/controller/SensorController.h \
    src/utils/ads1115/ads1115.h \
    src/model/SensorData.h
    src/audio/AudioModule.h

# Configuración de los recursos
RESOURCES += \
    qml.qrc

# Configuración de bibliotecas adicionales
LIBS += -lwiringPi -lrt -lrtaudio -lsndfile

# Configuración de rutas de inclusión
INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/src/controller
INCLUDEPATH += $$PWD/src/utils/ads1115
INCLUDEPATH += $$PWD/src/model
INCLUDEPATH += $$PWD/src/audio

# Rutas de importación adicionales para QML
QML_IMPORT_PATH =

# Rutas de importación para el Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =
