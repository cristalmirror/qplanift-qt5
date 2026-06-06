// *************************************************************************
//  Fichero: PlanifPFAging.cc
//  Descripcion: Priority Scheduling No Expulsivo con Aging
// *************************************************************************

#define PlanifPFAging_CC

#include "PlanifPFAging.hh"
#include "ProcesadorCls.hh"

int PlanifPFAging::TareaNueva(tarea_activa_t *tarea)
{
  tarea_aging_t ta;
  ta.tarea = tarea;
  ta.prioridad_efectiva = tarea->prioridad;
  preparadas.push_back(ta);
  return OK;
}

// Incrementa la prioridad efectiva segun el tiempo de espera acumulado.
// prioridad_efectiva = prioridad_original + floor(espera / AGING_INTERVAL)
// Menor numero = mayor prioridad (convencion del proyecto).
// El aging baja el numero => sube la prioridad efectiva.
void PlanifPFAging::aplicarAging(tiempo_t tiempo_actual)
{
  for (auto &ta : preparadas) {
    tiempo_t espera = tiempo_actual - ta.tarea->llegada;
    int bonus = (int)(espera / AGING_INTERVAL);
    ta.prioridad_efectiva = ta.tarea->prioridad - bonus;
  }
}

// Devuelve el indice de la tarea con mejor prioridad efectiva.
// En caso de empate, la que llego antes; si llegan al mismo tiempo, menor ident.
int PlanifPFAging::indiceMejor() const
{
  int mejor = 0;
  for (int i = 1; i < (int)preparadas.size(); i++) {
    const tarea_aging_t &m = preparadas[mejor];
    const tarea_aging_t &c = preparadas[i];
    if (c.prioridad_efectiva < m.prioridad_efectiva ||
        (c.prioridad_efectiva == m.prioridad_efectiva &&
         c.tarea->llegada < m.tarea->llegada) ||
        (c.prioridad_efectiva == m.prioridad_efectiva &&
         c.tarea->llegada == m.tarea->llegada &&
         c.tarea->ident < m.tarea->ident))
      mejor = i;
  }
  return mejor;
}

tiempo_t PlanifPFAging::SiguienteActivacion(const ProcesadorCls &procesador,
                                             tiempo_t tiempo_actual)
{
  if (!preparadas.empty()) {
    if (procesador.EstaLibre()) {
      t_seleccion = tiempo_actual;
      return tiempo_actual;
    } else {
      return procesador.SiguienteLibre(tiempo_actual);
    }
  }
  return MAX_TIEMPO;
}

tarea_activa_t *PlanifPFAging::BorraSiguiente()
{
  if (preparadas.empty())
    return nullptr;

  aplicarAging(t_seleccion);
  int idx = indiceMejor();
  tarea_activa_t *t = preparadas[idx].tarea;
  preparadas.erase(preparadas.begin() + idx);
  return t;
}

PlanifPFAging::~PlanifPFAging() {}
