# Nombre del Proyecto
T3 Java

# Descripción
Comparación entre dos programas en Java para buscar el número con la mayor cantidad de divisores entre 1 y n, donde uno de los programas utiliza Threads y el otro no

Para la parte 2 se usaron 2 hebras, donde cada una recibe un % del total de números a inspeccionar

Ya que el proceso de selección del número con más divisores tiene un costo de O(n²), observamos que la última parte resulta más costosa, por lo que decidí asignarle un 60% de los números a la hebra 1 y el restante 40% a la hebra 2. Los detalles paso a paso se encuentran dentro del código del programa

Preguntas:
1. ¿A partir de qué número N, se empieza a notar una diferencia entre ambas implementaciones?
R: Desde N = 5.000

2. ¿A qué se debe esto? Argumente
R: Con N pequeño, la creación de hebras resulta más costosa que el proceso de selección del número  con más divisores, por lo que la parte 1 es más rápida de forma imperceptible. En cambio, con números más grandes, el costo de creación de hebras se vuelve despreciable en comparación con el proceso de selección

# Uso
1. Descomprimir la carpeta con los archivos y dentro de esta abrir el terminal
2. Para la parte 1, compilar con `make parte1` y ejecutarlo con `java SinThreads`
3. Para la parte 2, compilar con `make parte2` y ejecutarlo con `java ConThreads`

