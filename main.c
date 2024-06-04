#include <stdio.h>
#include <stdlib.h>
#include "ETSEMaps.h"

/*
 * Programa que muestra el uso del TAD grafo de números enteros
 */

/* Funcion que muestra el menu de modificacion del grafo (añadir/eliminar vertices y arcos) */
void menuModificacion(grafo *G);

/* Funcion que muestra el menu de impresion del grafo, permite imprimir todo el grafo con sus arcos o las diferentes
 * rutas entre dos vertices, a parte de la minima estructura de conexiones entre todos los vertices */
void menuImpresion(grafo G);

int main() {
    //Grafo de números enteros
    grafo G; //grafo
    char opcion;

    //Creo el grafo y lo inicializo con los vertices y arcos escritos en el fichero grafo_mapa.txt
    crear_grafo(&G);
    inicializar_desde_fichero(&G, "grafo_mapa.txt");
    do {
        printf("\n\na. Menu de modificacion del grafo\n");
        printf("b. Menu de impresion del grafo\n");
        printf("s. Salir\n");

        printf("Opcion: ");
        scanf(" %c", &opcion);

        switch (opcion) {
            case 'a':
            case 'A':
                menuModificacion(&G);
                break;
            case 'b':
            case 'B':
                menuImpresion(G);
                break;
            case 's':
            case 'S':
                opcion = 's';
                break;
            default:
                printf("Opción equivocada\n");
        }
    } while (opcion != 's');
    escribir_grafo_en_fichero(G, "grafo_mapa.txt");
    //Al salir, liberamos la memoria del TAD, lo destruimos
    borrar_grafo(&G);

    return (EXIT_SUCCESS);
}

void menuModificacion(grafo *G) {
    char opcion;
    do {
        printf("\n\na. Insertar nuevo vertice\n");
        printf("b. Eliminar vertice\n");
        printf("c. Crear arco por Carretera\n");
        printf("d. Crear arco por Autopista\n");
        printf("e. Eliminar arco por Carretera\n");
        printf("f. Eliminar arco por Autopista\n");
        printf("s. Salir\n");

        printf("Opcion: ");
        scanf(" %c", &opcion);
        switch (opcion) {
            case 'a':
            case 'A':
                introducir_vertice(G);
                break;
            case 'b':
            case 'B':
                eliminar_vertice(G);
                break;
            case 'c':
            case 'C':
                nuevo_arco_carretera(G);
                break;
            case 'd':
            case 'D':
                nuevo_arco_autopista(G);
                break;
            case 'e':
            case 'E':
                eliminar_arco_carretera(G);
                break;
            case 'f':
            case 'F':
                eliminar_arco_autopista(G);
                break;
            case 's':
            case 'S':
                opcion = 's';
                break;
            default:
                printf("Opción equivocada\n");
        }

    } while (opcion != 's');
}

void menuImpresion(grafo G) {
    char opcion;
    do {
        printf("\n\na. Imprimir la ruta más corta\n");
        printf("b. Imprimir la ruta más rápida\n");
        printf("c. Imprimir la ruta más económica\n");
        printf("d. Imprimir a mínima infraestrutura de conexións que fai que as cidades estean conectadas\n");
        printf("i. Imprimir grafo\n");
        printf("s. Salir\n");

        printf("Opcion: ");
        scanf(" %c", &opcion);
        switch (opcion) {
            case 'a':
            case 'A':
                imprimir_ruta(G, 0);
                break;
            case 'b':
            case 'B':
                imprimir_ruta(G, 1);
                break;
            case 'c':
            case 'C':
                imprimir_ruta(G, 2);
                break;
            case 'd':
            case 'D':
                imprimir_ruta_minima(G);
                break;
            case 'i':
            case 'I':
                imprimir_grafo(G);
                break;
            case 's':
            case 'S':
                opcion = 's';
                break;
            default:
                printf("Opción equivocada\n");
        }
    } while (opcion != 's');
}

