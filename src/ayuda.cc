#include <stdlib.h>

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QPainter>
#include <QMessageBox>
#include <QList>
#include <QPaintDevice>
#include <QMenu>
#include <QMenuBar>
#include <QTextEdit>

#include "ayuda.hh"

const char* ayuda_general=
#include "General.help"
;

const char* ayuda_tareas=
#include "Tareas.help"
;

const char* ayuda_descripcion=
#include "Descripcion.help"
;

const char* ayuda_FCFS=
#include "FCFS.help"
;

const char* ayuda_RR=
#include "Round_Robin.help"
;

const char* ayuda_SJF=
#include "SJF.help"
;

const char* ayuda_prioridades=
#include "Prioridades.help"
;

const char* ayuda_prioridades_expulsivas=
#include "Prioridades_Expulsivas.help"
;

const char* ayuda_prioridades_RR=
#include "Prioridades_con_RR.help"
;

const char* ayuda_SRTF=
#include "SRTF.help"
;

typedef QList<AyudaCls*> AyudaClsList;
static AyudaClsList *spawnedAyudaClss = 0;

AyudaCls::AyudaCls( QWidget *parent, const char *name )
  : QWidget( parent )
{
  Q_UNUSED(name);

  m = new QMenuBar( this );
  Q_CHECK_PTR(m);

  QMenu *files = new QMenu("Archivos", this);
  Q_CHECK_PTR(files);
  m->addMenu(files);
  files->addAction("Nueva ventana", this, SLOT(newDoc()));
  files->addAction("Cerrar",        this, SLOT(closeDoc()));

  temas = new QMenu("Temas de Ayuda", this);
  Q_CHECK_PTR(temas);
  m->addMenu(temas);

  temas->addAction("General");          Ayudas["General"]                  = ayuda_general;
  temas->addAction("Tareas");           Ayudas["Tareas"]                   = ayuda_tareas;
  temas->addSeparator();
  temas->addAction("Descripción");      Ayudas["Descripción"]               = ayuda_descripcion;
  temas->addAction("FCFS");             Ayudas["FCFS"]                     = ayuda_FCFS;
  temas->addAction("SJF");              Ayudas["SJF"]                      = ayuda_SJF;
  temas->addAction("SRTF");             Ayudas["SRTF"]                     = ayuda_SRTF;
  temas->addAction("Round Robin");      Ayudas["Round Robin"]              = ayuda_RR;
  temas->addAction("Prioridades");      Ayudas["Prioridades"]              = ayuda_prioridades;
  temas->addAction("Prioridades Expulsivas"); Ayudas["Prioridades Expulsivas"] = ayuda_prioridades_expulsivas;
  temas->addAction("Prioridades + Round Robin"); Ayudas["Prioridades + Round Robin"] = ayuda_prioridades_RR;

  connect( temas, SIGNAL(triggered(QAction*)), SLOT(ayuda_elegida(QAction*)) );

  e = new QTextEdit( this );
  load(ayuda_general);

  e->setReadOnly(true);
  e->setFocus();
  resize( 400, 400 );
  show();
}

AyudaCls::~AyudaCls() {
  if ( spawnedAyudaClss ) {
    spawnedAyudaClss->removeAll( this );
    if ( spawnedAyudaClss->count() == 0 ) {
      delete spawnedAyudaClss;
      spawnedAyudaClss = 0;
    }
  }
}

void AyudaCls::newDoc() {
  if ( !spawnedAyudaClss )
    spawnedAyudaClss = new AyudaClsList;
  AyudaCls *ed = new AyudaCls;
  spawnedAyudaClss->append( ed );
  ed->resize( 400, 400 );
  ed->show();
}

void AyudaCls::load( const char *help_text ) {
  e->setPlainText( QString(help_text) );
}

void AyudaCls::closeDoc() {
  close();
}

void AyudaCls::resizeEvent( QResizeEvent * ) {
  if ( e && m )
    e->setGeometry( 0, m->height(), width(), height() - m->height() );
}

void AyudaCls::closeEvent( QCloseEvent * ) {
  if ( spawnedAyudaClss && spawnedAyudaClss->indexOf(this) != -1 ) {
    delete this;
  } else {
    hide();
  }
}

void AyudaCls::ayuda_elegida(QAction *action) {
  QByteArray ba = action->text().toLocal8Bit();
  const char *text = Ayudas[ba.data()];
  if (text)
    load(text);
}
