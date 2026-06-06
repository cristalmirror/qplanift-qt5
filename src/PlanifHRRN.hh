// *************************************************************************
//  Fichero: PlanifHRRN.hh
//  Descripcion: Highest Response Ratio Next (no expulsivo)
//    Selecciona la tarea con mayor ratio: (espera + computo) / computo
// *************************************************************************

#ifndef PlanifHRRN_HH
#define PlanifHRRN_HH

#include <vector>
#include "tipos.h"
#include "PlanificadorCls.hh"

class PlanifHRRN : public PlanificadorCls
{
public:
  PlanifHRRN() : t_seleccion(0) {}

  virtual int TareaNueva(tarea_activa_t *tarea);

  virtual tiempo_t SiguienteActivacion(const ProcesadorCls &procesador,
                                       tiempo_t tiempo_actual);

  virtual tarea_activa_t *BorraSiguiente();

  virtual int TareaExpulsada(tarea_activa_t */*tarea*/)
  {
    return OK; // no expulsivo
  }

  virtual ~PlanifHRRN();

private:
  int indiceMejor() const;

  std::vector<tarea_activa_t *> preparadas;
  tiempo_t t_seleccion; // tiempo guardado desde SiguienteActivacion
};

#endif // PlanifHRRN_HH
