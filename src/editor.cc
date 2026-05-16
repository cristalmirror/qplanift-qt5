#include "editor.hh"
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QMenu>
#include <QTextStream>
#include <QPainter>
#include <QMessageBox>
#include <QPixmap>
#include <QTextCursor>
#include <QTextBlock>
#include "filesave.xpm"

Editor::Editor( QWidget *parent, const char *name )
    : QWidget( parent )
{
  Q_UNUSED(name);
  QPixmap saveIcon;

  m = new QMenuBar( this );
  s = new QStatusBar( this );

  QMenu *file = new QMenu("&Archivo", this);
  m->addMenu(file);

  saveIcon = QPixmap( filesave );

  file->addAction("&Nuevo",          this, SLOT(newFile()),  QKeySequence(Qt::CTRL+Qt::Key_N));
  file->addAction(saveIcon, "&Guardar", this, SLOT(save()),   QKeySequence(Qt::CTRL+Qt::Key_G));
  file->addAction("Guardar Como",    this, SLOT(saveAs()),   QKeySequence(Qt::CTRL+Qt::Key_C));
  file->addSeparator();
  file->addAction("Cerrar",          this, SLOT(close()),    QKeySequence(Qt::CTRL+Qt::Key_W));

  e = new Papel_t( this );
  e->setFocus();

  connect( e, SIGNAL(cursor_Changed()), this, SLOT(update_status()) );

  hide();
}

Editor::~Editor() {
  delete m;
  delete s;
  delete e;
  hide();
}

void Editor::load() {
  Nombre_fichero = QFileDialog::getOpenFileName(this);
  if ( !Nombre_fichero.isEmpty() )
    load( Nombre_fichero.toLocal8Bit().data() );
}

void Editor::newFile() {
  Nombre_fichero = "Nuevo.def";
  e->setPlainText("");
  setWindowTitle( Nombre_fichero );
  emit re_carga("");
}

void Editor::load( const char *fileName ) {
  QFile f( fileName );
  if ( !f.open( QIODevice::ReadOnly ) )
    return;

  Nombre_fichero = fileName;
  e->clear();

  QTextStream t(&f);
  e->setPlainText( t.readAll() );
  f.close();

  setWindowTitle( Nombre_fichero );
  update_status();
}

void Editor::save() {
  if (Nombre_fichero.isEmpty())
    saveAs();
  else {
    QFile f( Nombre_fichero );
    if ( !f.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
      return;
    QTextStream t(&f);
    t << e->toPlainText();
    f.close();
  }
  emit re_carga( Nombre_fichero.toLocal8Bit().data() );
}

void Editor::saveAs() {
  Nombre_fichero = QFileDialog::getSaveFileName(this, "Guardar Como..", "", "Def files (*.def)");
  if ( !Nombre_fichero.isEmpty() ) {
    save();
    setWindowTitle( Nombre_fichero );
  }
}

void Editor::close() {
  hide();
}

void Editor::resizeEvent( QResizeEvent * ) {
  e->setGeometry( 0, m->height(), width(), height() - m->height() - s->height() );
  s->setGeometry( 0, height() - s->height(), width(), s->height() );
}

void Editor::closeEvent( QCloseEvent * ) {
  hide();
}

void Editor::RemarcaLinea(int posicion) {
  QTextCursor cursor = e->textCursor();
  cursor.movePosition(QTextCursor::Start);
  cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, posicion);
  e->setTextCursor(cursor);
  update_status();
}

void Editor::update_status() {
  char cadena[200];
  QTextCursor cursor = e->textCursor();
  int l = cursor.blockNumber();
  int c = cursor.columnNumber();
  QByteArray nf = Nombre_fichero.toLocal8Bit();
  sprintf(cadena, " Fila:%3d  Columna:%3d | %s", l, c, nf.data());
  s->showMessage(cadena);
}
