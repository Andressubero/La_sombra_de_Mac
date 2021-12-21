#include "la_sombra_de_mac.h"
#include <stdio.h> 
#include <stdlib.h> 
#include "utiles.h"
#include <string.h> 
#define ARRIBA 'W'
#define ABAJO 'S'
#define DERECHA 'D'
#define IZQUIERDA 'A'
#define PERSONAJE 'M'
#define SOMBRA 'B'
#define PUNTO '.'
#define INGRESE_MOVIMIENTO "Ingrese un movimiento: W = arriba, S= abajo, A = izquierda y D = derecha\n"
#define FORMATO_DATOS "Vidas: %i\nPuntos: %i\nLlave: %s\nNivel: %i\n"
#define INGRESAR_NOMBRE "Ingrese el nombre del jugador, la inicial en mayuscula y si es mas de  una palabra, unidas por guion bajo"
#define INGRESAR_NIVEL "Ingrese el nivel llegado"
#define INGRESAR_VIDAS "Ingrese las vidas restantes"
#define INGRESAR_PUNTOS "Ingrese los puntos obtenidos"
#define INGRESAR_RESULTADO "Gano?"
#define FORMATO_IMPRESION "%s;%i;%i;%i;%s\n"
#define FORMATO_LECTURA "%[^;];%i;%i;%i;%[^\n]\n"
#define ERROR "no se pudo abrir\n"
#define SI "Si"
#define NO "No"
#define AUXILIAR "archivo_aux.csv"
#define N3_POZOS_S "N3_POZOS"
#define N2_POZOS_S "N2_POZOS"
#define N1_POZOS_S "N1_POZOS"
#define N1_VELAS_S "N1_VELAS"
#define N2_VELAS_S "N2_VELAS"
#define N3_VELAS_S "N3_VELAS"
#define N1_INTERRUPTORES_S "N1_INTERRUPTORES"
#define N2_INTERRUPTORES_S "N2_INTERRUPTORES"
#define N3_INTERRUPTORES_S "N3_INTERRUPTORES"
#define N1_PORTALES_S "N1_PORTALES"
#define N2_PORTALES_S "N2_PORTALES"
#define N3_PORTALES_S "N3_PORTALES"
#define N1_MONEDAS_S "N1_MONEDAS"
#define N2_MONEDAS_S "N2_MONEDAS"
#define N3_MONEDAS_S "N3_MONEDAS"
#define N1_ESCALERAS_S  "N1_ESCALERAS"
#define N2_ESCALERAS_S  "N2_ESCALERAS"
#define N3_ESCALERAS_S "N3_ESCALERAS"
#define N1_LLAVES_S "N1_LLAVES"
#define N2_LLAVES_S "N2_LLAVES"
#define N3_LLAVES_S "N3_LLAVES"
#define CONFIGURACIONES "config.txt"
#define LEER "r"
#define ESCRIBIR "w"
const bool SENTIDO = true;
const char ESCALERA = 'E';
const char PUERTA = 'D';
const char LLAVE = 'L';
const char MONEDA = 'C';
const char POZO = 'W';
const char INTERRUPTOR = 'O';
const char PORTAL = 'P';
const char VELA = 'V';
const char VIDA = 'V';
const char PARED = '=';
const int PUNTOS_VIDA = 200;
const int PUNTOS_REVIVIR_SOMBRA = 50;
const int LLAVE_N1= 0;
const int LLAVE_N2= 1;
const int LLAVE_N3= 1;
const int N_PUERTAS= 1;
const int ESCALERAS_N1= 10;
const int ESCALERAS_N2= 15;
const int ESCALERAS_N3= 15;
const int MONEDAS_N1= 10;
const int MONEDAS_N2= 15;
const int MONEDAS_N3= 15;
const int POZOS_N1= 15;
const int POZOS_N2= 20;
const int POZOS_N3= 30;
const int INTERRUPTORES_N1= 1;
const int INTERRUPTORES_N2= 2;
const int INTERRUPTORES_N3= 4;
const int PORTALES_N1= 0;
const int PORTALES_N2= 2;
const int PORTALES_N3= 4;
const int VELAS_N1= 5;
const int VELAS_N2= 10;
const int VELAS_N3= 12;
const int VIDAS_INICIALES = 3;
const bool INICIO_FALSE = false;
const bool INICIO_TRUE = true;

/*
*    Función que recibe dos coordenadas, devuelve true si las
* coordenadas son iguales, sino false.
*/

bool coordenadas_iguales(coordenada_t a, coordenada_t b){
		return a.fila == b.fila && a.col == b.col;
}


/*
*   Procedimiento que dejará la estructura personaje_t en un
* estado válido para iniciar el juego.
* 'arranque_personaje' es un parámetro que representa la posición 
* en la arrancará el personaje.
*/

void inicializar_personaje(personaje_t* ref_personaje, coordenada_t arranque_personaje) {
	(*ref_personaje).posicion = arranque_personaje;
	(*ref_personaje).vida = VIDAS_INICIALES;
	(*ref_personaje).puntos = 0;
	(*ref_personaje).tiene_llave = INICIO_FALSE;
	(*ref_personaje).interruptor_apretado = INICIO_FALSE;
}


/*
*   Procedimiento que dejará una estructura elemento_t que
* represente a la sombra, en un estado válido para iniciar el juego. 
* 'arranque_sombra' es un parámetro que representa la posición 
* en la arrancará la sombra.
*/

void inicializar_sombra(sombra_t* ref_sombra, coordenada_t arranque_sombra){
	(*ref_sombra).posicion = arranque_sombra;
    (*ref_sombra).esta_viva = INICIO_TRUE;
}

/*
*   La función recibe un caracter y devuelve true
* si el movimiento recibido concuerda con la convención
* propuesta, sino false.
* -> W: Si el personaje debe moverse para la arriba. 
* -> A: Si el personaje debe moverse para la izquierda.
* -> S: Si el personaje debe moverse para la abajo.
* -> D: Si el personaje debe moverse para la derecha.
*/


bool es_movimiento_valido(char movimiento){
	return movimiento == ARRIBA || movimiento == ABAJO || movimiento == IZQUIERDA || movimiento == DERECHA || movimiento == VIDA;
}


/*
* Procedimiento que se encargará de pedirle al usuario
* que ingrese un movimiento hasta que sea válido.
*/

void pedir_movimiento(char* ref_movimiento) {
	scanf(" %c", ref_movimiento );
	while(!es_movimiento_valido(*ref_movimiento)) {
		printf(INGRESE_MOVIMIENTO);
		scanf(" %c", ref_movimiento );
	}
}


/*
*    La función recibe una coordenada, devuelve true
* si la posición dentro de las dimensiones dadas, sino false.
* Una posición nunca podrá tener un valor negativo y no tendrá un 
* valor más alto que los máximos.
*/

