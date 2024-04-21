#include "funciones.h"

int main(){
    int puntaje = 0, miguel = 0, rolando = 0, tabano = 0, play, top[10];
    topo* tablero[5][5];

    inicializar_tablero(tablero);

    printf("Bienvenid@ a Rompe-Topos 3000!\nLos topos se escapan cada %i segundos y solo tienes 10 golpes\nPodras derrotarlos y convertirte en el Rey de los topos?\n(1: Si -- 9: Modo automatico -- 0: Salir):\n", WAIT);
    play = leer();
    printf("---------------------------------\n");

    while(play != 1 && play != 9 && play != 0){
        printf("Opcion invalida, intente de nuevo\n");
        printf("Desea jugar? (1: Si -- 9: Modo automatico -- 0: Salir):\n");
        play = leer();
        printf("---------------------------------\n");
    }

    while(play == 1 || play == 9){
        if (play == 1) ready();
        for(int golpes=9; golpes>=0; golpes--){
            system("clear");
            generar_topos(tablero, &miguel, &rolando, &tabano);
            imprimir_tablero(tablero, puntaje, golpes);
            golpear(tablero, &puntaje, &golpes, play);
            sleep(2);
        }

        ranking(puntaje, top);
        puntaje = 0;
        printf("Desea volver a jugar? (1: Si -- 9: Modo automatico -- 0: Salir):\n");
        play = leer();
        printf("---------------------------------\n");
        while(play != 1 && play != 9 && play != 0){
            printf("Opcion invalida, intente de nuevo\n");
            printf("Desea jugar? (1: Si -- 9: Modo automatico -- 0: Salir):\n");
            play = leer();
            printf("---------------------------------\n");
        }
    }

    liberar_tablero(tablero);
    return 0;
}