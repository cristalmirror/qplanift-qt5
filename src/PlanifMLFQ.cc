// *************************************************************************
//  Fichero: PlanifMLFQ.cc
//  Descripcion: Multi-Level Feedback Queue
// *************************************************************************

#define PlanifMLFQ_CC

#include "PlanifMLFQ.hh"
#include "ProcesadorCls.hh"

int PlanifMLFQ::Inicializa(tiempo_t quantum)
{
  CPU_quantum         = quantum;
  quantum_por_cola[0] = quantum;
  quantum_por_cola[1] = quantum * 2;
  quantum_por_cola[2] = MAX_TIEMPO; // FCFS
  return OK;
}

bool PlanifMLFQ::hayTareas() const
{
  for (int i = 0; i < MLFQ_NUM_COLAS; i++)
    if (!colas[i].empty()) return true;
  return false;
}

int PlanifMLFQ::TareaNueva(tarea_activa_t *tarea)
{
  colas[0].push(tarea);
  nivel_tarea[tarea] = 0;
  return OK;
}

int PlanifMLFQ::TareaExpulsada(tarea_activa_t *tarea)
{
  // La tarea agoto su quantum: baja al siguiente nivel de cola
  int nivel_anterior = nivel_en_cpu;
  int nivel_nuevo = (nivel_anterior + 1 < MLFQ_NUM_COLAS)
                  ? nivel_anterior + 1
                  : MLFQ_NUM_COLAS - 1;

  colas[nivel_nuevo].push(tarea);
  nivel_tarea[tarea] = nivel_nuevo;
  return OK;
}

tiempo_t PlanifMLFQ::SiguienteActivacion(const ProcesadorCls &procesador,
                                          tiempo_t tiempo_actual)
{
  if (!hayTareas())
    return MAX_TIEMPO;

  if (procesador.EstaLibre()) {
    t_seleccion = tiempo_actual;
    return tiempo_actual;
  }

  // No expulsivo: no interrumpimos por llegada de nueva tarea.
  // Solo comprobamos si la tarea en CPU llego a su limite de quantum.
  tiempo_t ej = procesador.EjecutandoseTarea(tiempo_actual);
  tiempo_t q  = quantum_por_cola[nivel_en_cpu];

  if (q < MAX_TIEMPO && ej % q == 0 && ej > 0)
    return tiempo_actual; // limite exacto de quantum
  else if (q < MAX_TIEMPO)
    return tiempo_actual + q - (ej % q); // proximo limite
  else
    return procesador.SiguienteLibre(tiempo_actual); // FCFS: espera fin natural
}

tarea_activa_t *PlanifMLFQ::BorraSiguiente()
{
  for (int i = 0; i < MLFQ_NUM_COLAS; i++) {
    if (!colas[i].empty()) {
      tarea_activa_t *t = colas[i].front();
      colas[i].pop();
      nivel_en_cpu = i;
      nivel_tarea.erase(t);
      return t;
    }
  }
  return nullptr;
}

PlanifMLFQ::~PlanifMLFQ() {}
