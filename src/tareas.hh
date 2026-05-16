#ifndef DIRVIEW_H
#define DIRVIEW_H

#include <QString>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "tipos.h"

class Tareas_ventana: public QTreeWidget {
  Q_OBJECT
public:
  Tareas_ventana();
  ~Tareas_ventana();
};

class TareaViewItem: public QTreeWidgetItem {
private:
  static int contador;
  int posicion;

public:
  TareaViewItem(QTreeWidgetItem *parent, const char *label1=0,
                const char *label2=0)
    : QTreeWidgetItem(parent, QStringList() << (label1?label1:"") << (label2?label2:""))
  {
    posicion = contador++;
  }

  virtual bool operator<(const QTreeWidgetItem &other) const {
    const TareaViewItem *o = dynamic_cast<const TareaViewItem*>(&other);
    return o ? posicion < o->posicion : false;
  }
};

#endif
