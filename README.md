# DisenoAlgoritmosII

Proyecto de la cadena Diseño de Algoritmos II (CI-5652) de la Universidad Simón Bolívar

## Datasets

Se cuenta con 5 conjuntos de datos:

- Iris. Disponible en: /APC/datasets/iris
- Sonar. Disponible en: /APC/datasets/Sonar
- WDBC. Disponible en: /APC/datasets/wdbc
- Spambase. Disponible en: /APC/datasets/spambase
- Waveform. Disponible en: /APC/datasets/waveform

Dentro de estos directorios se encuentran los datos en un archivo en
<nombre_dataset>.data y su descripción en <nombre_dataset>.names.

## Main

Para compilar el main del proyecto se utiliza el makefile que se encuentra en APC/.
El main recibe como argumento un archivo de configuración con la lista de los
algoritmos que se desean ejecutar.

> Nota: asegúrese de colocar los datos correctamente ya que el Main no tiene manejo de errores.

### Configuración

Para ejecutar las pruebas se debe contar con un archivo que presente la siguiente estructura:

```
<cantidad_de_particiones>
<nombre_dataset> <nombre_algoritmo> <parámetro1>..<parámetroN>
```
Por ejemplo:
```
2
iris LS_random 2 4
```

Dentro del directorio se encuentra con archivo llamado "config" que tiene la
estructura descrita. Adicionalmente, el archivo "best_config" contiene los
parámetros que se consideraron los mejores.

En detalle, lo parámetros (en orden) para cada algoritmo son:

- no_weights : no aplica.
- relief : no aplica.
- LS_random : máximo de iteraciones, vecinos generados.
- LS_relief : máximo de iteraciones, vecinos generados.
- ILS_random : máximo de iteraciones, vecinos generados, máximo de iteraciones sin cambio.
- ILS relief : máximo de iteraciones, vecinos generados, máximo de iteraciones sin cambio.
- SA_random : máximo de iteraciones, vecinos generados, temperatura inicial, iteraciones para
estabilizar.
- SA_relief : máximo de iteraciones, vecinos generados, temperatura inicial, iteraciones para
estabilizar.
- Scatter_random : máximo de iteraciones, tamaño de la población, máximo de iteraciones sin cambio.
- Scatter relief : máximo de iteraciones, tamaño de la población, máximo de iteraciones sin cambio.
- DE : máximo de iteraciones, tamaño de la población, CR, F.

## Estadísticas

### Experimentos

En el directorio /Paper/statistics/<nombre_dataset> se encuentran los resultados
obtenidos con cada ejecución de cada metaheurística con los mejores parámetros
("best_config"). Además se incluyen los experimentos realizados para obtener
los mejores parámetros en el directorio /Paper/statistics/statistics/<nombre_dataset>,
en él, se puede observer un archivo llamado "experiments.csv" con todos los experimentos
realizados.

### Gráficos

Los boxplots se encuentran dentro del directorio /Paper/graphics/<nombre_dataset>

## Paper

Se encuentra en el archivo "[Informe.pdf](https://github.com/fefi95/DisenoAlgoritmosII/blob/master/Paper/Informe.pdf)" con su correspondiente archivo escrito en LAteX
"Informe.tex". Para compilar el archivo, asegúrese de tener TeXLive 2016 o una versión
superior.