bool esta_dentro_rango(coordenada_t posicion, int max_alto, int max_ancho){
	return  (posicion.fila < max_alto && posicion.fila >= 0) && (posicion.col < max_ancho && posicion.col >=0);

}

/*Funcion  que recibe una coordenada, un movimiento y un booleano.
*La coordenada debe estar previamente inicializada.
*El movimiento debe estar previamente  validado para cumplir con la convencion propuesta.
*el booleano debe estar inicializado al  igual que la coordenada.
Devuelve la futura coordenada acuerdo al movimiento dado.

* Si el booleano es true, se mueve de la forma:
* -> W: Si el personaje debe moverse para la arriba. 
* -> A: Si el personaje debe moverse para la izquierda
* -> S: Si el personaje debe moverse para la abajo.
* -> D: Si el personaje debe moverse para la derecha
*  Si el booleano es false, se intercambia el sentido en direccion horizontal: 
* -> D: Si el personaje debe moverse para la izquierda
* -> A: Si el personaje debe moverse para la derecha
*/

coordenada_t futura_coordenada(coordenada_t posicion, char movimiento, bool sentido){
	coordenada_t futura_posicion = posicion;
	if (movimiento == ARRIBA){
		futura_posicion.fila -=1;
	}
	if (movimiento == ABAJO){
		futura_posicion.fila +=1;
	}
	if (movimiento == DERECHA && sentido){
		futura_posicion.col +=1;
	}
	if (movimiento == IZQUIERDA && sentido){
		futura_posicion.col -=1;
	}
	if (movimiento == DERECHA && !sentido){
		futura_posicion.col -=1;
	}
	if (movimiento == IZQUIERDA && !sentido){
		futura_posicion.col +=1;
	}
	return futura_posicion;
}


/*
*   Función que dados una altura máxima y un ancho máximo
* devuelve una coordenada aleatoria. Una posición nunca podrá
* tener un valor negativo, ni ser mayor a sus máximos.
* -> A: Si el personaje debe moverse para la derecha.
* -> D: Si el personaje debe moverse para la izquierda.
-> W: Si el personaje debe moverse hacia arriba.
-> S: Si el personaje debe moverse hacia abajo.
*/

coordenada_t coordenada_aleatoria(int max_alto, int max_ancho) {
	coordenada_t posicion; 
	posicion.col = rand () % max_ancho;
	posicion.fila = rand () % max_alto;
	return posicion;
}


/*
*    La función recibe un vector de elementos, su respectivo tope y una coordenada,
* devuelve la posición del elemento del vector, que coincida con la coordenada pasada 'posicion',
* si no se encuentra ningún elemento, se devolverá -1. 
*/

int buscar_elemento(elemento_t elementos[MAX_ELEMENTOS], int tope, coordenada_t posicion){
	int posicion_vector = -1;
	bool posicion_encontrada = false;
	int i = 0;
	while (i < tope && !posicion_encontrada ) {
		if (coordenadas_iguales(elementos[i].coordenada, posicion)) {
			posicion_vector = i;
			posicion_encontrada = true;
		}
	i++;
	}
	return posicion_vector;
}



/*Procedimiento que recibe un de obstaculos, el tope por referencia y 4 datos de tipo int que corresponden a la cantidad de obstaculos a cargar.
Estos numeros no pueden ser negativos.
El tope del vector se inicializara en cero dentro del procedicimiento asi que no es necesario inicializarlo en cero  antes.
Llena el vector con los distintos tipos de obstaculos segun las cantidades pasadas por parametro. */

void cargar_vector_obstaculos (elemento_t obstaculos[MAX_ELEMENTOS], int *tope_obstaculos, int velas, int pozos, int interruptores, int portales) {
	*tope_obstaculos  = 0;
	int numero_obstaculos = velas + pozos + interruptores + portales;
	for (int i = 0; i < numero_obstaculos;  i++){
		if(velas>0) {
			obstaculos[i].tipo = VELA;
			velas--;
		} else if (pozos > 0){
			obstaculos[i].tipo = POZO;
			pozos--;
		} else if (interruptores > 0){
			obstaculos[i].tipo = INTERRUPTOR;
			interruptores--;
		} else if (portales > 0){
			obstaculos[i].tipo = PORTAL;
			portales--; }
		(*tope_obstaculos)++;
	}
}

/*Recibe un  vector de  elemento_t, el tope por referencia y 4 datos de tipo int que corresponden a la cantidad de herramientas a cargar.
 Estos numeros no pueden ser negativos.
El tope del vector se inicializara en cero dentro del procedicimiento asi que no es necesario inicializarlo en cero antes. 
Llena el vector de herramientas con los distintos tipo de herramientas segun  las cantidades pasadas por parametros.*/


void cargar_vector_herramientas (elemento_t herramientas[MAX_ELEMENTOS], int *tope_herramientas,int monedas, int llave, int puerta) {
	*tope_herramientas  = 0;
	int numero_herramientas =  monedas + llave + puerta;
	for (int i = 0; i < numero_herramientas;  i++){	
		if (monedas > 0){
			herramientas[i].tipo = MONEDA;
			monedas--;
		} else if (llave > 0){
			herramientas[i].tipo = LLAVE;
			llave--;
		} 
		else if (puerta > 0){
			herramientas[i].tipo = PUERTA;
			puerta--;
		} 
		(*tope_herramientas)++;
	}
}

/* Funcion que recibe una coordenada inicializada, un vector de coordenada_t correspondiente  a las  paredes, previamente cargado con las coordenadas y su tope por valor.  
Devuelve true si la coordenada coincide con alguna coordenada del vector*/

bool buscar_pared(coordenada_t paredes[MAX_PAREDES],int tope_paredes, coordenada_t posicion){
	bool posicion_encontrada = false;
	int i = 0;
	while (i < tope_paredes && !posicion_encontrada ) {
		if (coordenadas_iguales(paredes[i], posicion)) {
			posicion_encontrada = true;
		}
	i++;
	}
	return posicion_encontrada;
}

/*Procedimiento que recibe los vectores de herramientas, obstaculos y paredes y sus topes correspondientes. 
Los vectores de herramientas  y obstaculos ya deben estar cargados con los tipos  de herramientas u obstaculos.
El vector de paredes  ya debe estar  inicializado con las coordenadas correspondientes.
Genera una  coordenada aleatoria para las herramientas y obstaculos, validando que no se sobrepongan entre ellos y las paredes*/

