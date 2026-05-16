#include "planificador.hh"
#include <algorithm>

#include "SistemaCls.hh"
#include "EventosCls.hh"
#include "PlanifFCFS.hh"
#include "PlanifSJF.hh"
#include "PlanifSRTF.hh"
#include "PlanifPF.hh"
#include "PlanifRR.hh"
#include "PlanifPFRR.hh"

#include <QCheckBox>
#include <QPixmap>
#include <QToolBar>
#include <QToolButton>
#include <QMenu>
#include <QMenuBar>
#include <QFile>
#include <QFileDialog>
#include <QStatusBar>
#include <QMessageBox>
#include <QApplication>
#include <QTextStream>
#include <QPainter>
#include <QToolTip>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>
#include <QList>

#include "fileopen.xpm"
#include "tareas.xpm"

extern "C" {int Parsea_fichero(const char *);}

typedef QList<Planificador*> PlanificadorList;
static PlanificadorList *spawnedPlanificadores = 0;


Planificador::Planificador(int contador)
    : QMainWindow( 0 )
{
    QPixmap openIcon, tareaIcon;
    QComboBox *seleccion_planificadores;
    QComboBox *seleccion_recursos;
    int x;

    politica_planificador_actual = 0;
    politica_recurso_actual      = 0;
    quantum_actual               = 1;

    if (editor == 0)
      editor = new Editor(NULL, "Editor");

    if (num_tareas)
      Simulator.Simula(politica_planificador_actual, politica_recurso_actual,
                       Tareas, num_tareas, num_recursos, 500, quantum_actual, lst_eventos);

    canvas = new Canvas(this, "Canvas");
    canvas->setFocus();
    setCentralWidget( canvas );
    connect( canvas, SIGNAL(hayQueRedibujar(bool)), this, SLOT(actualiza(bool)) );

    openIcon  = QPixmap( fileopen );
    tareaIcon = QPixmap( tareas );

    // --- Toolbar: paso a paso ---
    pasoTools = addToolBar("Pasos");
    paso = new QCheckBox("Ejecución paso a paso");
    paso->setChecked(true);
    paso->setToolTip("Ejecución paso a paso\no ejecución completa");
    pasoTools->addWidget(paso);

    QLabel *lbl_instante = new QLabel("   Instante ");
    lbl_instante->setToolTip("Tiempo de Simulación");
    pasoTools->addWidget(lbl_instante);

    seleccion_paso = new QSpinBox();
    seleccion_paso->setRange(1, 10000);
    seleccion_paso->setSingleStep(1);
    seleccion_paso->setToolTip("Instante de Simulación");
    seleccion_paso->setEnabled(true);
    pasoTools->addWidget(seleccion_paso);

    connect( seleccion_paso, SIGNAL(valueChanged(int)), this, SLOT(setPaso(int)) );
    connect( paso, SIGNAL(toggled(bool)), seleccion_paso, SLOT(setEnabled(bool)) );
    connect( paso, SIGNAL(toggled(bool)), this, SLOT(togglePaso(bool)) );

    // --- Toolbar: controles ---
    controlTools = addToolBar("Herramientas");

    seleccion_planificadores = new QComboBox();
    char **NombrePlanificadores = Simulator.PlanifCPU();
    for (x=0; NombrePlanificadores[x]; x++)
      seleccion_planificadores->addItem( NombrePlanificadores[x] );
    connect( seleccion_planificadores, SIGNAL(activated(int)), SLOT(planificador_elegido(int)) );
    seleccion_planificadores->setToolTip("Selecciona el planificador\na visualizar");
    controlTools->addWidget(seleccion_planificadores);

    seleccion_recursos = new QComboBox();
    NombrePlanificadores = Simulator.PlanifRecursos();
    for (x=0; NombrePlanificadores[x]; x++)
      seleccion_recursos->addItem( NombrePlanificadores[x] );
    connect( seleccion_recursos, SIGNAL(activated(int)), SLOT(recurso_elegido(int)) );
    seleccion_recursos->setToolTip("Selecciona la política\nde gestión de recursos");
    controlTools->addWidget(seleccion_recursos);

    QLabel *lbl_quantum = new QLabel("   Quantum ");
    lbl_quantum->setToolTip("Ajuste Quantum");
    controlTools->addWidget(lbl_quantum);

    seleccion_quantum = new QSpinBox();
    seleccion_quantum->setRange(1, 100);
    seleccion_quantum->setSingleStep(1);
    seleccion_quantum->setToolTip("Quantum del Sistema");
    seleccion_quantum->setEnabled(false);
    connect( seleccion_quantum, SIGNAL(valueChanged(int)), this, SLOT(setQuantum(int)) );
    controlTools->addWidget(seleccion_quantum);

    QLabel *lbl_grid = new QLabel("   Grid ");
    lbl_grid->setToolTip("Ajuste del grid");
    controlTools->addWidget(lbl_grid);

    QSpinBox *grid = new QSpinBox();
    grid->setRange(0, 100);
    grid->setSingleStep(1);
    grid->setToolTip("Ajuste del grid");
    grid->setValue(1);
    grid->setSpecialValueText("No grid");
    connect( grid, SIGNAL(valueChanged(int)), canvas, SLOT(setGrid(int)) );
    controlTools->addWidget(grid);

    QLabel *lbl_zoom = new QLabel("   Zoom ");
    lbl_zoom->setToolTip("Ajuste del zoom");
    controlTools->addWidget(lbl_zoom);

    QSpinBox *zoom = new QSpinBox();
    zoom->setRange(0, 100);
    zoom->setSingleStep(1);
    zoom->setToolTip("Ajuste del zoom");
    zoom->setValue(20);
    connect( zoom, SIGNAL(valueChanged(int)), canvas, SLOT(setZoom(int)) );
    controlTools->addWidget(zoom);

    // --- Menú: Archivos ---
    QMenu *file = new QMenu("&Archivos", this);
    menuBar()->addMenu(file);
    file->addAction("&Nueva ventana", this, SLOT(newDoc()),     QKeySequence(Qt::CTRL+Qt::Key_N));
    file->addAction(openIcon, "&Abrir", this, SLOT(cargar()),   QKeySequence(Qt::CTRL+Qt::Key_A));
    file->addAction(tareaIcon, "&Editar", editor, SLOT(show()), QKeySequence(Qt::CTRL+Qt::Key_E));
    file->addSeparator();
    file->addAction("&Cerrar", this, SLOT(closeDoc()),          QKeySequence(Qt::CTRL+Qt::Key_W));
    file->addAction("&Salir",  qApp,  SLOT(quit()),             QKeySequence(Qt::CTRL+Qt::Key_Q));

    // --- Menú: Controles ---
    controls = new QMenu("&Controles", this);
    menuBar()->addMenu(controls);

    toggleHerrAction = controls->addAction("Barra de &Herramientas", this, SLOT(toggleHerrBar()),
                                           QKeySequence(Qt::CTRL+Qt::Key_H));
    toggleHerrAction->setCheckable(true);
    toggleHerrAction->setChecked(true);

    toggleStatusAction = controls->addAction("Barra de &Estado", this, SLOT(toggleStatusBar()),
                                             QKeySequence(Qt::CTRL+Qt::Key_E));
    toggleStatusAction->setCheckable(true);
    toggleStatusAction->setChecked(true);

    menuBar()->addSeparator();

    // --- Menú: Ayuda ---
    QMenu *ayuda = new QMenu("&Ayuda", this);
    menuBar()->addMenu(ayuda);
    ayuda->addAction("&Autores",          this, SLOT(mostrarAutores()));
    ayuda->addAction("&Temas de Ayuda",   this, SLOT(mostrarAyuda()), QKeySequence(Qt::Key_F1));
    ayuda->addSeparator();
    ayuda->addAction("Acerca de &Qt",     this, SLOT(aboutQt()));

    statusBar()->showMessage("Preparado");
    connect( this, SIGNAL(tareasNuevas(int)), this, SLOT(setTareasNuevas(int)) );
    connect( this, SIGNAL(ParseErrorEn(int)), editor, SLOT(RemarcaLinea(int)) );
    connect( editor, SIGNAL(re_carga(const char *)), this, SLOT(cargar(const char *)) );

    num_lect = contador;
}


