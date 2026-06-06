// *************************************************************************
// 
//  Departamento de Informática de Sistema y Computadores (DISCA)
//  Universidad Politecnica de Valencia.                         
// 
//  Autor: Sergio Sáez (ssaez@disca.upv.es)
// 
//  Fichero: ProcesadorCls.hh
//  
//  Fecha: 
// 
//  Descripción: 
// 	 Declaración de la clase 'ProcesadorCls'
// 
// *************************************************************************

#ifndef ProcesadorCls_HH
#define ProcesadorCls_HH

// *** Includes ***********************************************

#include "tipos.h"
#include <cstddef>

// *** Constantes *********************************************


// *** Clases auxiliares **************************************


// **** CLASE 'ProcesadorCls' ****

class ProcesadorCls
{
public:
  ProcesadorCls 
  (
   void
   );

  // Inicialización

  void			Inicializa 
  (
   void 
   )
    {
      tarea_ejecucion= NULL;
      instante_activacion= 0;
    }; 

  // Coloca en ejecución la 'Tarea' y devuelve la tarea que estaba en
  // Ejecución, NULL si estaba vacio.
  tarea_activa_t *	TareaNueva 
  (
   tiempo_t		tiempo_actual,
   tarea_activa_t *	tarea
   );

  // Saca de ejecución la tarea. Se debe utilizar sólo cuando la tarea
  // en ejecución ha terminado
  tarea_activa_t *	BorraSiguiente 
  (
   tiempo_t		tiempo_actual
   );

  // *** Información ************************************************

  // Informa si el procesador está libre
  bool			EstaLibre
  (
   ) const
    {
      return (tarea_ejecucion == NULL);
    };

  // Informa del siguiente instante en que estará el procesador libre 
  tiempo_t		SiguienteLibre 
  (
   tiempo_t		tiempo_actual
   ) const;

  // Devuelve el tiempo que le queda a la tarea en ejecución por
  // ejecutarse
  // PRE: !(EstaLibre())
  tiempo_t		ComputoTarea
  (
   tiempo_t		tiempo_actual
   ) const 
    {
      return tarea_ejecucion->computo - (tiempo_actual - instante_activacion);
    };

  // Devuelve el tiempo que lleva la tarea en ejecución
  // PRE: !(EstaLibre())
  tiempo_t		EjecutandoseTarea
  (
   tiempo_t		tiempo_actual
   ) const 
    {
      return tiempo_actual - instante_activacion;
    };

  // Devuelve la prioridad de la tarea en ejecución
  // PRE: !(EstaLibre())
  int			PrioridadTarea
  (
   tiempo_t		/*tiempo_actual*/
   ) const 
    {
      return tarea_ejecucion->prioridad;
    };

private:
  tarea_activa_t *	tarea_ejecucion;
				// Tarea en ejecución
  tiempo_t		instante_activacion;
				// Instante en el que entro la tarea
				// que está en ejecución
};

#endif // *** Include ProcesadorCls.hh