void cargar_coordenadas(elemento_t vector_1[MAX_ELEMENTOS], int tope_1, elemento_t vector_2[MAX_ELEMENTOS], int tope_2, coordenada_t paredes[MAX_PAREDES], int  tope_paredes){
	for(int i = 0; i<tope_1; i++){
			vector_1[i].coordenada = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
			while((buscar_elemento(vector_1, i, vector_1[i].coordenada) !=-1)  || (buscar_pared(paredes, tope_paredes, vector_1[i].coordenada))){
				vector_1[i].coordenada = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
			}
	}
	for(int i = 0; i<tope_2; i++){
			vector_2[i].coordenada = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
			while((buscar_elemento(vector_2, i, vector_2[i].coordenada) !=-1)  || (buscar_elemento(vector_1, tope_1, vector_2[i].coordenada) !=-1) || (buscar_pared(paredes, tope_paredes, vector_2[i].coordenada))){
				vector_2[i].coordenada = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
			}
	}
}

/*Procedimiento que recibe un vector de paredes previamente inicializado y su tope, un vector de herramientas previamente cargado
con el tipo de herramientas, sus coordenadas y su tope por referencia, y el numero de  escaleras que se quiere generar, este numero no puede ser negativo.
Carga al vector de herramientas el numero de escaleras correspondientes, verificando que la posicion de la escalera coincida con 
una pared y no se sobreponga sobre  otra escalera previamente cargada*/

void generar_escaleras(coordenada_t paredes[MAX_PAREDES], int tope_paredes, int numero_escaleras, elemento_t herramientas[MAX_ELEMENTOS], int *tope_herramientas){
	coordenada_t escalera;
	for (int i = 0; i<numero_escaleras; i++){
		escalera  = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
		while((!buscar_pared(paredes, tope_paredes, escalera)) || (buscar_elemento(herramientas,*tope_herramientas, escalera)!= -1) ){
			escalera  = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
		}
		herramientas[*tope_herramientas].coordenada = escalera;
		herramientas[*tope_herramientas].tipo = ESCALERA;
		(*tope_herramientas)++;
	}
}


/*Procedimiento que recibe el nivel  por referencia, el numero del nivel y la cantidad de los distintos tipos de herramientas y obstaculos correspondientes al nivel.
Los numeros correspondientes a las herramientas y obstaculos no pueden ser negativos.
El numero del nivel tiene que ser entre 1 y 3, ambos inclusive.
Inicializa el nivel cargando los vectores de herramientas, obstaculos y paredes, y asigna el numero de nivel correspondiente*/

void generar_nivel (nivel_t* nivel, int numero_nivel, int velas, int pozos, int interruptores, int portales, int escaleras, int monedas, int llave, int  puerta) {
	(*nivel).numero_nivel = numero_nivel;
	obtener_mapa((*nivel).paredes, &(*nivel).tope_paredes);
	cargar_vector_obstaculos((*nivel).obstaculos, &(*nivel).tope_obstaculos, velas, pozos, interruptores, portales);
	cargar_vector_herramientas ((*nivel).herramientas, &(*nivel).tope_herramientas, monedas, llave, puerta);
	cargar_coordenadas((*nivel).obstaculos, (*nivel).tope_obstaculos, (*nivel).herramientas, (*nivel).tope_herramientas, (*nivel).paredes, (*nivel).tope_paredes);
	generar_escaleras((*nivel).paredes, (*nivel).tope_paredes, escaleras, (*nivel).herramientas, &(*nivel).tope_herramientas );
}

/* Funcion que recibe una coordenada previmente  inicializada y un nivel previamente  inicializado con sus  vectores.
La coordenada debe estar dentro del rango de la matriz. 
Compara la coordenada con las coordenadas de los elementos del vector herramientas, obstaculos y paredes del nivel.
Si coincide, devuelve el caracter del tipo de elemento con el que coincide, en caso contrario devuelve punto que corresponde a un espacio vacio*/

char comparar_coordenadas (coordenada_t coordenada, nivel_t nivel){
	char tipo = PUNTO;
	int posicion;

	if (buscar_pared(nivel.paredes,nivel.tope_paredes, coordenada)){
		tipo = PARED;
	}
	if (buscar_elemento(nivel.obstaculos, nivel.tope_obstaculos, coordenada) !=-1){
		posicion = buscar_elemento(nivel.obstaculos, nivel.tope_obstaculos, coordenada);
		tipo = nivel.obstaculos[posicion].tipo;
	}
	if (buscar_elemento(nivel.herramientas, nivel.tope_herramientas, coordenada) !=-1){
		posicion = buscar_elemento(nivel.herramientas, nivel.tope_herramientas, coordenada);
		tipo = nivel.herramientas[posicion].tipo;
	}
	return tipo;
}



/* Procedimiento que recibe las coordenadas de Max y Blue por referencia y el nivel en el que estan. 
Las coordenadas deben estar previamente inicializadas con una coordenada aleatoria y el nivel debe estar previamente inicializado con los vectores correspondientes.
Compara las coordenadas de Bloo y Max con las coordenadas de los elementos de los vectores de herramientas, obstaculos y paredes.
Cambia  las  coordenadas de Bloo Y Max hasta que no coincidan con algun elemento del nivel.*/

void  validar_coordenadas_personajes (coordenada_t* coordenada_max, coordenada_t* coordenada_blue, nivel_t nivel){
	while ((comparar_coordenadas(*coordenada_max, nivel) != PUNTO ) || (comparar_coordenadas(*coordenada_blue,nivel) != PUNTO ))
	*coordenada_max = coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
	(*coordenada_blue).fila = (*coordenada_max).fila;
	(*coordenada_blue).col = MAX_COLUMNAS - (*coordenada_max).col -1;

}

//  Procedimiento que recibe una coordenada, un vector de elementos y su tope por referencia. El vector tiene que ser de coordenada_t y debe estar previamente cargado.
// Elimina al elemento del vector que coincida con la coordenada recibida en el caso de que exista.


void eliminar_elemento(coordenada_t coordenada_buscada, elemento_t elementos[MAX_ELEMENTOS], int* tope) {
	int indice_buscado = buscar_elemento(elementos, *tope, coordenada_buscada);
	if(indice_buscado != -1){
		elementos[indice_buscado] = elementos[(*tope) -1];
	(*tope)--;
	}
}

/*Procedimiento que recibe al personaje por referencia,un movimiento y un nivel por referencia.
El movimiento tiene que estar previamente validado y el nivel tiene que  ser el  nivel actual(donde se encuentra el  personaje).
Mueve al personaje, saltandose todas  las  posiciones de las escaleras que haya en  su camino y luego las  elimina del vector de  obstaculos.
En el caso de que al final de las escaleras que tenga que pasar  hay una pared, no se  mueve al  personaje y no se eliminan  las escaleras*/

