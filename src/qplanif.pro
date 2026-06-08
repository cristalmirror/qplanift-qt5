QT += core gui widgets

CONFIG += c++11

TARGET = qplanif
TEMPLATE = app

INCLUDEPATH += ../include

SOURCES += \
    qplanif.cc \
    planificador.cc \
    canvas.cc \
    editor.cc \
    ayuda.cc \
    tareas.cc \
    CargaCls.cc \
    DescripcionCls.cc \
    PlanifFCFS.cc \
    PlanifPF.cc \
    PlanifPFNE.cc \
    PlanifPFRR.cc \
    PlanifRR.cc \
    PlanifSJF.cc \
    PlanifSRTF.cc \
    ProcesadorCls.cc \
    Simulador.cc \
    PlanifHRRN.cc \
    PlanifPFAging.cc \
    PlanifMLFQ.cc \
    input_tab.c \
    linput.c

HEADERS += \
    ../include/planificador.hh \
    ../include/canvas.hh \
    ../include/editor.hh \
    ../include/ayuda.hh \
    ../include/tareas.hh \
    ../include/tipos.h \
    ../include/CargaCls.hh \
    ../include/ColasCls.hh \
    ../include/DescripcionCls.hh \
    ../include/EventosCls.hh \
    ../include/PlanifFCFS.hh \
    ../include/PlanifPF.hh \
    ../include/PlanifPFNE.hh \
    ../include/PlanifPFRR.hh \
    ../include/PlanifRR.hh \
    ../include/PlanifSJF.hh \
    ../include/PlanifSRTF.hh \
    ../include/PlanificadorCls.hh \
    ../include/ProcesadorCls.hh \
    ../include/Simulador.hh \
    ../include/SistemaCls.hh \
    ../include/PlanifHRRN.hh \
    ../include/PlanifPFAging.hh \
    ../include/PlanifMLFQ.hh \
    ../include/input_tab.h
LIBS += -lfl
