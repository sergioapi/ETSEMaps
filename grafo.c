#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/////////////////////////////////////////////////////////// TIPOS DE DATOS

// Estructura privada
struct tipografo {
    int N; //número de vértices del grafo
    tipovertice VERTICES[MAXVERTICES]; //vector de vértices
    double A_Carretera[MAXVERTICES][MAXVERTICES]; //matriz de adyacencia
    double A_Autopista[MAXVERTICES][MAXVERTICES]; //matriz de adyacencia
};

//////////////////////////////////////////////////////////////// FUNCIONES

//HAY QUE MODIFICAR ESTA FUNCIÓN SI SE CAMBIA EL TIPO DE DATO tipovertice
/* 
 * Esta función devuelve 0 si los dos nodos son iguales
 * -1 si V1 está antes de V2 o 1 en otro caso.
 */
int _comparar_vertices(tipovertice V1, tipovertice V2) {
    return strcmp(V1.dato, V2.dato) == 0 ? 0 : strcmp(V1.dato, V2.dato) > 1 ? 1 : -1;
}

//Creación del grafo con 0 nodos
void crear_grafo(grafo *G) {
    *G = (struct tipografo *) malloc(sizeof(struct tipografo));
    (*G)->N = 0;
}

//Devuelve la posición del vértice Vert en el vector VERTICES del grafo G
//Si devuelve -1 es porque no encontró el vértice
int posicion(grafo G, tipovertice Vert) {
    int contador = 0;
    //comparo V con todos los vertices almacenados en VERTICES 
    while (contador < G->N) {
        //if (G->VERTICES[contador]==V)  //encontré la posicion de V
        if (_comparar_vertices(G->VERTICES[contador], Vert) == 0) {
            return contador;
        }
        contador++;
    }
    return -1;
}

//Devuelve 1 si el grafo G existe y 0 en caso contrario
int existe(grafo G) {
    return (G != NULL);
}

//Devuelve 1 si el vértice Vert existe en el grafo G
int existe_vertice(grafo G, tipovertice V) {
    return (posicion(G, V) >= 0);
}

//Inserta un vértice en el grafo, devuelve -1 si no ha podido insertarlo por estar el grafo lleno
int insertar_vertice(grafo *G, tipovertice Vert) {
    int i;
    if ((*G)->N == MAXVERTICES) {
        // Se ha llegado al maximo numero de vertices
        return -1;
    }

    (*G)->N++;
    (*G)->VERTICES[((*G)->N) - 1] = Vert;
    for (i = 0; i < (*G)->N; i++) {
        (*G)->A_Carretera[i][((*G)->N) - 1] = 0;
        (*G)->A_Carretera[((*G)->N) - 1][i] = 0;
        (*G)->A_Autopista[i][((*G)->N) - 1] = 0;
        (*G)->A_Autopista[((*G)->N) - 1][i] = 0;
    }
    return (*G)->N - 1;
}

//Borra un vertice del grafo
void borrar_vertice(grafo *G, tipovertice *Vert) {
    int F, C, P, N = (*G)->N;
    P = posicion(*G, *Vert);
    if (P == -1) {
        return;
    }
    //if (P >= 0 && P < (*G)->N) {
    for (F = P; F < N - 1; F++) {
        (*G)->VERTICES[F] = (*G)->VERTICES[F + 1];
    }
    for (C = P; C < N - 1; C++) {
        for (F = 0; F < N; F++) {
            (*G)->A_Carretera[F][C] = (*G)->A_Carretera[F][C + 1];
            (*G)->A_Autopista[F][C] = (*G)->A_Autopista[F][C + 1];
        }
    }
    for (F = P; F < N - 1; F++) {
        for (C = 0; C < N; C++) {
            (*G)->A_Carretera[F][C] = (*G)->A_Carretera[F + 1][C];
            (*G)->A_Autopista[F][C] = (*G)->A_Autopista[F + 1][C];
        }
    }
    (*G)->N--;
}

//Crea el arco de relación entre VERTICES(pos1) y VERTICES(pos2) con valor distancia en la matriz de Carretera
void insertarArcoCarretera(grafo *G, int pos1, int pos2, double distancia) {
    (*G)->A_Carretera[pos1][pos2] = distancia;
    (*G)->A_Carretera[pos2][pos1] = distancia;
}

//Crea el arco de relación entre VERTICES(pos1) y VERTICES(pos2) con valor distancia en la matriz de Autopista
void insertarArcoAutopista(grafo *G, int pos1, int pos2, double distancia) {
    (*G)->A_Autopista[pos1][pos2] = distancia;
    (*G)->A_Autopista[pos2][pos1] = distancia;
}

//Borra el arco de relación entre VERTICES(pos1) y VERTICES(pos2) en la matriz de Carretera
void borrarArcoCarretera(grafo *G, int pos1, int pos2) {
    (*G)->A_Carretera[pos1][pos2] = 0;
    (*G)->A_Carretera[pos2][pos1] = 0;
}

//Borra el arco de relación entre VERTICES(pos1) y VERTICES(pos2) en la matriz de Autopista
void borrarArcoAutopista(grafo *G, int pos1, int pos2) {
    (*G)->A_Autopista[pos1][pos2] = 0;
    (*G)->A_Autopista[pos2][pos1] = 0;
}

//Devuelve la distancia entre VERTICES(pos1) y VERTICES(pos2) por Carretera si son adyacentes
double distanciaCarretera(grafo G, int pos1, int pos2) {
    return (G->A_Carretera[pos1][pos2]);
}

//Devuelve la distancia entre VERTICES(pos1) y VERTICES(pos2) por Autopista si son adyacentes
double distanciaAutopista(grafo G, int pos1, int pos2) {
    return (G->A_Autopista[pos1][pos2]);
}

//Destruye el grafo
void borrar_grafo(grafo *G) {
    free(*G);
    *G = NULL;
}

//Devuelve el número de vértices del grafo G
int num_vertices(grafo G) {
    return G->N;
}

//Devuelve el vector de vértices VERTICES del grafo G
tipovertice *array_vertices(grafo G) {
    return G->VERTICES;
}
