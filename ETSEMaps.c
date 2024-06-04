#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "grafo.h"

//Velocidad media por autopista (120 km/h)
#define V_AUTOPISTA 120.0
//Velocidad media por carretera (70 km/h)
#define V_CARRETERA 70.0
//Coste medio del km por autopista (0.07€)
#define C_AUTOPISTA 0.07
//Coste medio del km por carretera (0.01€)
#define C_CARRETERA 0.01

#define MAX_LINEA 100

// Estructura de los elementos de la matriz P, con la posición de un vértice del grafo y el tipo de vía por el que se comunica
struct elemMatrizP {
    int ciudad;
    char via;
};

// Imprime la matriz
void _printMatrix(double matrix[][MAXVERTICES], int V);

//Inicializa las matrices D y P
void
__inicializar_matrices(grafo G, double D[][MAXVERTICES], struct elemMatrizP P[][MAXVERTICES], double valorCarretera,
                       double valorAutopista);

// Función recursiva que imprime un camino entre dos vértices
void __imprimir_camino(grafo G, struct elemMatrizP P[][MAXVERTICES], int origen, int destino);

//Inicializa el grafo añadiendo los vertices y arcos mediante la lectura de un archivo
void inicializar_desde_fichero(grafo *G, char *archivo) {
    FILE *fp;
    char linea[MAX_LINEA], *token;
    tipovertice v1, v2;
    double distancia;
    unsigned short carretera;
    fp = fopen(archivo, "r");
    if (fp == NULL) {
        printf("No se ha podido abrir el archivo con los datos\n");
        return;
    }
    while (fgets(linea, MAX_LINEA, fp) != NULL) {
//En token guardamos el string hasta el primer ; que será la ciudad origen del arco, si no existe el vertice lo creamos
        token = strtok(linea, ";");
        strcpy(v1.dato, token);
        if (!existe_vertice(*G, v1))
            insertar_vertice(G, v1);
//En token guardamos el string hasta el segundo ; que será la ciudad destino del arco, si no existe el vertice lo creamos
        token = strtok(NULL, ";");
        strcpy(v2.dato, token);
        if (!existe_vertice(*G, v2))
            insertar_vertice(G, v2);
//En token guardamos el string hasta el tercer ; que será el caracter que indique si es una carretera (C) o una autopista (A)
        token = strtok(NULL, ";");
        carretera = !strcmp(token, "C") ? 1 : 0;
//En token guardamos el string despues del tercer ; que será la distancia en km de los vertices
        token = strtok(NULL, ";");
        distancia = strtof(token, NULL);
//Insertamos el arco
        if (carretera)
            insertarArcoCarretera(G, posicion(*G, v1), posicion(*G, v2), distancia);
        else insertarArcoAutopista(G, posicion(*G, v1), posicion(*G, v2), distancia);
    }
    fclose(fp);
}

void escribir_grafo_en_fichero(grafo G, char *archivo) {
    FILE *fp;
    tipovertice *VECTOR;
    int N;
    double distancia;
    N = num_vertices(G);
    VECTOR = array_vertices(G);
    int i, j;

    fp = fopen(archivo, "w");
    if (fp == NULL) {
        printf("No se ha podido abrir el archivo de escritura\n");
        return;
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            distancia = distanciaAutopista(G, i, j);
            if (distancia > 0)
                fprintf(fp, "%s;%s;A;%.2f\n", VECTOR[i].dato, VECTOR[j].dato, distancia);
            distancia = distanciaCarretera(G, i, j);
            if (distancia > 0)
                fprintf(fp, "%s;%s;C;%.2f\n", VECTOR[i].dato, VECTOR[j].dato, distancia);
        }
    }

    fclose(fp);
}

//Opción a del menú de modificacion, introducir un vertice en el grafo
void introducir_vertice(grafo *G) {
    tipovertice v1;
    printf("Introduce vertice: ");
    scanf("%s", v1.dato);
    if (existe_vertice(*G, v1))
        printf("Ese vertice ya esta en el grafo\n");
    else
        insertar_vertice(G, v1);
}

