#ifndef QWERTY_H
#define QWERTY_H

#include <QWidget>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QString>
#include <QStatusBar>

class Papel_t: public QPlainTextEdit {
  Q_OBJECT
public:
  Papel_t(QWidget *parent=0, const char *name=0)
    : QPlainTextEdit(parent) { Q_UNUSED(name); }
  ~Papel_t() {}

signals:
  void cursor_Changed();

protected:
  void keyPressEvent(QKeyEvent *e) {
    emit cursor_Changed();
    QPlainTextEdit::keyPressEvent(e);
  }
};

class Editor : public QWidget
{
    Q_OBJECT
public:
    Editor( QWidget *parent=0, const char *name=0 );
   ~Editor();

public slots:
    void newFile();
    void load();
    void load( const char *fileName );
    void save();
    void saveAs();
    void close();
    void RemarcaLinea(int);
    void update_status();

signals:
    void re_carga(const char *);

protected:
    void resizeEvent( QResizeEvent * );
    void closeEvent( QCloseEvent * );

private:
    QMenuBar   *m;
    Papel_t    *e;
    QStatusBar *s;
    QString     Nombre_fichero;
};

#endif // QWERTY_H
