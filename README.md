# Qplanif

Simulador gráfico de algoritmos de planificación de CPU y recursos.

Permite cargar un conjunto de tareas definidas en un archivo `.def`, elegir un algoritmo de planificación y visualizar el cronograma resultante en pantalla. Se pueden abrir múltiples ventanas simultáneas para comparar algoritmos lado a lado.

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
# Esto es un comentario

RECURSO "Disco SCSI"
RECURSO "Teclado"

TAREA "cp"  INICIO=0  PERIODO=20  PRIORIDAD=1
    [CPU, 2]
    [1, 3]
    [CPU, 1]
    [2, 1]
    [CPU, 1]

TAREA "gcc"  PRIORIDAD=2
    [CPU, 1]
    [1, 3]
    [CPU, 10]
```

- `RECURSO "nombre"` — declara un recurso con nombre. Se numeran a partir de 1 en orden de declaración y **deben declararse antes de usarse**.
- `TAREA "nombre"` — declara una tarea. Los campos `INICIO`, `PERIODO` y `PRIORIDAD` son opcionales (por defecto: `0`, aperiódica y prioridad `0`).
- `[CPU, duración]` — subtarea de CPU. `CPU` es palabra reservada.
- `[N, duración]` — subtarea de recurso, donde `N` es el número de orden del recurso declarado (1, 2, …).

El archivo de especificación se puede dividir en varias líneas para mayor legibilidad. El parser ignora cualquier símbolo no reconocido, por lo que es muy tolerante ante errores de formato.

En el directorio `examples/` hay archivos `.def` de ejemplo listos para cargar.

---

## Interfaz gráfica

- **Rectángulos de colores** — tiempo que una tarea ocupa un recurso. La CPU se representa en azul claro.
- **Flecha hacia arriba (▲)** — petición de un recurso por parte de una tarea.
- **Flecha hacia abajo (▼)** — liberación del recurso / fin de la subtarea.
- **Tooltip** — al situar el ratón sobre el nombre de una tarea aparece un globo con su información.
- **Múltiples ventanas** — `Ctrl+N` o **Archivos → Nueva ventana** abre otra instancia mostrando el mismo conjunto de tareas con un algoritmo distinto, útil para comparar.

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

O bien pasando un archivo directamente:

```bash
./qplanif ruta/al/archivo.def
```

---

## Uso

1. Abrir un archivo `.def` desde **Archivos → Abrir** (o arrastrando el archivo)
2. Elegir el algoritmo de planificación en el desplegable de la toolbar
3. Elegir la política de gestión de recursos
4. Ajustar el quantum (solo activo en Round Robin y Prioridades+RR)
5. Activar **Ejecución paso a paso** e incrementar el instante para avanzar instante a instante, o desactivarlo para ver la simulación completa
6. Ajustar zoom y grid para visualizar mejor el cronograma

---

## Créditos

- Versión original: Ismael Ripoll y Sergio Saez — DISCA, Universidad Politécnica de Valencia
- Adaptación a Qt3/Qt4: Julián Perelli
- Adaptación a Qt5: cristalmirror

---

## Licencia

GNU GPL v2 o posterior