//Opción b del menú de modificacion, eliminar un vértice del grafo
void eliminar_vertice(grafo *G) {
    tipovertice v1;
    printf("Introduce vertice: ");
    scanf("%s", v1.dato);
    if (existe_vertice(*G, v1))
        borrar_vertice(G, &v1);
    else
        printf("Ese vertice no existe en el grafo\n");
}

//Opción c del menú de modificacion, crear una relación entre dos vértices  por carretera
void nuevo_arco_carretera(grafo *G) {
    if (num_vertices(*G) == 0) {
        printf("No hay vertices en el grafo, introduzcalos antes de crear un arco\n");
        return;
    }
    tipovertice v1, v2;
    double distancia;
    //Insertamos una nueva relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Nueva relacion vertice1<-->vertice2\n");
    //Vértice origen del arco
    printf("Introduce vertice origen: ");
    scanf("%s", v1.dato);
    if (!existe_vertice(*G, v1)) {
        printf("El vertice %s no existe en el grafo\n", v1.dato);
        return;
    }
    //Vértice destino del arco
    printf("Introduce vertice destino: ");
    scanf("%s", v2.dato);
    if (!existe_vertice(*G, v2)) {
        printf("El vertice %s no existe en el grafo\n", v2.dato);
        return;
    } else if (!strcmp(v1.dato, v2.dato)) {
        printf("No se pueden crear lazos\n");
        return;
    }
    //Creación del arco
    if (!distanciaCarretera(*G, posicion(*G, v1), posicion(*G, v2))) {
        printf("Introduce la distancia por carretera entre los dos vertices:");
        scanf("%lf", &distancia);
        if (distancia > 0) {
            insertarArcoCarretera(G, posicion(*G, v1), posicion(*G, v2), distancia);
            printf("Se ha creado la conexion por carretera entre %s y %s de %.2lf km\n", v1.dato, v2.dato, distancia);
        } else printf("La distancia tiene que ser mayor que cero\n");
    } else printf("Ya existe conexión entre estos dos vertices por carretera\n");
}

//Opción d del menú de modificacion, crear una relación entre dos vértices por autopista
void nuevo_arco_autopista(grafo *G) {
    if (num_vertices(*G) == 0) {
        printf("No hay vertices en el grafo, introduzcalos antes de crear un arco\n");
        return;
    }
    tipovertice v1, v2;
    double distancia;
    //Insertamos una nueva relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Nueva relacion vertice1<-->vertice2\n");
    //Vértice origen del arco
    printf("Introduce vertice origen: ");
    scanf("%s", v1.dato);
    if (!existe_vertice(*G, v1)) {
        printf("El vertice %s no existe en el grafo\n", v1.dato);
        return;
    }
    //Vértice destino del arco
    printf("Introduce vertice destino: ");
    scanf("%s", v2.dato);
    if (!existe_vertice(*G, v2)) {
        printf("El vertice %s no existe en el grafo\n", v2.dato);
        return;
    } else if (!strcmp(v1.dato, v2.dato)) {
        printf("No se pueden crear lazos\n");
        return;
    }
    //Creación del arco
    if (!distanciaAutopista(*G, posicion(*G, v1), posicion(*G, v2))) {
        printf("Introduce la distancia por autopista entre los dos vertices:");
        scanf("%lf", &distancia);
        if (distancia > 0) {
            insertarArcoAutopista(G, posicion(*G, v1), posicion(*G, v2), distancia);
            printf("Se ha creado la conexion por autopista entre %s y %s de %.2lf km\n", v1.dato, v2.dato, distancia);
        } else printf("La distancia tiene que ser mayor que cero\n");
    } else printf("Ya existe conexión entre estos dos vertices por autopista\n");
}

