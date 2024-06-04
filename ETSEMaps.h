#ifndef ETSEMAPS_H
#define ETSEMAPS_H

#include "grafo.h"

//FUNCIONES DEL PROGRAMA DE PRUEBA DE GRAFOS

//Inicializa el grafo añadiendo los vertices y arcos mediante la lectura de un archivo
void inicializar_desde_fichero(grafo *G, char *archivo);

//Escribe el grafo resultante de la ejecución del programa en un archivo
void escribir_grafo_en_fichero(grafo G, char *archivo);

//Opción a del menú de modificacion, introducir un vertice en el grafo
void introducir_vertice(grafo *G);

//Opción b del menú de modificacion, eliminar un vértice del grafo
void eliminar_vertice(grafo *G);

//Opción c del menú de modificacion, crear una relación entre dos vértices  por carretera
void nuevo_arco_carretera(grafo *G);

//Opción d del menú de modificacion, crear una relación entre dos vértices por autopista
void nuevo_arco_autopista(grafo *G);

//Opción e del menú de modificacion, eliminar una relación entre dos vértices por carretera
void eliminar_arco_carretera(grafo *G);

//Opción f del menú de modificacion, eliminar una relación entre dos vértices por autopista
void eliminar_arco_autopista(grafo *G);

//Opción i del menú de impresion, imprimir el grafo
void imprimir_grafo(grafo G);

/*Opciones a, b, c del menú de impresion
 * Si flags == 0 imprime la ruta mas corta (menor distancai en km) entre dos ciudades
 * Si flags == 1 imprime la ruta mas rapida (menor tiempo) entre dos ciudades
 * Si flags == 2 imprime la ruta mas economica (menor costo) entre dos ciudades
 * */
void imprimir_ruta(grafo G, int flags);

//Opción d del menú de impresion, imprime el arbol minimo de expansion segun el algoritmo de Prim utilizando como criterio minimo el tiempo
void imprimir_ruta_minima(grafo G);

#endif    /* ETSEMAPS_H */

