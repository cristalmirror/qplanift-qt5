#include <QApplication>
#include "planificador.hh"

int main( int argc, char **argv ) {
    QApplication a( argc, argv );

    Planificador *mw = new Planificador(1);
    mw->setWindowTitle("qplanif-qt5 - Simulador de Planificación 2026");
    mw->show();

    if (argc > 1)
      mw->cargar(argv[1]);

    QObject::connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    return a.exec();
}

Editor *Planificador::editor = 0;