//Opción e del menú de modificacion, eliminar una relación entre dos vértices por carretera
void eliminar_arco_carretera(grafo *G) {
    if (num_vertices(*G) == 0) {
        printf("No hay vertices en el grafo, introduzcalos antes de crear un arco\n");
        return;
    }
    /**Añadir comprobacion de si existen arcos*/
    tipovertice v1, v2;
    //Eliminamos una relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Eliminar relacion vertice1-->vertice2\n");
    //Vértice origen del arco
    printf("Introduce vertice origen: ");
    scanf("%s", v1.dato);
    if (!existe_vertice(*G, v1)) {
        printf("El vertice %s no existe en el grafo\n", v1.dato);
        return;
    }
    //Vértice destino del arco
    printf("Introduce vertice destino: ");
    scanf("%s", v2.dato);
    if (!existe_vertice(*G, v2)) {
        printf("El vertice %s no existe en el grafo\n", v2.dato);
        return;
    }
    //Eliminación del arco
    if (distanciaCarretera(*G, posicion(*G, v1), posicion(*G, v2)) > 0) {
        borrarArcoCarretera(G, posicion(*G, v1), posicion(*G, v2));
        printf("Se ha eliminado la conexion por carretera entre %s y %s.\n", v1.dato, v2.dato);
    } else printf("Los vértices %s y %s no estan conectados por ningun arco\n", v1.dato, v2.dato);
}

//Opción f del menú de modificacion, eliminar una relación entre dos vértices por autopista
void eliminar_arco_autopista(grafo *G) {
    if (num_vertices(*G) == 0) {
        printf("No hay vertices en el grafo, introduzcalos antes de crear un arco\n");
        return;
    }
    /**Añadir comprobacion de si existen vertices y/o arcos*/
    tipovertice v1, v2;
    //Eliminamos una relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Eliminar relacion vertice1-->vertice2\n");
    //Vértice origen del arco
    printf("Introduce vertice origen: ");
    scanf("%s", v1.dato);
    if (!existe_vertice(*G, v1)) {
        printf("El vertice %s no existe en el grafo\n", v1.dato);
        return;
    }
    //Vértice destino del arco
    printf("Introduce vertice destino: ");
    scanf("%s", v2.dato);
    if (!existe_vertice(*G, v2)) {
        printf("El vertice %s no existe en el grafo\n", v2.dato);
        return;
    }
    //Eliminación del arco
    if (distanciaAutopista(*G, posicion(*G, v1), posicion(*G, v2)) > 0) {
        borrarArcoAutopista(G, posicion(*G, v1), posicion(*G, v2));
        printf("Se ha eliminado la conexion por autopista entre %s y %s.\n", v1.dato, v2.dato);
    } else printf("Los vértices %s y %s no estan conectados por ningun arco\n", v1.dato, v2.dato);
}

/* Opción i del menú de impresion, función que imprime el grafo utilizando num_vertices para saber cuántos vértices
 * tiene y array_vertices para recuperar el vector de vértices y recorrerlo */
void imprimir_grafo(grafo G) {
    tipovertice *VECTOR; //Para almacenar el vector de vértices del grafo
    int N; //número de vértices del grafo
    double distancia;
    //Para recorrerla, simplemente vamos a recorrer la matriz de adyacencia
    N = num_vertices(G);
    VECTOR = array_vertices(G);

    int i, j;
    printf("El grafo actual es:\n");
    for (i = 0; i < N; i++) {
        //Imprimo el vértice
        printf("Vertice(%d): %s\n", i, VECTOR[i].dato);
        //Chequeo sus arcos
        for (j = 0; j < N; j++) {
            distancia = distanciaAutopista(G, i, j);
            if (distancia > 0)
                printf("\tPor autopista: %s<=>%s %.2f km\n", VECTOR[i].dato, VECTOR[j].dato, distancia);
            distancia = distanciaCarretera(G, i, j);
            if (distancia > 0)
                printf("\tPor carretera: %s<->%s %.2f km\n", VECTOR[i].dato, VECTOR[j].dato, distancia);
        }
    }
}

// Función recursiva que imprime un camino entre dos vértices
void __imprimir_camino(grafo G, struct elemMatrizP P[][MAXVERTICES], int origen, int destino) {
    tipovertice *VECTOR = array_vertices(G);

    if (origen != destino) {
        __imprimir_camino(G, P, origen, P[origen][destino].ciudad);
        // Segun el tipo de via imprimimos: --> si es por carretera o ==> si es por autopista
        if (P[origen][destino].via == 'C')
            printf(" --> ");
        else printf(" ==> ");
    }
    printf("%s", VECTOR[destino].dato);
}

