#ifndef AYUDA_HH
#define AYUDA_HH

#include <QWidget>
#include <QMenuBar>
#include <QTextEdit>
#include <QMenu>

#include <map>
#include <string.h>

struct menor_cadena {
  bool operator()(const char* t1, const char* t2) const {
    return strcmp(t1,t2) < 0;
  }
};

typedef std::map<const char*,const char*,menor_cadena> ayudas_t;

class AyudaCls : public QWidget
{
    Q_OBJECT
public:
    AyudaCls( QWidget *parent=0, const char *name=0 );
   ~AyudaCls();

public slots:
    void newDoc();
    void load( const char *text );
    void closeDoc();
    void ayuda_elegida(QAction*);

protected:
    void resizeEvent( QResizeEvent * );
    void closeEvent( QCloseEvent * );

private:
  QMenuBar   *m;
  QTextEdit  *e;
  QMenu      *temas;
  ayudas_t    Ayudas;
};

#endif
