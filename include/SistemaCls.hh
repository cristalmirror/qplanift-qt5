	// *************************************************************************
// 
//  Departamento de Inform�tica de Sistema y Computadores (DISCA)
//  Universidad Politecnica de Valencia.                         
// 
//  Autor: Sergio S�ez (ssaez@disca.upv.es)
// 
//  Fichero: SistemaCls.hh
//  
//  Fecha: 
// 
//  Descripci�n: 
// 	 Declaraci�n de la clase 'SistemaCls'
// 
// *************************************************************************

#ifndef SistemaCls_HH
#define SistemaCls_HH

// *** Includes ***********************************************

#include <stdio.h>
#include <cstdlib>
#include "CargaCls.hh"
#include "EventosCls.hh"
#include "ProcesadorCls.hh"
#include "PlanificadorCls.hh"

// *** Constantes *********************************************

//  #define DEBUG

// *** Clases auxiliares **************************************

typedef enum {Llegada, Activacion, RecursoLibre} evento_simul_t;

// **** CLASE 'SistemaCls' ****

template <class CPUPlanif_c, class RECPlanif_c>  
class SistemaCls
{
public:
  SistemaCls 
  (
   void
   );

  // Realiza la simulaci�n del sistema
  int			Simula 
  (
   tarea_c *		tbl_tareas,
   int			num_tareas,
   int			num_recursos,
   tiempo_t		tiempo_simulacion,
   tiempo_t		quantum,
   EventosCls&		lst_eventos
   );

private:
  // Estado de la clase

  PlanificadorCls *	planificador[MAX_RECURSOS];
  ProcesadorCls 	recurso[MAX_RECURSOS];
  CargaCls		carga;
};

			       
// *** Definici�n de los cuerpos

template <class CPUPlanif_c,class RECPlanif_c>  
SistemaCls<CPUPlanif_c,RECPlanif_c>::SistemaCls ()
{
  for (int i=0; i<MAX_RECURSOS; i++)
    {
      if (i == 0)
	planificador[i]= new CPUPlanif_c;
      else
	planificador[i]= new RECPlanif_c;
    } // endfor

} // end SistemaCls::SistemaCls
 
// *** SistemaCls::Simula

