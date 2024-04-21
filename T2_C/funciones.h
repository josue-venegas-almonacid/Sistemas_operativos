#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 
#include <sys/wait.h> 
#include <sys/syscall.h>
#include <string.h>

#define READER 0
#define WRITER 1
#define WAIT 5

typedef struct topo{
    int x, y, iteraciones, puntaje;
} topo;

void inicializar_tablero(topo* tablero[5][5]);
void imprimir_tablero(topo* tablero[5][5], int puntaje, int golpes);
void liberar_tablero(topo* tablero[5][5]);
int generar_topos(topo* tablero[5][5],int* miguel, int* rolando, int* tabano);
void golpear(topo* tablero[5][5], int* puntaje, int* golpes, int modo);
void ranking(int puntaje, int top[10]);
void new_record(int puntaje, int posicion, int top[10]);
void ready();
int leer();


void inicializar_tablero(topo* tablero[5][5]){
    for (int i=0; i<5; i++){
        for (int j=0; j<5; j++){
            topo* topo_nuevo = (topo*)malloc(sizeof(topo));
            topo_nuevo->x = i;
            topo_nuevo->y = j;
            topo_nuevo->iteraciones = 0;
            topo_nuevo->puntaje = 0;
            tablero[i][j] = topo_nuevo;
        }
    }
}


void imprimir_tablero(topo* tablero[5][5], int puntaje, int golpes){
    //Imprimo golpes restantes, puntaje actual y tablero, donde X es un topo y 0 un espacio vacio
    printf("---------------------------------\n");
    printf("       Golpes restantes: %i\n", golpes);
    printf("           Puntaje: %i\n", puntaje);
    printf("---------------------------------\n");
    for (int i=0; i<5; i++){
        printf("------  |");
        for (int j=0; j<5; j++){
            if (tablero[i][j]->iteraciones > 0) printf(" P ");
            else printf(" 0 ");
        }
        printf("|  ------");
        printf("\n");
    }
    printf("---------------------------------\n");
}


void liberar_tablero(topo* tablero[5][5]){
    for (int i=0; i<5; i++){
        for (int j=0; j<5; j++){
            free(tablero[i][j]);
        }
    }
}


void ready(){
    printf("El juego empieza en...\n");
    sleep(1);
    for (int i = 3; i>0; i--){
        printf("%i...\n", i);
        sleep(1);
    }
    printf("YA!!\n");
    sleep(1);
}


int leer(){
    char splay[10] = {0};
    int play;
    
    int read_bytes = read(0, splay, 9);
    if(read_bytes==2){
        play = atoi(&splay[0]);
    }
    else play = -23;
    return play;
}


void golpear(topo* tablero[5][5], int* puntaje, int* golpes, int modo){
    int x, y;

    //Si es modo manual, pregunta coordenadas; si es automatico, las genera al azar
    if (modo == 1){
        //Variables para guardar lo que leo antes de que pasen WAIT segundos
        char coordenada[10] = {0};
        fd_set input_set;
        struct timeval timeout;
        int ready_for_reading = 0;
        int read_bytes = 0;

        FD_ZERO(&input_set);
        FD_SET(0, &input_set);
        timeout.tv_sec = WAIT;
        timeout.tv_usec = 0;

        //Empiezo a leer
        printf("Ingrese coordenada fila,columna: \n");
        ready_for_reading = select(1, &input_set, NULL, NULL, &timeout);

        if (ready_for_reading){
            read_bytes = read(0, coordenada, 9);

            //Si leo una coordenada tipo x,y
            if(read_bytes==4){
                x = atoi(&coordenada[0]);
                y = atoi(&coordenada[2]);

                //Le resto 1 a ambas coordenadas para que sea valido en el tablero (el cual va del 0-4)
                x-=1;
                y-=1;
            } 

            //Si recibo una coordenada invalida
            else{
                x = -1;
                y = -1;
            }
        } 

        //Si se acaba el tiempo
        else {
            x = -23;
            y = -23;
            *golpes+=1;
        }
    }

    else if (modo == 9){
        x = rand()%6;
        y = rand()%6;
        printf("Golpeando %i,%i...\n", x, y);
    }
    
    //Si golpea una posicion sin un topo o una posicion fuera del tablero
    if (x>4 || x<0 || y>4 || y<0 || (tablero[x][y]->iteraciones)==0){
        if (x == -23 && y == -23);
        else {
            printf("Ay! has errado el golpe\n");
            printf("---------------------------------\n");
        }
    }

    //Si golpea un topo muestra un mensaje, lo elimina y suma el puntaje obtenido
    else if ((tablero[x][y]->iteraciones)>0){
        if (tablero[x][y]->puntaje == 1000) printf("WOW! Golpeaste al topo MIGUEL y obtienes 1000 puntos!!\n");
        else if (tablero[x][y]->puntaje == 2500) printf("GENIAL! Golpeaste al topo ROLANDO y obtienes 2500 puntos!!\n");
        else if (tablero[x][y]->puntaje == 5000) printf("INCREIBLE! Golpeaste al topo TABANO y obtienes 5000 puntos!!\n");
        else printf("Topo eliminado! Obtienes %i puntos\n", tablero[x][y]->puntaje);

        printf("---------------------------------\n");
        tablero[x][y]->iteraciones = 0;
        *puntaje += tablero[x][y]->puntaje;
    }

    //Se les resta 1 a las iteraciones restantes de los otros topos
    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            if ((tablero[i][j]->iteraciones) != 0){
                tablero[i][j]->iteraciones -= 1;
            }
        }
    }
}