Planificador::~Planificador() {
  if ( spawnedPlanificadores ) {
    spawnedPlanificadores->removeAll( this );
    if ( spawnedPlanificadores->count() == 0 ) {
      delete spawnedPlanificadores;
      spawnedPlanificadores = 0;
    }
  }
}


void Planificador::newDoc() {
  if ( !spawnedPlanificadores )
    spawnedPlanificadores = new PlanificadorList;

  Planificador *ed = new Planificador(num_lect);
  spawnedPlanificadores->append( ed );

  connect( this, SIGNAL(tareasNuevas(int)), ed,   SLOT(setTareasNuevas(int)) );
  connect( ed,   SIGNAL(tareasNuevas(int)), this, SLOT(setTareasNuevas(int)) );

  ed->show();
}


void Planificador::cargar(const char *nombre) {
  int linea_error = Parsea_fichero(nombre);

  if (y_error != -1) {
    statusBar()->showMessage("Carga Abortada");
    canvas->Limpia();
    num_tareas = 0;
    QMessageBox::warning(editor, "Error de carga", y_mensaje);
    emit ParseErrorEn(linea_error);
    return;
  }

  emit tareasNuevas(num_lect+1);
}


void Planificador::cargar() {
  QString fn = QFileDialog::getOpenFileName(this, "Abrir..", "", "Def files (*.def)");

  if ( fn.isEmpty() ) {
    statusBar()->showMessage("Carga Abortada");
    return;
  }

  QByteArray ba = fn.toLocal8Bit();
  Parsea_fichero(ba.data());
  editor->load(ba.data());

  if (y_error != -1) {
    statusBar()->showMessage("Carga Abortada");
    canvas->Limpia();
    num_tareas = 0;
    QMessageBox::warning(editor, "Error de carga", y_mensaje);
    return;
  }

  emit tareasNuevas(num_lect+1);
}


