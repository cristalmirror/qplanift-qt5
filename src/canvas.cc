#include "tipos.h"
#include "input.tab.h"
#include "canvas.hh"
#include <QApplication>
#include <QMessageBox>
#include <QPixmap>
#include <QFrame>
#include <QScrollBar>
#include <QFileDialog>
#include <QPoint>
#include <QColor>
#include <QPen>
#include <QPainter>
#include <QToolTip>
#include <math.h>

Canvas::Canvas( QWidget *parent, const char *name )
  : QWidget( parent )
{
  Q_UNUSED(name);
  grid   = 1;
  scroll = 0;
  zoom   = 20;
  final_simulacion = 0;

  area_de_nombres = new Pizarra_Cls(this, Qt::lightGray);
  area_de_nombres->setFrameStyle( QFrame::Box | QFrame::Raised );
  area_de_nombres->setGeometry(0, 0, 100, height());
  area_de_nombres->setMidLineWidth(2);
  area_de_nombres->initBuffer();

  area_de_cronograma = new Pizarra_Cls(this, Qt::white);
  area_de_cronograma->setGeometry(100, 0, width()-100, height());
  area_de_cronograma->setMidLineWidth(2);
  area_de_cronograma->initBuffer();

  hscroll = new QScrollBar(Qt::Horizontal, area_de_cronograma);
  Q_CHECK_PTR( hscroll );

  connect( hscroll, SIGNAL(valueChanged(int)), SLOT(setScroll(int)));
}


void Canvas::resizeEvent( QResizeEvent * ) {
  area_de_nombres->setGeometry(0, 0, 100, height());
  area_de_cronograma->setGeometry(100, 0, width()-100, height());
  hscroll->setGeometry(0, height()-16, width()-100, 16);
  area_de_nombres->initBuffer();
  area_de_cronograma->initBuffer();
  Limpia();
  emit hayQueRedibujar(true);
}


void Canvas::setZoom(int z)   { zoom=z;       Limpia(); emit hayQueRedibujar(true); }
void Canvas::setScroll(int s) { scroll=-(float)s; Limpia(); emit hayQueRedibujar(true); }
void Canvas::setGrid(int g)   { grid=g;       Limpia(); emit hayQueRedibujar(true); }


void Canvas::Limpia() {
  if (!area_de_nombres->buffer.isNull()) {
    area_de_nombres->buffer.fill(Qt::lightGray);
    area_de_nombres->update();
  }
  if (!area_de_cronograma->buffer.isNull()) {
    area_de_cronograma->buffer.fill(Qt::white);
    area_de_cronograma->update();
  }

  area_de_nombres->setToolTip("");
  area_de_cronograma->setToolTip("");

  final_simulacion = 0;
}


void Canvas::Dibuja(int pos, const char *nombre, const char *tip) {
  if (area_de_nombres->buffer.isNull()) return;

  QPainter p(&area_de_nombres->buffer);
  p.drawText(4, ESPACIO_REGLA + 20 + 40*pos, nombre);
  p.end();
  area_de_nombres->update();

  if (tip)
    area_de_nombres->setToolTip(tip);
}


void Canvas::finalizado_dibujo() {
  if (area_de_cronograma->buffer.isNull()) return;

  QPainter p(&area_de_cronograma->buffer);
  int X = 0;
  QPoint Linea;
  char cade[100];

  mat_escalado.reset();
  mat_escalado.translate(scroll, ESPACIO_REGLA);
  mat_escalado.scale(zoom, 1.0);

  p.setPen( QPen(Qt::DotLine) );
  if (grid > 0) {
    for (int x=0; (x<10000) && (X <= area_de_cronograma->width()); x += grid) {
      Linea = mat_escalado.map(QPoint(x, (x%5)==0 ? -20 : -10));
      X = Linea.rx();

      if ((x%5) == 0) {
        sprintf(cade, "%d", x);
        p.drawText(X, 15, cade);
        p.setPen(QPen(Qt::DashDotLine));
        p.drawLine(Linea, QPoint(Linea.rx(), area_de_cronograma->height()));
        p.setPen( QPen(Qt::DotLine) );
      } else {
        p.drawLine(Linea, QPoint(Linea.rx(), area_de_cronograma->height()));
      }
    }
  }

  p.setPen( QPen(Qt::SolidLine) );
  for (int y=0; y < num_tareas; y++) {
    X = y*40 + 24 + ESPACIO_REGLA;
    p.drawLine(0, X, area_de_cronograma->width(), X);
  }
  p.end();
  area_de_cronograma->update();

  int r_max = ((final_simulacion+1) * zoom) - area_de_cronograma->width();
  r_max = r_max > 0 ? r_max : 0;

  hscroll->setRange(0, r_max);
  hscroll->setSingleStep(zoom);
  hscroll->setPageStep(zoom*10);
}


void Canvas::Dibuja(const evento_c &evento) {
  if (area_de_cronograma->buffer.isNull()) return;

  QPainter p(&area_de_cronograma->buffer);
  QRect r;

  static QColor gama_col[] = {
    Qt::cyan, Qt::darkRed, Qt::darkGreen, Qt::darkBlue,
    Qt::darkCyan, Qt::darkMagenta, Qt::darkYellow,
    Qt::lightGray, Qt::green, Qt::blue, Qt::red, Qt::magenta,
    Qt::yellow, Qt::black, Qt::white,
    Qt::darkGray, Qt::gray
  };

  QBrush c(gama_col[evento.Recurso() % 17]);

  mat_escalado.reset();
  mat_escalado.translate(scroll, ESPACIO_REGLA);
  mat_escalado.scale(zoom, 1.0);

  int ultimo_punto = evento.Inicio();
  if (ultimo_punto > final_simulacion)
    final_simulacion = ultimo_punto;

  QPoint point(evento.Inicio(), evento.Identificador()*40);
  point = mat_escalado.map(point);

  if (point.x() > area_de_cronograma->width()) return;

  p.setPen( QPen(Qt::SolidLine) );
  p.setBrush( c );

  switch(evento.Tipo()) {
  case E_llegada: {
    r.setRect(point.x()-7, point.y()+20, 15, 15);
    p.drawPie(r, 3840, 960);
    if (evento.Recurso() != 0) {
      p.setPen((QColor)Qt::black);
      p.setFont( QFont("helvetica", 8) );
      p.drawText(point.x(), point.y()+9, Recursos[evento.Recurso()].nombre);
    }
    break;
  }
  case E_activacion: {
    r.setRect(evento.Inicio(),
              evento.Identificador()*40 + 10,
              evento.Fin() - evento.Inicio(),
              15);
    QRect r_esca = mat_escalado.mapRect(r);
    p.drawRect(r_esca);
    break;
  }
  case E_final: {
    r.setRect(point.x()-7, point.y(), 15, 15);
    p.drawPie(r, 960, 960);
    break;
  }
  }

  p.end();
  area_de_cronograma->update();
}


void Canvas::Dibuja() {}


void Canvas::paintEvent( QPaintEvent * ) {}
