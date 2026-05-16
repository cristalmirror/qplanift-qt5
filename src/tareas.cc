#include "tipos.h"
#include "tareas.hh"

#include <QMessageBox>
#include <QFrame>
#include <QScrollBar>
#include <QFileDialog>
#include <QColor>
#include <QPen>
#include <QPainter>

int TareaViewItem::contador = 0;

Tareas_ventana::Tareas_ventana() : QTreeWidget(NULL) {
  QTreeWidgetItem *tmp, *sub;
  char cade[100];

  setHeaderLabels(QStringList() << "             Atributo" << "Valor");
  setColumnWidth(0, 200);
  setColumnWidth(1, 50);
  setRootIsDecorated(true);
  setFixedWidth(250);

  for (int x=0; x < num_tareas; x++) {
    tmp = new QTreeWidgetItem(this, QStringList(Tareas[x].nombre));

    sprintf(cade, "%d", Tareas[x].prioridad);
    new TareaViewItem(tmp, "Prioridad", cade);

    sprintf(cade, "%d", Tareas[x].periodo);
    new TareaViewItem(tmp, "Periodo", cade);

    sprintf(cade, "%d", Tareas[x].llegada);
    new TareaViewItem(tmp, "Llegada", cade);

    sub = new QTreeWidgetItem(tmp, QStringList("Secuencia de Ejecución"));
    for (int y=0; y < Tareas[x].Nsubtareas; y++) {
      char str_computo[100];
      sprintf(str_computo, "%d", Tareas[x].subtarea[y].tiempo);
      if (Tareas[x].subtarea[y].recurso == 0) {
        new TareaViewItem(sub, "CPU", str_computo);
      } else {
        new TareaViewItem(sub, Recursos[Tareas[x].subtarea[y].recurso].nombre, str_computo);
      }
    }
  }

  show();
}

Tareas_ventana::~Tareas_ventana() {}