void Planificador::setTareasNuevas(int lecturas) {
  if (num_lect != lecturas) {
    num_lect = lecturas;
    Simulator.Simula(politica_planificador_actual, politica_recurso_actual,
                     Tareas, num_tareas, num_recursos, 500, quantum_actual, lst_eventos);
    actualiza(true);
    emit tareasNuevas(lecturas);
  }
}


struct dibuja_canvas {
  dibuja_canvas(Canvas &canvas, bool paso_a_paso=false, int pos=0)
    : cv(canvas), pap(paso_a_paso), fin(pos) {}

  void operator()(const evento_c &ev) const {
    if (!pap || ev.Inicio() < (tiempo_t)fin)
      cv.Dibuja(ev);
  }
  bool pap;
  int  fin;
  Canvas &cv;
};


void Planificador::actualiza(bool hay_que_limpiar) {
  char cade[1000];
  tiempo_t computo;

  if (!num_tareas) return;

  if (hay_que_limpiar)
    canvas->Limpia();

  for (int x=0; x < num_tareas; x++) {
    computo = 0;
    for (int y=0; y < Tareas[x].Nsubtareas; y++)
      computo += Tareas[x].subtarea[y].recurso ? 0 : Tareas[x].subtarea[y].tiempo;

    sprintf(cade, "Prio:\t%d\nLlegada:\t%d\nPerid:\t%d\nCómputo:\t%d",
            Tareas[x].prioridad,
            Tareas[x].llegada,
            Tareas[x].periodo,
            (int)computo);
    canvas->Dibuja(x, Tareas[x].nombre, cade);
  }

  if (paso->isChecked()) {
    for_each(lst_eventos.begin(), lst_eventos.end(),
             trocea_eventos(lst_ord_eventos));
    for_each(lst_ord_eventos.begin(), lst_ord_eventos.end(),
             dibuja_canvas(*canvas, paso->isChecked(), seleccion_paso->value()));
  } else {
    for_each(lst_eventos.begin(), lst_eventos.end(),
             dibuja_canvas(*canvas));
  }
  canvas->finalizado_dibujo();
}


void Planificador::setPaso(int) {
  Simulator.Simula(politica_planificador_actual, politica_recurso_actual,
                   Tareas, num_tareas, num_recursos, 500, quantum_actual, lst_eventos);
  actualiza(true);
}

void Planificador::togglePaso(bool) {
  Simulator.Simula(politica_planificador_actual, politica_recurso_actual,
                   Tareas, num_tareas, num_recursos, 500, quantum_actual, lst_eventos);
  actualiza(true);
}

void Planificador::setQuantum(int quant) {
  quantum_actual = quant;
  Simulator.Simula(politica_planificador_actual, politica_recurso_actual,
                   Tareas, num_tareas, num_recursos, 500, quantum_actual, lst_eventos);
  actualiza(true);
}

void Planificador::closeDoc() {
  close();
}

void Planificador::closeEvent(QCloseEvent *) {
  if ( spawnedPlanificadores && spawnedPlanificadores->indexOf(this) != -1 ) {
    delete this;
  } else {
    hide();
  }
}

void Planificador::toggleHerrBar() {
  if ( controlTools->isVisible() ) {
    controlTools->hide();
    pasoTools->hide();
    toggleHerrAction->setChecked(false);
  } else {
    controlTools->show();
    pasoTools->show();
    toggleHerrAction->setChecked(true);
  }
}

void Planificador::toggleStatusBar() {
  if ( statusBar()->isVisible() ) {
    statusBar()->hide();
    toggleStatusAction->setChecked(false);
  } else {
    statusBar()->show();
    toggleStatusAction->setChecked(true);
  }
}

void Planificador::planificador_elegido(int plan) {
  politica_planificador_actual = plan;
  seleccion_quantum->setEnabled( (plan==4) || (plan==7) );
  Simulator.Simula(politica_planificador_actual, politica_recurso_actual,
                   Tareas, num_tareas, num_recursos, 500, quantum_actual, lst_eventos);
  actualiza(true);
}

void Planificador::recurso_elegido(int rec) {
  politica_recurso_actual = rec;
  Simulator.Simula(politica_planificador_actual, politica_recurso_actual,
                   Tareas, num_tareas, num_recursos, 500, quantum_actual, lst_eventos);
  actualiza(true);
}

void Planificador::aboutQt() {
  QMessageBox::aboutQt( this, "Planificador desarrollado con:" );
}

void Planificador::mostrarAutores() {
  QMessageBox::about( this, "Planificador",
                      "Desarrollado por:\n"
                      "Ismael Ripoll y Sergio Saez\n"
                      "DISCA, UPV");
}

void Planificador::mostrarAyuda() {
  new AyudaCls(NULL, "Ayuda");
}