void ranking(int puntaje, int top[10]){
    //Abre el archivo de ranking
    FILE *fp;
    char* filename = "./ranking.txt";

    //Si no existe lo creo y guardo directamente el puntaje actual
    fp = fopen(filename, "r");
    if (fp == NULL){
        fp = fopen(filename, "w");
        printf("FELICITACIONES! Te has posicionado en el lugar N°1 del ranking\n");
        for(int i=0; i<10; i++){
            //Escribo en el archivo el puntaje actual y los otros con un 0
            if (i==0) top[i] = puntaje;
            else top[i] = 0;
            fprintf(fp, "%i\n", top[i]);
        }
    }

    //Si existe verifico el puntaje maximo
    else{
        int cont = 0;
        char buffer[11];

        //Guardo los puntajes del archivo en el arreglo top10
        while (fgets(buffer,100,fp)){
            strtok(buffer, "\n");
            if (cont < 10){
                top[cont] = atoi(buffer);
                cont+=1;
            }
        }

        //Recorro el arreglo top10 y veo si el puntaje actual supera alguno
        for (int i=0; i<10; i++){
            //En caso positivo, arrastro los puntajes siguientes hacia abajo
            if (puntaje > top[i]){
                new_record(puntaje, i, top);
                printf("FELICITACIONES! Te has posicionado en el lugar N°%i del ranking\n", i+1);
                break;
            }
        }

        //Escribo la tabla potencialmente actualizada en el archivo
        fclose(fp);
        fp = fopen(filename, "w");
        for (int i=0; i<10; i++){
            fprintf(fp, "%i\n", top[i]);
        }
    }

    //En cualquier caso muestra el puntaje obtenido
    printf("Tu puntaje fue de %i\n", puntaje);
    printf("---------------------------------\n");
    
    //Cierro el archivo
    fclose(fp);
}

void new_record(int puntaje, int posicion, int top[10]){
    int i;
    //Los valores siguientes al puntaje son desplazados
    for (i = 9; i > posicion; i--){
        top[i] = top[i-1];
    }
    //Guardo el nuevo puntaje en la posicion correspondiente
    top[i] = puntaje;
}