void pasar_escalera(personaje_t* ref_personaje, char movimiento, nivel_t* nivel){
	coordenada_t futura_posicion_personaje = futura_coordenada((*ref_personaje).posicion, movimiento, SENTIDO);
	char tipo_coordenada_personaje  = comparar_coordenadas (futura_posicion_personaje, *nivel);
	while( tipo_coordenada_personaje == ESCALERA){
    	futura_posicion_personaje = futura_coordenada(futura_posicion_personaje, movimiento,SENTIDO);
        tipo_coordenada_personaje  = comparar_coordenadas (futura_posicion_personaje, *nivel);
        }
        if(tipo_coordenada_personaje != PARED){
            futura_posicion_personaje = futura_coordenada((*ref_personaje).posicion, movimiento,  SENTIDO);
            tipo_coordenada_personaje  = comparar_coordenadas (futura_posicion_personaje, *nivel);
            while( tipo_coordenada_personaje == ESCALERA){
                eliminar_elemento(futura_posicion_personaje, (*nivel).herramientas, &(*nivel).tope_herramientas);
                futura_posicion_personaje = futura_coordenada(futura_posicion_personaje, movimiento,SENTIDO);
                tipo_coordenada_personaje  = comparar_coordenadas (futura_posicion_personaje, *nivel);
               	}
            (*ref_personaje).posicion = futura_posicion_personaje;
        } 
}

/*
*    El procedimiento recibe el personaje y la sombra por referencia, el nivel en el que se encuentran y el movimiento,
* y según este último, los mueve acordemente siempre y  cuando no se topen con una pared
* 
* El personaje se mueve de la forma:
* -> W: Si el personaje debe moverse para la arriba. 
* -> A: Si el personaje debe moverse para la izquierda.
* -> S: Si el personaje debe moverse para la abajo.
* -> D: Si el personaje debe moverse para la derecha.
*
*   La sombra se mueve de igual forma para el eje vertical, pero
* para el eje horizontal se moverá de forma inversa:
*
* -> A: Si el personaje debe moverse para la derecha.
* -> D: Si el personaje debe moverse para la izquierda.
*/

void mover_personaje(personaje_t* ref_personaje, sombra_t* ref_sombra, char movimiento, nivel_t* nivel){
	coordenada_t futura_posicion_personaje = futura_coordenada((*ref_personaje).posicion, movimiento, SENTIDO);
	char tipo_coordenada_personaje  = comparar_coordenadas (futura_posicion_personaje, *nivel);
	if (esta_dentro_rango(futura_posicion_personaje, MAX_FILAS, MAX_COLUMNAS) && tipo_coordenada_personaje != PARED){
		if(tipo_coordenada_personaje == ESCALERA){
			 pasar_escalera(ref_personaje,movimiento,nivel);
		}else{
			(*ref_personaje).posicion = futura_posicion_personaje;
			 }
	}
	coordenada_t futura_posicion_sombra = futura_coordenada((*ref_sombra).posicion, movimiento, (*ref_personaje).interruptor_apretado);
	char tipo_coordenada_sombra  = comparar_coordenadas (futura_posicion_sombra, *nivel);
	if ((esta_dentro_rango(futura_posicion_sombra, MAX_FILAS, MAX_COLUMNAS)) && (tipo_coordenada_sombra != PARED) && (tipo_coordenada_sombra != ESCALERA) && (*ref_sombra).esta_viva ){
		(*ref_sombra).posicion = futura_posicion_sombra;
	}
}

// Procedimiento que recibe al personaje por referencia y suma puntos aleatorios entre 10 y 20.

void sumar_puntos_aleatorios(personaje_t* personaje){
	(*personaje).puntos += rand () % 11 + 10;
}

//Procedimiento que recibe al personaje, a la sombra y el nivel en el que se encuentran.
//La sombra y el personaje deben estar por referencia y pueden o no estar previamente  inicializados.
//El nivel debe estar previamente inicializado con los obstaculos, herramientas y paredes.
//Asigna las coordenadas aleatoriamente sin sobreponerse con los elementos del nivel.

void asignar_coordenadas (personaje_t* personaje, sombra_t* sombra, nivel_t nivel){
	coordenada_t posicion_sombra;
	coordenada_t posicion_max;
	posicion_max = coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
	posicion_sombra.fila = posicion_max.fila;
	posicion_sombra.col = MAX_COLUMNAS - posicion_max.col -1;
	validar_coordenadas_personajes (&posicion_max, &posicion_sombra, nivel);
	(*personaje).posicion = posicion_max;
	(*sombra).posicion = posicion_sombra;
}

//Procedimiento que recibe dos coordenadas por referencia previamente inicializadas y las intercambia

void intercambiar_coordenadas(coordenada_t* coordenada_1, coordenada_t* coordenada_2){
	coordenada_t aux;
	aux = *coordenada_1;
	*coordenada_1 = *coordenada_2;
	*coordenada_2 = aux;
}

//Funcion que recibe un caracter, un vector de elementos y su tope.
//El caracter tiene que cumplir con la convencion  propuesta para  herramientas u obstaculos
//El vector debe estar  previamente cargado con las coordenadas y el tipo de herramienta u obstaculo.
//Devuelve la coordenada del ultimo elemento encontrado que coincida con la busqueda.

coordenada_t buscar_elemento_caracter(char tipo, elemento_t vector[MAX_ELEMENTOS], int tope){
	coordenada_t posicion;
	for(int i=0; i < tope; i++){
		if(vector[i].tipo == tipo){
			posicion = vector[i].coordenada;
		}
	}
	return posicion;
}

/*Funcion que recibe dos coordenadas  previamente inicializadas.
Devuelve la distancia Manhatan que  hay entre ellas*/

int calcular_distancia_manhatan(coordenada_t coordenada_1, coordenada_t coordenada_2){
	int distancia_x = abs(coordenada_1.col  - coordenada_2.col);
	int distancia_y = abs(coordenada_1.fila  - coordenada_2.fila);
	int distancia_manhatan = distancia_y + distancia_x;
	return distancia_manhatan;
}


//Funcion que recibe una coordenada y un vector de obstaculos.
//El vector debe estar previamente cargado tanto con las coordenadas como el tipo de obstaculo.
//La coordenada debe estar previamente inicializada y debe estar dentro del rango de la  matriz.
//Verifica si la coordenada esta dentro del rango de alguna de las velas del vector(El rango es una matriz 3x3 con la vela en el centro)
//Si esta dentro del rango de alguna vela devuelve true,  sino false.


bool esta_dentro_rango_vela (coordenada_t coordenada_blue, elemento_t obstaculos[MAX_ELEMENTOS], int tope){
	bool esta_en_rango =  false;
	for(int i = 0; i<tope; i++){
		if(obstaculos[i].tipo == VELA){
			if((calcular_distancia_manhatan(obstaculos[i].coordenada, coordenada_blue)==1)){
				esta_en_rango = true;
			}
			if((calcular_distancia_manhatan(obstaculos[i].coordenada, coordenada_blue)==2) && ((coordenada_blue.fila != obstaculos[i].coordenada.fila)&&(coordenada_blue.col != obstaculos[i].coordenada.col))){
				esta_en_rango = true;
			}
		}
	}
	return esta_en_rango;	
}



