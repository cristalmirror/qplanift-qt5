QT += core gui widgets

CONFIG += c++11

TARGET = qplanif
TEMPLATE = app

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
    planificador.hh \
    canvas.hh \
    editor.hh \
    ayuda.hh \
    tareas.hh \
    tipos.h \
    CargaCls.hh \
    ColasCls.hh \
    DescripcionCls.hh \
    EventosCls.hh \
    PlanifFCFS.hh \
    PlanifPF.hh \
    PlanifPFNE.hh \
    PlanifPFRR.hh \
    PlanifRR.hh \
    PlanifSJF.hh \
    PlanifSRTF.hh \
    PlanificadorCls.hh \
    ProcesadorCls.hh \
    Simulador.hh \
    SistemaCls.hh \
    PlanifHRRN.hh \
    PlanifPFAging.hh \
    PlanifMLFQ.hh \
    input_tab.h
LIBS += -lfl