/*Opciones a, b, c del menú de impresion
 * Si flags == 0 imprime la ruta mas corta (menor distancai en km) entre dos ciudades
 * Si flags == 1 imprime la ruta mas rapida (menor tiempo) entre dos ciudades
 * Si flags == 2 imprime la ruta mas economica (menor costo) entre dos ciudades
 * */
void imprimir_ruta(grafo G, int flags) {
    tipovertice v1, v2;
    double D[MAXVERTICES][MAXVERTICES];
    struct elemMatrizP P[MAXVERTICES][MAXVERTICES];

    printf("Introduce vertice origen: ");
    scanf("%s", v1.dato);
    if (!existe_vertice(G, v1)) {
        printf("El vertice %s no existe en el grafo\n", v1.dato);
        return;
    }
    printf("Introduce vertice destino: ");
    scanf("%s", v2.dato);
    if (!existe_vertice(G, v2)) {
        printf("El vertice %s no existe en el grafo\n", v2.dato);
        return;
    }

    switch (flags) {
        case 0:
            __inicializar_matrices(G, D, P, 1, 1);
            if (D[posicion(G, v1)][posicion(G, v2)] != INFINITY)
                printf("La distancia total desde %s hasta %s es de %.2lf km\n", v1.dato, v2.dato,
                       D[posicion(G, v1)][posicion(G, v2)]);
            else printf("Las ciudades no están conectadas\n");
            break;
        case 1:
            __inicializar_matrices(G, D, P, 1 / V_CARRETERA, 1 / V_AUTOPISTA);
            if (D[posicion(G, v1)][posicion(G, v2)] != INFINITY)
                printf("El tiempo total desde %s hasta %s es de %.2lf horas\n", v1.dato, v2.dato,
                       D[posicion(G, v1)][posicion(G, v2)]);
            else printf("Las ciudades no están conectadas\n");
            break;
        case 2:
            __inicializar_matrices(G, D, P, C_CARRETERA, C_AUTOPISTA);
            if (D[posicion(G, v1)][posicion(G, v2)] != INFINITY)
                printf("El coste economico total desde %s hasta %s es de %.2lf €\n", v1.dato, v2.dato,
                       D[posicion(G, v1)][posicion(G, v2)]);
            else printf("Las ciudades no están conectadas\n");
            break;
    }
    if (D[posicion(G, v1)][posicion(G, v2)] != INFINITY) {
        printf("\tRuta: ");
        __imprimir_camino(G, P, posicion(G, v1), posicion(G, v2));
        printf("\n");
    }
}

