#ifndef APPLICATION_H
#define APPLICATION_H

#include <QMainWindow>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QToolBar>
#include <QMenu>
#include <QAction>
#include "tipos.h"

#include "canvas.hh"
#include "SistemaCls.hh"
#include "EventosCls.hh"

#include "PlanifFCFS.hh"
#include "PlanifSJF.hh"
#include "PlanifSRTF.hh"
#include "PlanifPF.hh"
#include "PlanifRR.hh"
#include "PlanifPFRR.hh"
#include "Simulador.hh"
#include "ayuda.hh"
#include "editor.hh"

class Planificador: public QMainWindow
{
    Q_OBJECT
public:
    Planificador(int contador);
    ~Planificador();

signals:
    void tareasNuevas(int);
    void ParseErrorEn(int);

public slots:
    void cargar(const char *);

private slots:
    void newDoc();
    void cargar();
    void closeDoc();

    void toggleStatusBar();
    void toggleHerrBar();
    void togglePaso(bool);

    void planificador_elegido(int);
    void recurso_elegido(int);
    void actualiza(bool hay=true);
    void setQuantum(int);
    void setPaso(int);
    void mostrarAyuda();
    void mostrarAutores();
    void setTareasNuevas(int);
    void aboutQt();

protected:
  void closeEvent( QCloseEvent * );

private:
    Canvas   *canvas;

    QToolBar *controlTools;
    QToolBar *pasoTools;
    QMenu    *controls;
    QAction  *toggleHerrAction;
    QAction  *toggleStatusAction;
    QCheckBox *paso;
    QSpinBox  *seleccion_quantum;
    QSpinBox  *seleccion_paso;

    int politica_planificador_actual;
    int politica_recurso_actual;
    int quantum_actual;

    static Editor *editor;

    SimuladorCls Simulator;

    EventosCls      lst_eventos;
    lista_eventos_c lst_ord_eventos;
    int num_lect;
};

#endif
