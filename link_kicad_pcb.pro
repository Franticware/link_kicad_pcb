TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        csv.cpp \
        kicad_pcb.cpp \
        main.cpp \
        proc_csv.cpp

HEADERS += \
    csv.h \
    kicad_pcb.h \
    proc_csv.h