//Opción d del menú de impresion, imprime el arbol minimo de expansion segun el algoritmo de Prim utilizando como criterio minimo el tiempo
void imprimir_ruta_minima(grafo G) {
    tipovertice *VECTOR = array_vertices(G);
    double tiempoTotal = 0, minimo;
    int vx, vy, numArcos = 0, N = num_vertices(G), selected[N];
    char via = '-';
    for (int i = 0; i < N; ++i) {
        selected[i] = 0;
    }
    //iniciamos el algoritmo seleccionando el primer vértice
    selected[0] = 1;
    while (numArcos < (N - 1)) {
        minimo = INFINITY;
        vx = vy = 0;
        //Busco el arco x-y con valor mínimo
        for (int i = 0; i < N; ++i) {
            if (selected[i]) {
                for (int j = 0; j < N; ++j) {
                    if ((selected[j] != 1) && (distanciaCarretera(G, i, j) > 0 ||
                                               distanciaAutopista(G, i, j) > 0)) {
                        if (((distanciaCarretera(G, i, j) / V_CARRETERA) != 0) &&
                            ((distanciaAutopista(G, i, j) / V_AUTOPISTA) != 0)) {
                            if ((distanciaAutopista(G, i, j) / V_AUTOPISTA) >
                                (distanciaCarretera(G, i, j) / V_CARRETERA)) {
                                if (minimo > (distanciaCarretera(G, i, j) /
                                              V_CARRETERA)) {
                                    minimo = distanciaCarretera(G, i, j) / V_CARRETERA;
                                    via = '-';
                                    vx = i;
                                    vy = j;
                                }
                            } else if (minimo > (distanciaAutopista(G, i, j) / V_AUTOPISTA)) {
                                minimo = distanciaAutopista(G, i, j) / V_AUTOPISTA;
                                via = '=';
                                vx = i;
                                vy = j;
                            }
                        } else if (((distanciaAutopista(G, i, j) / V_AUTOPISTA) == 0) &&
                                   (minimo > (distanciaCarretera(G, i, j) /
                                              V_CARRETERA))) {
                            minimo = distanciaCarretera(G, i, j) / V_CARRETERA;
                            via = '-';
                            vx = i;
                            vy = j;
                        } else if (((distanciaCarretera(G, i, j) / V_CARRETERA) == 0) &&
                                   (minimo > (distanciaAutopista(G, i, j) / V_AUTOPISTA))) {
                            minimo = distanciaAutopista(G, i, j) / V_AUTOPISTA;
                            via = '=';
                            vx = i;
                            vy = j;
                        }
                    }
                }
            }
        }
        //vx-vy es el arco con valor mínimo que añade vy al conjunto Selected
        selected[vy] = 1;
        numArcos++;
        if (minimo != INFINITY) {
            printf("%10s %c%c %-10s :\t%.2lf h\n", VECTOR[vx].dato, via, via, VECTOR[vy].dato, minimo);
            tiempoTotal = tiempoTotal + minimo;
        }
    }
    printf("El tiempo del árbol de expansión de coste mínimo es de %.2lf horas\n", tiempoTotal);
}

//Inicializa las matrices D y P
void __inicializar_matrices(grafo G, double D[][MAXVERTICES], struct elemMatrizP P[][MAXVERTICES],
                            double valorCarretera, double valorAutopista) {

    for (int i = 0; i < MAXVERTICES; ++i)
        for (int j = 0; j < MAXVERTICES; ++j) {

            if (i != j && ((distanciaCarretera(G, i, j) * valorCarretera) > 0 ||
                           (distanciaAutopista(G, i, j) * valorAutopista) > 0)) {

                if ((distanciaCarretera(G, i, j) * valorCarretera) != 0 &&
                    (distanciaAutopista(G, i, j) * valorAutopista) == 0) {
                    D[i][j] = distanciaCarretera(G, i, j) * valorCarretera;
                    P[i][j].via = 'C';
                } else if ((distanciaCarretera(G, i, j) * valorCarretera) == 0 &&
                           (distanciaAutopista(G, i, j) * valorAutopista) != 0) {
                    D[i][j] = distanciaAutopista(G, i, j) * valorAutopista;
                    P[i][j].via = 'A';
                } else if ((distanciaAutopista(G, i, j) * valorAutopista) <
                           (distanciaCarretera(G, i, j) * valorCarretera)) {
                    D[i][j] = distanciaAutopista(G, i, j) * valorAutopista;
                    P[i][j].via = 'A';
                } else {
                    D[i][j] = distanciaCarretera(G, i, j) * valorCarretera;
                    P[i][j].via = 'C';
                }
                P[i][j].ciudad = i;
            } else {
                P[i][j].via = '\0';
                P[i][j].ciudad = 0;
                if (i == j)
                    D[i][j] = 0;
                else
                    D[i][j] = INFINITY;
            }
        }

    for (int k = 0; k < MAXVERTICES; ++k)   // Analizamos matriz Dk
        for (int i = 0; i < MAXVERTICES; ++i)   // Arco i->k
            for (int j = 0; j < MAXVERTICES; ++j) {     // Arco j->k
                if (D[i][j] > (D[i][k] + D[k][j])) {
                    D[i][j] = (D[i][k] + D[k][j]);
                    P[i][j] = P[k][j];
                }
            }
}

void _printMatrix(double matrix[][MAXVERTICES], int V) {
    int i, j;
    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            if (matrix[i][j] == INFINITY)
                printf("%10s", "INF");
            else
                printf("%10.2f", matrix[i][j]);
        }
        printf("\n");
    }
}