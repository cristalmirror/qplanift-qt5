# Qplanif

Simulador gráfico de algoritmos de planificación de CPU y recursos.

Permite cargar un conjunto de tareas definidas en un archivo `.def`, elegir un algoritmo de planificación y visualizar el cronograma resultante en pantalla.

---

## Algoritmos disponibles

### CPU
- Descripción (sin planificación)
- FCFS — First Come First Served
- SJF — Shortest Job First
- SRTF — Shortest Remaining Time First
- Round Robin (quantum configurable)
- Prioridades (no expulsivo)
- Prioridades Expulsivas
- Prioridades + Round Robin

### Recursos
- FCFS
- SJF
- Prioridades

---

## Formato del archivo de tareas (`.def`)

```
RECURSO "disco"

TAREA "T1" PERIODO=10 OFFSET=0 PRIORIDAD=3 [0,5][1,2]
TAREA "T2" PERIODO=20 OFFSET=5 PRIORIDAD=1 [0,3]
```

Cada `[recurso, tiempo]` define una subtarea. El recurso `0` es siempre la CPU; los demás son los definidos con `RECURSO`.

---

## Compilación

### Dependencias

```bash
sudo apt-get install qtbase5-dev qt5-qmake flex bison
```

### Pasos

```bash
cd src

bison input.y -o input_tab.c
flex -o linput.c linput.l
cp input_tab.h input.tab.h

qmake qplanif.pro
make
```

### Ejecución

```bash
./qplanif
```

---

## Uso

1. Abrir un archivo `.def` desde **Archivos → Abrir**
2. Elegir el algoritmo de planificación en el desplegable de la toolbar
3. Elegir la política de gestión de recursos
4. Ajustar el quantum (solo activo en Round Robin y Prioridades+RR)
5. Usar **Ejecución paso a paso** para avanzar instante a instante
6. Ajustar zoom y grid para visualizar mejor el cronograma

---

## Créditos

- Versión original: Ismael Ripoll y Sergio Saez — DISCA, Universidad Politécnica de Valencia
- Adaptación a Qt3/Qt4: Julián Perelli
- Adaptación a Qt5: cristalmirror

---

## Licencia

GNU GPL v2 o posterior