template <class CPUPlanif_c,class RECPlanif_c>  
int 
SistemaCls<CPUPlanif_c,RECPlanif_c>::Simula 
( 
 tarea_c *		tbl_tareas,
 int			num_tareas,
 int			num_recursos,
 tiempo_t		tiempo_simulacion,
 tiempo_t		quantum,
 EventosCls&		lst_eventos
 )
{
  // *** Objetos Locales. Inicializaci�n

  evento_simul_t
    evento;			// Tipo del siguiente evento
  tiempo_t	
    tiempo_actual,		// Instante actual de la simulaci�n
    sig_llegada,		// Instante de la pr�xima llegada
    sig_activacion,		// Instante de la pr�xima activaci�n
    sig_libre;			// Instante del pr�ximo instante 
				//  de procesador libre
  tiempo_t
    siguiente;			// tiempo auxiliar

  tiempo_t
    inicio[MAX_RECURSOS];	// Inicio de cada activacion

  int
    identif;			// Identificador de la tarea que termina
    
  int
    recurso_activacion= 0;	// Recurso sobre el que sucede el siguiente 
				//  evento
  int
    recurso_libre= 0;		// Siguiente recurso libre
  bool
    final_simulacion;		// Indica que la simulaci�n a terminado

  tarea_activa_t *
    tarea;			// Tarea a incorporar
  tarea_activa_t *
    tarea_expulsada;		// Tarea expulsada
  
  // *** Cuerpo principal del m�todo


  lst_eventos.erase(lst_eventos.begin(),lst_eventos.end());

  // --- Inicializaci�n de la simulaci�n

  carga.Inicializa(tbl_tareas, num_tareas); 
				// Inicializa la carga de la simulaci�n

  for (int i= 0; i<=num_recursos; i++)
    {
      planificador[i]->Inicializa(quantum);
      recurso[i].Inicializa();
    } // endif
				// Inicializa los recursos del sistema
				// El primer recurso es el procesador
  final_simulacion= false;
  tiempo_actual= 0;

  // Bucle principal de la simulaci�n

  while (!final_simulacion)
    {
      // --- Siguiente evento
      
				// Siguiente recurso libre
      siguiente= 0;
      sig_libre= MAX_TIEMPO;
      for (int i= 0; 
	   i<=num_recursos; 
	   i++)
	{
	  siguiente= recurso[i].SiguienteLibre(tiempo_actual);
	  if (siguiente < sig_libre)
	    {
	      sig_libre= siguiente;
	      recurso_libre= i;
	    } // endif
	} // endfor
				// Siguiente llegada de una tarea
      sig_llegada= carga.SiguienteLlegada ();

				// Siguiente activaci�n de una tarea
      siguiente= 0;
      sig_activacion= MAX_TIEMPO;
      for (int i= 0; 
	   i<=num_recursos; 
	   i++)
	{
	  siguiente= planificador[i]->SiguienteActivacion(recurso[i], 
							  tiempo_actual);
	  if (siguiente < sig_activacion)
	    {
	      sig_activacion= siguiente;
	      recurso_activacion= i;
	    } // endif
	} // endfor

      // --- Selecciona el siguinte evento

      if (sig_libre <= sig_llegada && 
	  sig_libre <= sig_activacion)
	{
	  evento= RecursoLibre;
	  tiempo_actual= sig_libre;
	}
      else if (sig_llegada <= sig_activacion)
	{
	  evento= Llegada;
	  tiempo_actual= sig_llegada;
	}
      else if (sig_activacion < MAX_TIEMPO)
	{
	  evento= Activacion;
	  tiempo_actual= sig_activacion;
	}
      else			// Final de la simulaci�n
	{			// No hay m�s eventos
	  final_simulacion= true;
	  continue;
	} // endif

      // --- Control del tiempo

      if (tiempo_actual > tiempo_simulacion)
	{			// Final de la simulaci�n
	  tiempo_actual= tiempo_simulacion;
	  final_simulacion= true;	
	  continue;
	} // endif

      // --- Informaci�n de depuraci�n

#ifdef DEBUG
      fprintf(stdout, "SIS>> t: %ld Ev: %s\n", 
	      tiempo_actual,
	      (evento == Llegada) ? "Llegada" : ((evento == Activacion) 
						 ? "Activacion" : "Final" ) 
	      );
#endif

      // --- Manejo del evento

      switch (evento)
	{
	case Llegada:		// Llegada de una nueva tarea
	  tarea= carga.BorraSiguiente();
	  planificador[tarea->recurso]->TareaNueva(tarea);
				// Comunica al planificador la llegada
				// de una nueva tarea
	  lst_eventos.Llegada(tarea->ident, 
			      tarea->recurso, 
			      tiempo_actual);
				// Genera un evento de llegada
	  break;

	case Activacion:	// Activaci�n de una tarea
	  tarea= planificador[recurso_activacion]->BorraSiguiente();
				// Coge la siguiente tarea a planificar 
	  tarea_expulsada= 
	    recurso[recurso_activacion].TareaNueva(tiempo_actual,
						   tarea);
				// Pasa la atrea al estado 'ejecuci�n'
				// en el recurso correspondiente
	  if (tarea_expulsada != NULL)
	    {
	      planificador[tarea_expulsada->recurso]->
		TareaExpulsada(tarea_expulsada);
				// Le indica al planificador que se ha
				// expulsado una tarea
	      lst_eventos.Activacion(tarea_expulsada->ident, 
				     tarea_expulsada->recurso,
				     inicio[tarea_expulsada->recurso],
				     tiempo_actual);
				// Genera el evento de final de activaci�n
	    } // endif

	  inicio[recurso_activacion]= tiempo_actual;

       	  break;

	case RecursoLibre:	// Liberaci�n de un recurso. Implica
				// normalmente la terminaci�n de una tarea
	  tarea= recurso[recurso_libre].BorraSiguiente(tiempo_actual);
				// Saca la tarea del procesador
	  identif= tarea->ident;
				
	  lst_eventos.Activacion(identif, 
				 recurso_libre,
				 inicio[recurso_libre],
				 tiempo_actual);
				// Genera el evento de final de activaci�n

	  if (carga.TareaTerminada(tarea, tiempo_actual))
	    {			// Informa al generador de carga de la
				// terminaci�n de una subtarea
	      lst_eventos.Final(identif, 
				recurso_libre,
				tiempo_actual);
				// Si era la �ltima subtarea, genera
				// el evento de final de tarea
	    } // endif
	  break;
	default:
	  fprintf(stderr, "ERROR de implementaci�n:\n"
		  "  Fich: %s Linea: %d\n", __FILE__, __LINE__);
	  std::exit (1);
	  break;
	} // endswitch

    } // endwhile

  // *** Elimina las extructuras de datos

  carga.Termina();

  return OK;

} // end SistemaCls::Simula
 
#endif // *** Include SistemaCls.hh
