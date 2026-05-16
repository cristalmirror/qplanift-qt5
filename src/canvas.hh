#ifndef CANVAS_H
#define CANVAS_H

#include <QColor>
#include <QFrame>
#include <QTransform>
#include <QScrollBar>
#include <QPixmap>
#include <QPainter>
#include "EventosCls.hh"

#define ESPACIO_REGLA  40

class Pizarra_Cls: public QFrame {
  Q_OBJECT
public:
  QPixmap buffer;
  QColor  bgColor;

  Pizarra_Cls(QWidget *parent=0, QColor bg=Qt::white)
    : QFrame(parent), bgColor(bg) {}

  void initBuffer() {
    QSize sz = size().expandedTo(QSize(1,1));
    buffer = QPixmap(sz);
    buffer.fill(bgColor);
  }

protected:
  void paintEvent(QPaintEvent *p) {
    QPainter painter(this);
    if (!buffer.isNull())
      painter.drawPixmap(0, 0, buffer);
    QFrame::paintEvent(p);
  }

  void resizeEvent(QResizeEvent *e) {
    QFrame::resizeEvent(e);
    initBuffer();
  }
};


class Canvas : public QWidget {
    Q_OBJECT

protected:
    void paintEvent( QPaintEvent * );
    virtual void resizeEvent( QResizeEvent * );

public slots:
  void Dibuja (const evento_c &);
  void Dibuja (int, const char *, const char *tip=0);
  void Limpia ();

  void setZoom(int);
  void setGrid(int);
  void setScroll(int);

public:
  Canvas(QWidget *parent=0, const char *name=0);
  void finalizado_dibujo();

private:
  void Dibuja();

  QTransform mat_escalado;
  int grid;
  int zoom;
  float scroll;
  Pizarra_Cls *area_de_nombres;
  Pizarra_Cls *area_de_cronograma;

  QScrollBar *hscroll;

  int final_simulacion;

signals:
  void hayQueRedibujar(bool);
};

#endif
