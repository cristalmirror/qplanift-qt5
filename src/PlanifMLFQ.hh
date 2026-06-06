// *************************************************************************
//  Fichero: PlanifMLFQ.hh
//  Descripcion: Multi-Level Feedback Queue
//    3 colas de prioridad decreciente:
//      Cola 0 (alta)  : quantum = CPU_quantum       (Round Robin)
//      Cola 1 (media) : quantum = 2 * CPU_quantum   (Round Robin)
//      Cola 2 (baja)  : FCFS (quantum infinito)
//    Las tareas nuevas entran en la cola 0.
//    Si agotan su quantum sin terminar, bajan una cola.
//    No hay expulsion por llegada de tarea nueva (no expulsivo entre colas).
// *************************************************************************

#ifndef PlanifMLFQ_HH
#define PlanifMLFQ_HH

#include <queue>
#include <map>
#include "tipos.h"
#include "PlanificadorCls.hh"

#define MLFQ_NUM_COLAS 3

class PlanifMLFQ : public PlanificadorCls
{
public:
  PlanifMLFQ() : t_seleccion(0), nivel_en_cpu(0) {
    for (int i = 0; i < MLFQ_NUM_COLAS; i++)
      quantum_por_cola[i] = 1;
  }

  virtual int Inicializa(tiempo_t quantum);
  virtual int Inicializa() { return Inicializa(1); }

  virtual int TareaNueva(tarea_activa_t *tarea);

  virtual tiempo_t SiguienteActivacion(const ProcesadorCls &procesador,
                                       tiempo_t tiempo_actual);

  virtual tarea_activa_t *BorraSiguiente();

  virtual int TareaExpulsada(tarea_activa_t *tarea);

  virtual ~PlanifMLFQ();

private:
  bool hayTareas() const;

  std::queue<tarea_activa_t *> colas[MLFQ_NUM_COLAS];
  std::map<tarea_activa_t *, int> nivel_tarea; // nivel de cola de cada tarea

  tiempo_t t_seleccion;
  int      nivel_en_cpu;              // nivel de la tarea que esta en CPU
  tiempo_t quantum_por_cola[MLFQ_NUM_COLAS];
};

#endif // PlanifMLFQ_HH