/*Funcion que recibe 3 coordenadas, la del personaje, la sombra y la puerta.
Las tres coordenadas deben estar previamente  inicializadas.
La coordenada de la puerta tiene que ser  la  del  nivel actual donde estan los  personajes.
Devuelve true si la coordenadas del personaje  y la coordenada de  la  sombra se encuentran a una distancia <=1 de  la puerta*/


bool distancia_manhatan_puerta(coordenada_t posicion_personaje, coordenada_t posicion_sombra, coordenada_t puerta){
	int distancia_personaje  = calcular_distancia_manhatan(posicion_personaje,puerta);
	int distancia_sombra = calcular_distancia_manhatan(posicion_sombra,puerta);
	return distancia_sombra <= 1 && distancia_personaje <=1;
}

/*Procedimiento que recibe  el  puntero al archivo y las variables para guardar la cantidad de herramientas y obstaculos que tendra cada nivel.
*El archivo al cual pertenece el  puntero se tuvo que abrir correctamente y tiene que ser de tipo configuaraciones: N«numero_nivel»_«elemento»=«cantidad».
*Asigna  los valores que contiene  el archivo de configuraciones*/

void cargar_configuraciones(FILE* puntero, int* N1_POZOS, int* N2_POZOS, int* N3_POZOS, int* N1_VELAS, int* N2_VELAS, int* N3_VELAS,int* N1_INTERRUPTORES, int* N2_INTERRUPTORES, int* N3_INTERRUPTORES,int* N1_PORTALES, int* N2_PORTALES, int* N3_PORTALES,int* N1_MONEDAS,int* N2_MONEDAS,int* N3_MONEDAS, int* N1_ESCALERAS, int* N2_ESCALERAS, int* N3_ESCALERAS, int* N1_LLAVES, int* N2_LLAVES, int* N3_LLAVES){
	char linea[MAX_NOMBRE];
    char* etiqueta;
    char* valor;
	   int leidos = fscanf(puntero,"%s\n", linea);
	   		while(leidos == 1){
	   			etiqueta = strtok(linea, "=");
		   		if(strcmp(etiqueta, N1_POZOS_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N1_POZOS = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N2_POZOS_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N2_POZOS = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N3_POZOS_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N3_POZOS = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N1_VELAS_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N1_VELAS = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N2_VELAS_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N2_VELAS = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N3_VELAS_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N3_VELAS = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N1_INTERRUPTORES_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N1_INTERRUPTORES = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N2_INTERRUPTORES_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N2_INTERRUPTORES = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N3_INTERRUPTORES_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N3_INTERRUPTORES = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N1_PORTALES_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N1_PORTALES = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N2_PORTALES_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N2_PORTALES = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N3_PORTALES_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N3_PORTALES = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N1_MONEDAS_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N1_MONEDAS = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N2_MONEDAS_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N2_MONEDAS = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N3_MONEDAS_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N3_MONEDAS = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N1_ESCALERAS_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N1_ESCALERAS = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N2_ESCALERAS_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N2_ESCALERAS = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N3_ESCALERAS_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N3_ESCALERAS = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N1_LLAVES_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N1_LLAVES = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N2_LLAVES_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N2_LLAVES = atoi(valor);
		   		}
		   		if(strcmp(etiqueta, N3_LLAVES_S) == 0){
		   			valor = strtok(NULL, "\0");
		   			*N3_LLAVES = atoi(valor);
		   		}
	   			leidos = fscanf(puntero,"%s\n", linea);
	   	}
	   	
}

void inicializar_juego(juego_t* juego) {
    int N1_POZOS = POZOS_N1;
    int N2_POZOS = POZOS_N2;
    int N3_POZOS = POZOS_N3;
    int N1_VELAS = VELAS_N1;
    int N2_VELAS = VELAS_N2;
    int N3_VELAS = VELAS_N3;
    int N1_INTERRUPTORES = INTERRUPTORES_N1;
    int N2_INTERRUPTORES = INTERRUPTORES_N2;
    int N3_INTERRUPTORES = INTERRUPTORES_N3;
    int N1_PORTALES = PORTALES_N1;
    int N2_PORTALES = PORTALES_N2;
    int N3_PORTALES = PORTALES_N3;
    int N1_MONEDAS = MONEDAS_N1;
    int N2_MONEDAS = MONEDAS_N2;
    int N3_MONEDAS = MONEDAS_N3;
    int N1_ESCALERAS = ESCALERAS_N1;
    int N2_ESCALERAS = ESCALERAS_N2;
    int N3_ESCALERAS = ESCALERAS_N3;
    int N1_LLAVES = LLAVE_N1;
    int N2_LLAVES = LLAVE_N2;
    int N3_LLAVES = LLAVE_N3;
    FILE* puntero = fopen(CONFIGURACIONES, LEER);
    if(puntero){
    	cargar_configuraciones(puntero,&N1_POZOS,&N2_POZOS,&N3_POZOS,&N1_VELAS,&N2_VELAS,&N3_VELAS,&N1_INTERRUPTORES,&N2_INTERRUPTORES,&N3_INTERRUPTORES,&N1_PORTALES,&N2_PORTALES,&N3_PORTALES,&N1_MONEDAS,&N2_MONEDAS,&N3_MONEDAS,&N1_ESCALERAS,&N2_ESCALERAS,&N3_ESCALERAS,&N1_LLAVES,&N2_LLAVES,&N3_LLAVES);
    	fclose(puntero);   
	}
	cargar_mapas();
	generar_nivel(&(*juego).niveles[0], 1, N1_VELAS, N1_POZOS, N1_INTERRUPTORES, N1_PORTALES, N1_ESCALERAS, N1_MONEDAS, N1_LLAVES, N_PUERTAS);
	generar_nivel(&(*juego).niveles[1], 2, N2_VELAS, N2_POZOS, N2_INTERRUPTORES, N2_PORTALES, N2_ESCALERAS, N2_MONEDAS, N2_LLAVES, N_PUERTAS);
	generar_nivel(&(*juego).niveles[2], 3, N3_VELAS, N3_POZOS, N3_INTERRUPTORES, N3_PORTALES, N3_ESCALERAS, N3_MONEDAS, N3_LLAVES, N_PUERTAS);
	asignar_coordenadas (&(*juego).personaje, &(*juego).sombra, (*juego).niveles[0]);
	inicializar_personaje(&(*juego).personaje, (*juego).personaje.posicion);
	inicializar_sombra(&(*juego).sombra, (*juego).sombra.posicion);
	(*juego).nivel_actual = 1;
}

int estado_juego(juego_t juego){
	int estado = 0;
	coordenada_t  puerta = buscar_elemento_caracter(PUERTA, juego.niveles[2].herramientas, juego.niveles[2].tope_herramientas);
	if(juego.personaje.vida == 0){
		estado = -1;
	} 
	if(juego.nivel_actual == 3 && (juego.personaje.tiene_llave) && (distancia_manhatan_puerta(juego.personaje.posicion, juego.sombra.posicion, puerta)) && (juego.sombra.esta_viva)){
		estado=1;
	}
	return estado;
}

int estado_nivel(juego_t juego){
	int estado_nivel = 0;
	int i = juego.nivel_actual -1;
	coordenada_t  puerta = buscar_elemento_caracter(PUERTA, juego.niveles[i].herramientas, juego.niveles[i].tope_herramientas);
		if(i==0){
			if((distancia_manhatan_puerta(juego.personaje.posicion, juego.sombra.posicion, puerta)) && (juego.sombra.esta_viva)){
			estado_nivel=1;
			}		
		} else{
			if((distancia_manhatan_puerta(juego.personaje.posicion, juego.sombra.posicion, puerta)) && (juego.personaje.tiene_llave) && (juego.sombra.esta_viva)){
			estado_nivel=1;
			}		
		}
	return estado_nivel;
}


/*Procedimiento que recibe al juego por referencia previamente inicializado.
Cambia el nivel actual al siguiente, dejando a los personajes en  condiciones iniciales para el nivel  nuevo*/

void cambiar_nivel (juego_t* juego){
	if((*juego).nivel_actual< MAX_NIVELES){
		(*juego).nivel_actual += 1;
		int  i = (*juego).nivel_actual -1;
		asignar_coordenadas (&(*juego).personaje, &(*juego).sombra, (*juego).niveles[i]);
		(*juego).personaje.tiene_llave = false;
		(*juego).personaje.interruptor_apretado= false;
	}
}

//Procedimiento que recibe al personaje por referencia.
//El personaje debe  estar previamente inicializado.
//Le suma una vida si no tiene sus vidas completas y si tiene 200 puntos para comprarla.

void sumar_vida(personaje_t* personaje){
	if(((*personaje).puntos>=PUNTOS_VIDA) && ((*personaje).vida < 3)){
		(*personaje).vida +=1;
		(*personaje).puntos -=PUNTOS_VIDA;
		}
}

//Procedimiento que recibe al personaje por referencia, previamente inicializado y cambia el estado del interruptor.

void cambiar_interruptor(personaje_t* personaje){
	if((*personaje).interruptor_apretado == false){
			(*personaje).interruptor_apretado = true;
		}else{
			(*personaje).interruptor_apretado = false;
		}
}

//Procedimiento que  recibe a la sombra y al personaje por referencia y el nivel en el que se  encuentran.
//La sombra, el personaje y el nivel deben  estar previamente inicializados.
//Revive a la  sombra si no se encuentra en el rango de una vela y resta 50 puntos al personaje.

void revivir_sombra(personaje_t* personaje, sombra_t* sombra, elemento_t obstaculos[MAX_ELEMENTOS], int tope){
	(*personaje).puntos -= PUNTOS_REVIVIR_SOMBRA;
	if(!(esta_dentro_rango_vela ((*sombra).posicion, obstaculos, tope))){
		(*sombra).esta_viva  = true;
		}
}

/*Procedimiento que recibe al personaje, la sombra y el nivel actual.
Los tres deben estar previamente  inicializados y pasados por  referencia.
De acuerdo al elemento sobre el cual se encuentra el personaje realiza la accion que corresponda:
*Moneda: Suma puntos  aleatorios.
*Interruptor: cambia el estado del interruptor a true o viceversa
*Pozo: Resta una  vida y reasigna las coordenadas aleatoriamente de ambos
*LLave: Cambia a true la propiedad tiene_llave del personaje
*Vela: Apaga la vela y la  elimina del vector*/

void acciones_personaje (personaje_t *personaje, sombra_t *sombra, nivel_t *nivel){
	char posicion_actual_personaje = comparar_coordenadas ((*personaje).posicion, *nivel);
	if(((posicion_actual_personaje == MONEDA) && !(coordenadas_iguales((*sombra).posicion, (*personaje).posicion))) || ((posicion_actual_personaje == MONEDA) && (coordenadas_iguales((*sombra).posicion, (*personaje).posicion)))){
		eliminar_elemento((*personaje).posicion, (*nivel).herramientas, &(*nivel).tope_herramientas);
		sumar_puntos_aleatorios(personaje);
	}
	if(posicion_actual_personaje == INTERRUPTOR){
		cambiar_interruptor(personaje);
	}
	if(posicion_actual_personaje == POZO){
		(*personaje).vida -= 1;
		asignar_coordenadas (personaje, sombra, *nivel);
	}
	if(posicion_actual_personaje == LLAVE){
		(*personaje).tiene_llave = true;
		eliminar_elemento((*personaje).posicion, (*nivel).herramientas, &((*nivel).tope_herramientas));
	}
	if(posicion_actual_personaje == VELA){
		eliminar_elemento((*personaje).posicion, (*nivel).obstaculos, &((*nivel).tope_obstaculos));
	}
}

/*Procedimiento que recibe al personaje, la sombra y el nivel actual.
Los tres deben estar previamente  inicializados y pasados por referencia.
De acuerdo al elemento sobre el cual se encuentra el la sombra realiza la accion que corresponda:
*Moneda: suma puntos aleatorios  al personaje
*Portal: Intercambian las posiciones Mac Y Bloo
*Si esta dentro  del rango de una vela,  muere Bloo
*Si Bloo esta muerto, las coordenadas de Bloo y Max son iguales y no estan dentro del rango de  una vela,  revive a Bloo y resta  50 puntos a Mac.
*/

void acciones_sombra (personaje_t *personaje, sombra_t *sombra, nivel_t *nivel){
	char posicion_actual_sombra = comparar_coordenadas ((*sombra).posicion, *nivel);
	if((posicion_actual_sombra == MONEDA) && !(coordenadas_iguales((*sombra).posicion, (*personaje).posicion))){
		eliminar_elemento((*sombra).posicion, (*nivel).herramientas, &(*nivel).tope_herramientas);
		sumar_puntos_aleatorios(personaje);
	}
	if(posicion_actual_sombra == PORTAL){
		intercambiar_coordenadas(&(*personaje).posicion, &(*sombra).posicion);
	}
	if(esta_dentro_rango_vela ((*sombra).posicion, (*nivel).obstaculos, (*nivel).tope_obstaculos)){
		(*sombra).esta_viva = false;
	}
	if((coordenadas_iguales((*sombra).posicion, (*personaje).posicion)) && !((*sombra).esta_viva)){
		revivir_sombra(personaje, sombra, (*nivel).obstaculos, (*nivel).tope_obstaculos);
	}
}

void realizar_jugada(juego_t* juego){
	char movimiento;
	pedir_movimiento(&movimiento);
	if (movimiento == VIDA){
		sumar_vida(&(*juego).personaje);
	}
	int i = (*juego).nivel_actual -1;
	if(esta_dentro_rango_vela((*juego).sombra.posicion, (*juego).niveles[i].obstaculos, (*juego).niveles[i].tope_obstaculos)){
		(*juego).sombra.esta_viva = false;
	}
	mover_personaje(&(*juego).personaje, &(*juego).sombra, movimiento, &(*juego).niveles[i]);
	acciones_personaje(&(*juego).personaje, &(*juego).sombra, &(*juego).niveles[i]);
	acciones_sombra(&(*juego).personaje, &(*juego).sombra,&(*juego).niveles[i]);
	if(estado_nivel(*juego)==1){
		cambiar_nivel (juego);
	}
}

/*Procedimiento que recibe el terreno, los vectores de paredes, obstaculos y  herramientas con  sus respectivos topes.
El terreno debe estar previamente inicializado con los caracteres que representan  los  espacios vacios.
Los vectores deben estar  previamente cargados con los elementos que correspondan, es decir, sus coordenadas y tipo.
Posiciona a cada  elemento  de los vectores en la matriz,  en la coordenada que  cada uno posee y su caracter para  ser  representado*/

void cargar_vectores_matriz(char terreno[MAX_FILAS][MAX_COLUMNAS], coordenada_t paredes[MAX_PAREDES], int tope_paredes, elemento_t obstaculos[MAX_ELEMENTOS], int tope_obstaculos, elemento_t herramientas[MAX_ELEMENTOS], int tope_herramientas){
for (int i = 0;  i < tope_obstaculos; i++){
		terreno[obstaculos[i].coordenada.fila][obstaculos[i].coordenada.col] = obstaculos[i].tipo;
	}
	for (int i = 0;  i < tope_paredes; i++){
		terreno[paredes[i].fila][paredes[i].col] =  PARED;
	}
	for (int i = 0;  i < tope_herramientas; i++){
		terreno[herramientas[i].coordenada.fila][herramientas[i].coordenada.col] = herramientas[i].tipo;
	}
}

/*Procedimiento que  recibe el numero de vidas y puntos del personaje, un  booleano que representa si el  personaje tiene la llave o no
y el nivel actual del juego.
Imprime por consola dichos valores*/

void imprimir_datos(int vidas, int puntos, bool llave, int nivel_actual){
	char tiene_llave[MAX_ELEMENTOS];
	if (llave){
		strcpy(tiene_llave, SI);
	}else{
		strcpy(tiene_llave, NO);
	}
	printf(FORMATO_DATOS, vidas, puntos, tiene_llave, nivel_actual);
}

void imprimir_terreno(juego_t juego) {
	int  nivel = juego.nivel_actual -1;
	char terreno[MAX_FILAS][MAX_COLUMNAS];
	for (int i = 0; i< MAX_FILAS; i++) {
		for(int j = 0; j <  MAX_COLUMNAS; j++) {
			terreno[i][j] = PUNTO;
		}
	}
	cargar_vectores_matriz(terreno, juego.niveles[nivel].paredes, juego.niveles[nivel].tope_paredes, juego.niveles[nivel].obstaculos, juego.niveles[nivel].tope_obstaculos, juego.niveles[nivel].herramientas, juego.niveles[nivel].tope_herramientas);
	terreno[juego.personaje.posicion.fila][juego.personaje.posicion.col] = PERSONAJE;
	terreno[juego.sombra.posicion.fila][juego.sombra.posicion.col] = SOMBRA;
	for (int i = 0; i< MAX_FILAS; i++) {
		for (int j = 0; j< MAX_COLUMNAS; j++) {
			printf(" %c ", terreno[i][j]);
		}
		printf("\n");
	}
	imprimir_datos(juego.personaje.vida, juego.personaje.puntos, juego.personaje.tiene_llave, juego.nivel_actual);
}


//Funcion que recibe el  resultado de la partida
//Devuelve true si  el resultado es "Si" o "No"

bool es_resultado_valido (char resultado_nuevo[MAX_NOMBRE]){
	bool es_valido = false;
	if ((strcmp(resultado_nuevo, NO)==0) || (strcmp(resultado_nuevo, SI)==0)){
		es_valido = true;
	}
    return es_valido;
}

//Funcion que recibe el nivel llegado
//Devuelve true si nivel cumple con alguno de los niveles del juego: 1, 2 o 3.

bool es_nivel_valido (int nivel_llegado){
    return (nivel_llegado <= MAX_NIVELES) && (nivel_llegado > 0);
}

//Funcion que recibe las  vidas restantes
//Devuelve true si las vidas estan entre 0 y 3.

bool es_vida_valida (int vidas_restantes){
    return (vidas_restantes <= VIDAS_INICIALES) && (vidas_restantes >-1);
}


/*Procedimiento que recibe una partida y un string, ambos previamente inicializados
*Los datos ingresados por el usuario deben cumplir con la convencion propuesta:
	*El nombre no puede contener espacios vacios, si son  varias palabras, deben estar unidas mediante un guion  bajo.
	*La inicial del nombre debe estar en mayuscula.
*Guarda los datos ingresados por consola en los campos del struct partida*/

void pedir_datos(partida_t* partida_nueva,  char resultado_nuevo[MAX_NOMBRE]){
	printf("%s\n", INGRESAR_NOMBRE);
    scanf("%s", (*partida_nueva).jugador);
    printf("%s\n", INGRESAR_NIVEL);
    scanf("%i", &(*partida_nueva).nivel_llegado);
    while(!es_nivel_valido((*partida_nueva).nivel_llegado)){
    	printf("%s\n", INGRESAR_NIVEL);
    	scanf("%i", &(*partida_nueva).nivel_llegado);
    }
    printf("%s\n", INGRESAR_PUNTOS);
    scanf("%i", &(*partida_nueva).puntos);
    printf("%s\n", INGRESAR_VIDAS);
    scanf("%i", &(*partida_nueva).vidas_restantes);
    while(!es_vida_valida ((*partida_nueva).vidas_restantes)){
    	printf("%s\n", INGRESAR_VIDAS);
    	scanf("%i", &(*partida_nueva).vidas_restantes);
    }
    printf("%s\n", INGRESAR_RESULTADO);
    scanf("%s", resultado_nuevo);
    while(!es_resultado_valido (resultado_nuevo)){
    	printf("%s\n", INGRESAR_RESULTADO);
    	scanf("%s", resultado_nuevo);
    }
}

int agregar_partida (char nombre_archivo[MAX_ELEMENTOS]){
    FILE* puntero = fopen(nombre_archivo, LEER);
    if(!puntero){
        perror(ERROR);
        return -1;
    }
    FILE* puntero_aux = fopen(AUXILIAR, ESCRIBIR);
    if(!puntero_aux){
        fclose(puntero);
        perror(ERROR);
        return -1;
    }
    partida_t partida_nueva;
 	char resultado_nuevo[MAX_NOMBRE];
 	pedir_datos(&partida_nueva, resultado_nuevo);
    partida_nueva.gano = strcmp(resultado_nuevo, NO);
    char resultado[MAX_NOMBRE];
    partida_t partida;
    bool agregado = false;
    int leidos = fscanf(puntero,FORMATO_LECTURA, partida.jugador, &(partida.nivel_llegado), &(partida.puntos), &(partida.vidas_restantes), resultado);
    while(leidos == 5){
    	if((strcmp(partida_nueva.jugador,partida.jugador) <=0) && !agregado){
    		fprintf(puntero_aux, FORMATO_IMPRESION, partida_nueva.jugador, partida_nueva.nivel_llegado, partida_nueva.puntos,partida_nueva.vidas_restantes, resultado_nuevo);
    		agregado = true;
    	}
    	fprintf(puntero_aux, FORMATO_IMPRESION, partida.jugador, partida.nivel_llegado, partida.puntos,partida.vidas_restantes, resultado);
    	leidos = fscanf(puntero, FORMATO_LECTURA, partida.jugador, &(partida.nivel_llegado), &(partida.puntos), &(partida.vidas_restantes), resultado);
    }
    fclose(puntero);
    fclose(puntero_aux);
    rename(AUXILIAR, nombre_archivo);
    return 0;
}

int eliminar_partida (char nombre_jugador[MAX_NOMBRE], char nombre_archivo[MAX_NOMBRE]){
	FILE* puntero = fopen(nombre_archivo, LEER);
    if(!puntero){
        perror(ERROR);
        return -1;
    }
    FILE* puntero_aux = fopen(AUXILIAR, ESCRIBIR);
    if(!puntero_aux){
        fclose(puntero);
        perror(ERROR);
        return -1;
    }
    partida_t partida;
    char resultado[MAX_NOMBRE];
   	bool eliminado = false;
    int leidos = fscanf(puntero,FORMATO_LECTURA, partida.jugador, &(partida.nivel_llegado), &(partida.puntos), &(partida.vidas_restantes), resultado);
    while(leidos==5){
    	if((strcmp(nombre_jugador,partida.jugador) != 0) || ((strcmp(nombre_jugador,partida.jugador) == 0) && (eliminado))){
    		fprintf(puntero_aux, FORMATO_IMPRESION, partida.jugador, partida.nivel_llegado, partida.puntos,partida.vidas_restantes, resultado);
    	} else {
    		eliminado = true;
    	}
    	leidos = fscanf(puntero,FORMATO_LECTURA, partida.jugador, &(partida.nivel_llegado), &(partida.puntos), &(partida.vidas_restantes), resultado);
    }
    fclose(puntero);
    fclose(puntero_aux);
    rename(AUXILIAR, nombre_archivo);
    return 0;
}

//Procedimiento que recibe un vector de partida_t y su tope y lo ordena alfabeticamente segun los nombres de los jugadores
//El vector debe estar previamente cargado con las  partidas y los nombres deben cumplir con la convencion propuesta.

void ordenar_vector (partida_t partidas[MAX_ELEMENTOS], int tope){
	partida_t aux;
    for (int i = 1; i < tope; i++) {
        for (int j = 0; j < tope - i; j++){
            if(strcmp(partidas[j].jugador, partidas[j+1].jugador) > 0){
                aux = partidas[j];
                partidas[j] = partidas[j+1];
                partidas[j+1] = aux;
            }
        }

    }
}

/*Procedimiento que recibe  un vector de partidas, su tope y el puntero  a un archivo.
El archivo debe estar abierto correctamente
Imprime en el archivo las  partidas guardadas en el vector*/

void imprimir_vector_partidas(partida_t partidas[MAX_ELEMENTOS], int tope, FILE* puntero_aux){
	char resultado[MAX_NOMBRE];
	for(int i = 0; i<tope; i++){
     	strcpy(resultado, NO);
     	if(partidas[i].gano){
     		strcpy(resultado, SI);
     	}
     	fprintf(puntero_aux, FORMATO_IMPRESION, partidas[i].jugador, partidas[i].nivel_llegado, partidas[i].puntos,partidas[i].vidas_restantes, resultado);
     }
}

/*Procedimiento que recibe  un vector de partidas, su tope y el puntero  a un archivo.
El archivo debe estar abierto correctamente y ser de tipo csv.
El archivo debe tener todos los campos completados correspondientes a las partidas.
El tope del vector  debe estar inicializado en 0.
Carga el vector con las partidas guardadas en el archivo*/

void llenar_vector_partidas(partida_t partidas[MAX_ELEMENTOS], int* tope, FILE* puntero){
	char resultado[MAX_NOMBRE];
    int leidos = fscanf(puntero,FORMATO_LECTURA, partidas[*tope].jugador, &(partidas[*tope].nivel_llegado), &(partidas[*tope].puntos), &(partidas[*tope].vidas_restantes), resultado);
    partidas[*tope].gano = strcmp(resultado, NO);
     while(leidos == 5){
     	(*tope)++;
     	leidos = fscanf(puntero,FORMATO_LECTURA, partidas[*tope].jugador, &(partidas[*tope].nivel_llegado), &(partidas[*tope].puntos), &(partidas[*tope].vidas_restantes), resultado);
     	partidas[*tope].gano = strcmp(resultado, NO);
    }
}

int ordenar_partidas(char nombre_archivo[MAX_NOMBRE]){
	FILE* puntero = fopen(nombre_archivo, LEER);  
    if(!puntero){
        perror(ERROR);
        return -1;
    }
    FILE* puntero_aux = fopen(AUXILIAR, ESCRIBIR);
    
    if(!puntero_aux){
        fclose(puntero);
        perror(ERROR);
        return -1;
    }
    partida_t partidas[MAX_ELEMENTOS];
    int tope = 0;
    llenar_vector_partidas(partidas, &tope, puntero);
    ordenar_vector(partidas,tope);
    imprimir_vector_partidas(partidas,tope,puntero_aux);
    fclose(puntero);
    fclose(puntero_aux);
    rename(AUXILIAR, nombre_archivo);
    return 0;
}
                                            