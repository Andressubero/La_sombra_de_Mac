#ifndef __LA_SOMBRA_DE_MAC__
#define __LA_SOMBRA_DE_MAC__

#include <stdbool.h>

#define MAX_FILAS 20
#define MAX_COLUMNAS 25
#define MAX_ELEMENTOS 500
#define MAX_NIVELES 3
#define MAX_PAREDES 200
#define MAX_NOMBRE 100


typedef struct partida {
    char jugador[MAX_NOMBRE];
    int nivel_llegado;
    int puntos;
    int vidas_restantes;
    bool gano;
} partida_t;

typedef struct coordenada {
    int fila;
    int col;
}coordenada_t;

typedef struct personaje {
    coordenada_t posicion;
    int vida;
    int puntos;
    bool tiene_llave;
    bool interruptor_apretado;
}personaje_t;

typedef struct sombra {
    coordenada_t posicion;
    bool esta_viva;
}sombra_t;

typedef struct elemento {
    char tipo;
    coordenada_t coordenada;
}elemento_t;

typedef struct nivel {
    int numero_nivel;
    coordenada_t paredes[MAX_PAREDES];
    int tope_paredes;
    elemento_t obstaculos[MAX_ELEMENTOS];
    int tope_obstaculos;
    elemento_t herramientas[MAX_ELEMENTOS];
    int tope_herramientas;
}nivel_t;

typedef struct juego {
    personaje_t personaje;
    sombra_t sombra;
    nivel_t niveles[MAX_NIVELES];
    int nivel_actual;
}juego_t;


/*
 * Procedimiento que recibe el juego e imprime toda su información por pantalla.
 */
void imprimir_terreno(juego_t juego);

/* 
 *Inicializará el juego, cargando toda la información inicial, los datos del personaje, y los 3 niveles.
 */
void inicializar_juego(juego_t* juego);

/*
 * Recibe un juego con todas sus estructuras válidas.
 *
 * El juego se dará por ganado, si terminó todos los niveles. O perdido, si el personaje queda
 * sin vida. 
 * Devolverá:
 * -> 0 si el estado es jugando. 
 * -> -1 si el estado es perdido.
 * -> 1 si el estado es ganado.
 */
int estado_juego(juego_t juego);

/*
 * El nivel se dará por terminado, si ambos personajes pasaron por la puerta teniendo la 
 * llave correspondiente.
 * Devolverá:
 * -> 0 si el estado es jugando.
 * -> 1 si el estado es ganado.
 */
int estado_nivel(juego_t juego);

/*
 * Moverá el personaje, y realizará la acción necesaria en caso de chocar con un elemento
 */
void realizar_jugada(juego_t* juego);

/*Funcion que recibe el nombre de un archivo.
El archivo debe estar previmente creado, ser de tipo csv y con al menos una partida guardada.
El archivo debe tener todos los campos completados correspondientes a las partidas.
Si hay varias partidas  ya guardadas en el archivo, deben estar ordenadas alfabeticamente por el nombre del jugador
Guarda los datos ingresados por el usuario por consola como una nueva partida*/

int agregar_partida (char nombre_archivo[MAX_NOMBRE]);

/*Funcion que recibe el nombre del jugador y el del archivo donde se encuentra
El  archivo  debe  estar previamente  creado y ser de tipo csv.
el archivo debe tener todos los campos completados correspondientes a las partidas.
El nombre del jugador debe cumplir con la convencion propuesta: si es mas de una palabra, deben estar unidas por guion bajo y la inicial en mayuscula.
Si el nombre del jugador existe en  el  archivo sera eliminado*/

int eliminar_partida (char nombre_jugador[MAX_NOMBRE], char nombre_archivo[MAX_NOMBRE]);

/*Funcion que recibe el nombre de un archivo de partidas.
El archivo debe ser de tipo csv y tener todos los campos completados correspondientes a las partidas.
Ordena las partidas alfabeticamente segun el  nombre del jugador*/

int ordenar_partidas(char nombre_archivo[MAX_NOMBRE]);

#endif