int generar_topos(topo* tablero[5][5],int* miguel, int* rolando,int* tabano){
    srand(time(NULL));
    //Pipes de lectura y escritura
    int fd1_2[2], fd2_3[2], fd3_0[2];
    //Identificadores padre-hijo
    int pid, status;
    //Informacion de los topos generados
    int cantidad;
    topo* nuevos_topos = (topo*)malloc(25*sizeof(topo));

    if (pipe(fd1_2) == -1 || pipe(fd2_3) == -1 || pipe(fd3_0) == -1){
        printf("Error al abrir un pipe\n");
        return -1;
    }

    for(int i=0; i<3; i++){
        pid = fork();

        if (pid == -1){
            printf("Error al crear un hijo\n");
            exit (-1);
        }

        //En todos los hijos tengo todos los pipes abiertos
        else if (pid == 0){
            if (i == 0){
                //Hijo 1 determina la cantidad de topos
                //Solo dejo abierto el extremo de escritura de pipe1_2
                close(fd1_2[READER]);
                close(fd2_3[READER]);
                close(fd2_3[WRITER]);
                close(fd3_0[READER]);
                close(fd3_0[WRITER]);

                //Genero los datos y los envio al hijo 2
                cantidad = rand()%3+1;
                write(fd1_2[WRITER], &cantidad, sizeof(int));

                //Cierro el extremo de escritura de pipe1_2
                close(fd1_2[WRITER]);

                exit(1);
            }
            else if (i == 1){
                //Hijo 2 determina las iteraciones de cada topo
                //Solo dejo abierto el extremo de lectura del pipe1_2 y de escritura de pipe2_3
                close(fd1_2[WRITER]);
                close(fd2_3[READER]);
                close(fd3_0[READER]);
                close(fd3_0[WRITER]);

                //Recupero lo enviado por el hijo 1
                while(read(fd1_2[READER], &cantidad, sizeof(int))<=0){};

                //Cierro el extremo de lectura de pipe1_2
                close(fd1_2[READER]);

                //Genero los datos y los envio al hijo 3
                for(int i=0; i<cantidad; i++){
                    //Para hacer el juego mas divertido, le asigno distinto puntaje a cada topo
                    int secret = rand()%10;
                    if (*miguel == 0 && secret == 2){
                        nuevos_topos[i].puntaje = 1000;
                        nuevos_topos[i].iteraciones = 1;
                        *miguel = 1;
                    }
                    else if (*rolando == 0 && secret == 3){
                        nuevos_topos[i].puntaje = 2500;
                        nuevos_topos[i].iteraciones = 1;
                        *rolando = 1;
                    }
                    else if (*tabano == 0 && secret == 9){
                        nuevos_topos[i].puntaje = 5000;
                        nuevos_topos[i].iteraciones = 1;
                        *tabano = 1;
                    }
                    else {
                        int dice = rand()%3+1;
                        nuevos_topos[i].puntaje = 500/dice;
                        nuevos_topos[i].iteraciones = dice;
                    }
                }
                write(fd2_3[WRITER], &cantidad, sizeof(int));
                write(fd2_3[WRITER], nuevos_topos, cantidad*sizeof(topo));

                //Cierro el extremo de escritura de pipe2_3
                close(fd2_3[WRITER]);

                exit(2);
            }
            else if (i == 2){
                //Hijo 3 determina la posicion de cada topo
                //Solo dejo abierto el extremo de lectura del pipe2_3 y de escritura de pipe3_0
                close(fd1_2[READER]);
                close(fd1_2[WRITER]);
                close(fd2_3[WRITER]);
                close(fd3_0[READER]);

                //Recupero lo enviado por el hijo 2
                while(read(fd2_3[READER], &cantidad, sizeof(int))<=0){};
                while(read(fd2_3[READER], nuevos_topos, cantidad*sizeof(topo))<=0){};

                //Cierro el extremo de lectura de pipe2_3
                close(fd2_3[READER]);
                
                //Genero los datos y los envio al padre
                for(int i=0; i<cantidad; i++){
                    int rand_x = rand()%5;
                    int rand_y = rand()%5;
                    while (tablero[rand_x][rand_y]->iteraciones != 0){
                        rand_x = rand()%5;
                        rand_y = rand()%5;
                    }
                    nuevos_topos[i].x = rand_x;
                    nuevos_topos[i].y = rand_y;
                }
                write(fd3_0[WRITER], &cantidad, sizeof(int));
                write(fd3_0[WRITER], nuevos_topos, cantidad*sizeof(topo));

                //Cierro el extremo de escritura de pipe3_0
                close(fd3_0[WRITER]);

                exit(3);
            }
        }
    }

    //Vuelvo a estar en el padre
    //Espero a que finalicen todos los hijos
    for(int i=0; i<3; i++){
        wait(&status);
    }

    //Recupero lo enviado por los hijos
    while(read(fd3_0[READER], &cantidad, sizeof(int))<=0){};
    while(read(fd3_0[READER], nuevos_topos, cantidad*sizeof(topo))<=0){};
    //Actualizo el tablero
    for(int i=0; i<cantidad; i++){
        tablero[nuevos_topos[i].x][nuevos_topos[i].y]->iteraciones = nuevos_topos[i].iteraciones;
        tablero[nuevos_topos[i].x][nuevos_topos[i].y]->puntaje = nuevos_topos[i].puntaje;
    }

    //Libero memoria
    free(nuevos_topos);

    //Cierro los pipes
    close(fd1_2[READER]);
    close(fd1_2[WRITER]);
    close(fd2_3[READER]);
    close(fd2_3[WRITER]);
    close(fd3_0[READER]);
    close(fd3_0[WRITER]);

    return 0;
}