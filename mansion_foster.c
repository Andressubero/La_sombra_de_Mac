#include <stdio.h> 
#include <time.h>
#include "la_sombra_de_mac.h"
#include <stdlib.h> 
#include <string.h> 
#define AGREGAR "agregar_partida"
#define ELIMINAR "eliminar_partida"
#define ORDENAR "ordenar_partidas"
#define CONFIGURAR "config_juego"
#define GANASTE "GANASTE!!"
#define PERDISTE "PERDISTE!!"




int main (int argc, char *argv[]) {

	if((argc==3) && (strcmp(argv[1], AGREGAR) == 0)){
		agregar_partida(argv[2]);
	}

	if((argc==4) && (strcmp(argv[1],ELIMINAR) == 0)){

	 	eliminar_partida (argv[3], argv[2]);
	}

	if((argc==3)&&(strcmp(argv[1], ORDENAR) == 0)){
		ordenar_partidas(argv[2]);
	}

	if((argc==2)&&(strcmp(argv[1], CONFIGURAR) == 0)){

		srand (( unsigned)time(NULL)); // Genera la semilla aleatoria
		juego_t juego;
		inicializar_juego(&juego);
		int i= 0;
		while (estado_juego(juego) == 0) {
			system("clear");
			imprimir_terreno(juego);
			realizar_jugada(&juego);
			i++;
		}

		if (estado_juego(juego) == 1){
			printf("%s\n", GANASTE);
		} else {
			printf("%s\n",PERDISTE);
		}
	}
	

	







	




	return 0;
}

