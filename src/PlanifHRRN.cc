// *************************************************************************
//  Fichero: PlanifHRRN.cc
//  Descripcion: Highest Response Ratio Next (no expulsivo)
// *************************************************************************

#define PlanifHRRN_CC

#include "PlanifHRRN.hh"
#include "ProcesadorCls.hh"

int PlanifHRRN::TareaNueva(tarea_activa_t *tarea)
{
  preparadas.push_back(tarea);
  return OK;
}

// RR = (espera + computo) / computo,  espera = t_actual - t_llegada
int PlanifHRRN::indiceMejor() const
{
  int mejor = 0;
  double mejor_ratio = -1.0;

  for (int i = 0; i < (int)preparadas.size(); i++) {
    tarea_activa_t *t = preparadas[i];
    if (t->computo <= 0) continue;
    double espera = (double)(t_seleccion - t->llegada);
    double ratio  = (espera + t->computo) / (double)t->computo;
    if (ratio > mejor_ratio) {
      mejor_ratio = ratio;
      mejor = i;
    }
  }
  return mejor;
}

tiempo_t PlanifHRRN::SiguienteActivacion(const ProcesadorCls &procesador,
                                          tiempo_t tiempo_actual)
{
  if (!preparadas.empty()) {
    if (procesador.EstaLibre()) {
      t_seleccion = tiempo_actual; // guardamos para BorraSiguiente
      return tiempo_actual;
    } else {
      return procesador.SiguienteLibre(tiempo_actual);
    }
  }
  return MAX_TIEMPO;
}

tarea_activa_t *PlanifHRRN::BorraSiguiente()
{
  if (preparadas.empty())
    return nullptr;

  int idx = indiceMejor();
  tarea_activa_t *t = preparadas[idx];
  preparadas.erase(preparadas.begin() + idx);
  return t;
}

PlanifHRRN::~PlanifHRRN() {}
