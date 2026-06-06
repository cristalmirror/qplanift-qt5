// *************************************************************************
//  Fichero: PlanifPFAging.hh
//  Descripcion: Priority Scheduling No Expulsivo con Aging
//    Igual que PFNE pero cada AGING_INTERVAL unidades de tiempo la
//    prioridad efectiva de los procesos en espera aumenta en 1.
//    Evita la inanicion de procesos de baja prioridad.
// *************************************************************************

#ifndef PlanifPFAging_HH
#define PlanifPFAging_HH

#include <vector>
#include "tipos.h"
#include "PlanificadorCls.hh"

// Cada cuantas unidades de tiempo de espera se incrementa la prioridad
#define AGING_INTERVAL 5

struct tarea_aging_t {
  tarea_activa_t *tarea;
  int             prioridad_efectiva; // prioridad ajustada por aging
};

class PlanifPFAging : public PlanificadorCls
{
public:
  PlanifPFAging() : t_seleccion(0) {}

  virtual int TareaNueva(tarea_activa_t *tarea);

  virtual tiempo_t SiguienteActivacion(const ProcesadorCls &procesador,
                                       tiempo_t tiempo_actual);

  virtual tarea_activa_t *BorraSiguiente();

    virtual int TareaExpulsada(tarea_activa_t * /*tarea*/)
  {
    return OK; // no expulsivo
  }

  virtual ~PlanifPFAging();

private:
  int indiceMejor() const;
  void aplicarAging(tiempo_t tiempo_actual);

  std::vector<tarea_aging_t> preparadas;
  tiempo_t t_seleccion;
};

#endif // PlanifPFAging_HH
