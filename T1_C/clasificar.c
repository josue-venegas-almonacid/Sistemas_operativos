#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "structs.h"

void procesarArchivo(char *archivo, int n_files, Autor *lista_autores, int *arch, int *pos, int *pos2, char u[]);
void ordenarAutores(Autor lista_origen[], Autor lista_destino[], int size);
void moverArchivos(char home[], char nom_autor[], char nom_libro[], char cat[], char ubi[], int i);
void menu_principal(int *opcion1, char home[], char *from, char *current, int *lvl, Autor lista[], int cant_autores);
void dir_manager(int *opcion1, char home[], char *from, char *current, Autor lista[], int cant_autores, int *lvl);
void recomendar_categoria(int *opcion1, char home[], Autor lista[], int cant_autores);
void recomendar_azar(int *opcion1, char home[], Autor lista[], int cant_autores);
void printDirArch(char current[]);


int main(){
    DIR *dir;                               //Manejo de directorios
    struct dirent *ent;                     //Manejo de archivos
    int n_files = 0;                        //Guarda cuantos archivos fueron leidos
    int arch = 0;                           //Contador en que archivo voy
    int pos = 0;                            //Contador en que posicion guardo al autor
    int pos2 = 0;                           //Contador en que posicion guardo la categoria
    char home[100] = "./libros/";           //Ubicacion de los archivos
    
    //Intento abrir la carpeta
    dir = opendir(home);
    if (dir == NULL){
        printf("[ERROR] El directorio no se pudo abrir\n");
        return 1;
    }
    //Recorro la carpeta para obtener la cantidad de archivos
    while ((ent = readdir (dir)) != NULL){
        if ((strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0)){
            n_files+=1;
        }
    }
    //Cierro la carpeta
    closedir(dir);

    if (n_files > 100){
        printf("[ERROR] La cantidad de archivos supera el maximo permitido\n");
        return -1;
    }
    
    //Creo una lista de n autores vacia
    Autor lista_autores[n_files];
    
    //Abro de nuevo la carpeta para leer los libros txt
    dir = opendir(home);
    if (dir == NULL){
        printf("[ERROR] El directorio no se pudo abrir\n");
        return 1;
    }
    //Guardo los archivos en la lista de autores
    while ((ent = readdir (dir)) != NULL){
        if ((strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0)){                 
            procesarArchivo(ent->d_name, n_files, lista_autores, &arch, &pos, &pos2, home);
        }
    }
    //Cierro la carpeta
    closedir(dir);

    //Ordeno los autores por el promedio de notas
    int cant_autores = pos;
    if (cant_autores == 0){
        printf("[ERROR] Los archivos ya estan ordenados o la carpeta esta vacia\n");
        return -1;
    }
    
    Autor lista_ordenada[cant_autores];
    ordenarAutores(lista_autores, lista_ordenada, cant_autores);
  
    //Creo las carpetas y muevo los archivos al lugar correspondiente
    for (int i=0; i<cant_autores; i++){
        int pos_ordenada = lista_ordenada[i].slot3;
        for (int j=0; j<lista_autores[pos_ordenada].slot3; j++){
            moverArchivos(home, lista_autores[pos_ordenada].nombre, lista_autores[pos_ordenada].lista_libros[j].titulo, lista_autores[pos_ordenada].cat_mas_publicada, lista_autores[pos_ordenada].ubicaciones[j].ruta, i);
        }
    }
    //Inicio interaccion con usuario 
    int lvl = 0;
    int opcion1 = -1;
    char from[100], current[100];
    strcpy(from, home);
    strcpy(current, home);
    menu_principal(&opcion1, home, from, current, &lvl, lista_autores, cant_autores);
    return 0;
}

void procesarArchivo(char *archivo, int n_files, Autor *lista_autores, int *arch, int *pos, int *pos2, char u[]){
    char ubicacion[100];
    strcpy(ubicacion, u);
    strcat(ubicacion,archivo);
    FILE *fp;
    char linea[100];
    int cont = 0;
    char titulo[50];
    char autor[50];
    char categoria[50];
    char anno[5];
    char n[3];
    int nota, enc, enc2;

    //Intento abrir el archivo
    fp = fopen(ubicacion, "r");
    
    if (fp){
        //Archivo abierto. Guardar los datos
        while (fgets(linea,100,fp)){
            strtok(linea, "\n");
            if (cont%5 == 0){
                strcpy(titulo,linea);       //Titulo del libro
                cont+=1;
            }
            else if (cont%5 == 1){
                strcpy(autor,linea);        //Autor del libro
                cont+=1;
            } 
            else if (cont%5 == 2){
                strcpy(categoria,linea);   //Categoria del libro
                cont+=1;
            }
            else if (cont%5 == 3){
                strcpy(anno,linea);        //Anno de publicacion del libro
                cont+=1;
            }  
            else if (cont%5 == 4){
                //Obtengo la ultima linea del archivo y ahora tengo todas las variables
                strncpy(n,linea,2);       //Nota del libro
                nota = atoi(n);
                cont+=1;
                
                char ubicacion[100];
                strcpy(ubicacion, u);
                strcat(ubicacion,archivo);

                //Si el nombre del autor no esta en la lista_autores, lo creo y lo inicializo
                if ((enc = isAutorin(autor, lista_autores, n_files)) == -1){
                    //Guardo el nombre
                    strcpy(lista_autores[*pos].nombre, autor);
                    //Sumo la nota
                    lista_autores[*pos].notas = nota;
                    lista_autores[*pos].promedio = 0;
                    
                    lista_autores[*pos].slot = 0;
                    lista_autores[*pos].slot3 = 0;
                    //Guardo el libro
                    strcpy(lista_autores[*pos].lista_libros[0].titulo, titulo);
                    strcpy(lista_autores[*pos].lista_libros[0].autor, autor);
                    strcpy(lista_autores[*pos].lista_libros[0].categoria, categoria);
                    strcpy(lista_autores[*pos].lista_libros[0].anno, anno);
                    lista_autores[*pos].lista_libros[0].nota = nota;
                    strcpy(lista_autores[*pos].ubicaciones[0].ruta, ubicacion);

                    lista_autores[*pos].slot2 = 0;
                    //Agrego la categoria      
                    strcpy(lista_autores[*pos].lista_categorias[0].cat, categoria);
                    lista_autores[*pos].lista_cant_por_categoria[0] = 1;

                    //Actualizo donde se guardaran los libros, categorias y ubicaciones de los libros en la proxima coincidencia
                    lista_autores[*pos].slot+=1;
                    lista_autores[*pos].slot2+=1;
                    lista_autores[*pos].slot3+=1;

                    //Calculo constantemente la categoria mas publicada por el autor y el promedio de notas
                    int top_pos = getTopCategory(lista_autores[*pos].lista_cant_por_categoria, lista_autores[*pos].slot2);
                    strcpy(lista_autores[*pos].cat_mas_publicada, lista_autores[*pos].lista_categorias[top_pos].cat);
                    lista_autores[*pos].promedio = getPromedio(lista_autores[*pos].notas, lista_autores[*pos].slot);

                    //Actualizo donde se guardaran los autores en la proxima iteracion
                    *arch+=1;
                    *pos+=1;
                }
                
                //Si el nombre del autor esta en la lista_autores solo agrego el libro
                else {
                    //Sumo la nota
                    lista_autores[enc].notas += nota;
                    
                    //Guardo el libro
                    int slot_libro = lista_autores[enc].slot;
                    int slot_ubicacion = lista_autores[enc].slot3;
                    strcpy(lista_autores[enc].lista_libros[slot_libro].titulo, titulo);
                    strcpy(lista_autores[enc].lista_libros[slot_libro].autor, autor);
                    strcpy(lista_autores[enc].lista_libros[slot_libro].categoria, categoria);
                    strcpy(lista_autores[enc].lista_libros[slot_libro].anno, anno);
                    lista_autores[enc].lista_libros[slot_libro].nota = nota;
                    strcpy(lista_autores[enc].ubicaciones[slot_ubicacion].ruta, ubicacion);
                    
                    //Reviso la categoria
                    //Si la categoria no esta, la agrego a la lista_categorias
                    if ((enc2 = isCategoryin(categoria, lista_autores[enc].lista_categorias, lista_autores[enc].slot2)) == -1){
                        //Agrego la categoria
                        int slot_cat = lista_autores[enc].slot2;      
                        strcpy(lista_autores[enc].lista_categorias[slot_cat].cat, categoria);
                        lista_autores[enc].lista_cant_por_categoria[slot_cat] = 1;
                    }
                    //Si la categoria esta, solo sumo 1 a la cantidad de apariciones de la categoria
                    else{
                        lista_autores[enc].lista_cant_por_categoria[enc2] += 1;
                    }

                    //Actualizo donde se guardaran los libros, categorias y ubicaciones de los libros en la proxima coincidencia
                    lista_autores[enc].slot+=1;
                    lista_autores[enc].slot2+=1;
                    lista_autores[enc].slot3+=1;

                    //Calculo constantemente la categoria mas publicada por el autor y el promedio de notas
                    int top_pos = getTopCategory(lista_autores[enc].lista_cant_por_categoria, lista_autores[enc].slot2);
                    strcpy(lista_autores[enc].cat_mas_publicada, lista_autores[enc].lista_categorias[top_pos].cat);
                    lista_autores[enc].promedio = getPromedio(lista_autores[enc].notas, lista_autores[enc].slot);

                    //Actualizo donde se guardaran los autores en la proxima iteracion
                    *arch+=1;
                }
            } 
        }
        //Cierro el archivo
        fclose(fp);
    }
    //Si el archivo no se pudo abrir, imprimo un error
    else{
        printf("[ERROR] El archivo no se pudo leer\n");
    }
}

void ordenarAutores(Autor lista_origen[], Autor lista_destino[], int size){
    //Para evitar tener que guardar todos los datos (incluido cada libro)
    //guardo la posicion original en lista_autores ordenada por su promedio
    //alojandola en el slot3 
    for(int i=0;i<size;i++){
        lista_destino[i].promedio = lista_origen[i].promedio;
        lista_destino[i].slot3 = i;
    }
	
	for (int i = 0; i < size; i++){                                            
		for (int j = 0; j < size; j++){                                         //Compara los otros valores
			if (lista_destino[j].promedio < lista_destino[i].promedio){
                int tmp = lista_destino[i].promedio;                            //Guarda el que va a ser reemplazado en un lugar temporal
				int tmp_pos = lista_destino[i].slot3;

                lista_destino[i].promedio = lista_destino[j].promedio;          //Es reemplazado por el nuevo valor
				lista_destino[i].slot3 = lista_destino[j].slot3;
                
                lista_destino[j].promedio = tmp;                                //El que fue reemplazado toma la nueva posicion
                lista_destino[j].slot3 = tmp_pos;
            }
		}
	}
}

void moverArchivos(char home[], char nom_autor[], char nom_libro[], char cat[], char ubi[], int i){
    DIR *dir;
    char cmd_mkdir[250] = "mkdir ";
    char cmd_move[250] = "mv ";
    char prefijo[5];
    sprintf(prefijo, "%i", i);
    
    char nueva_ubi[250];
    strcpy(nueva_ubi, home);
    strcat(nueva_ubi, cat);                 // ./libros/top_cat
    dir = opendir(nueva_ubi);
    
    //Si ./libros/top_cat/ no existe, se crea, luego crea ./libros/top_cat/autor y mueve el libro
    if (dir == NULL){
        strcat(cmd_mkdir, nueva_ubi);       // mkdir ./libros/top_cat
        //Creo la carpeta ./libros/top_cat
        system(cmd_mkdir);
        
        strcat(cmd_mkdir, "/");             // mkdir ./libros/top_cat/
        strcat(cmd_mkdir, prefijo);
        strcat(cmd_mkdir, "_");
        strcat(cmd_mkdir, nom_autor);       // mkdir ./libros/top_cat/autor
        //Creo la carpeta ./libros/top_cat/autor
        system(cmd_mkdir);
        strcpy(cmd_mkdir, "mkdir ");
        
        strcat(nueva_ubi, "/");             // ./libros/top_cat/
        strcat(nueva_ubi, prefijo);
        strcat(nueva_ubi, "_");
        strcat(nueva_ubi, nom_autor);       // ./libros/top_cat/autor

        strcat(cmd_move, ubi);              // mv ./libros/libro.txt
        strcat(cmd_move, " ");              // mv ./libros/libro.txt 
        strcat(cmd_move, nueva_ubi);        // mv ./libros/libro.txt ./libros/top_cat/autor
        //Muevo el archivo a su nueva ubicacion
        system(cmd_move);
        strcpy(nueva_ubi, "");
        strcpy(cmd_move, "mv ");
    }
    //Si ./libros/top_cat/ existe, verifico si el autor esta
    else{
        strcat(nueva_ubi, "/");             // ./libros/top_cat/
        strcat(nueva_ubi, prefijo);
        strcat(nueva_ubi, "_");
        strcat(nueva_ubi, nom_autor);       // ./libros/top_cat/autor
        dir = opendir(nueva_ubi);

        //Si ./libros/top_cat/autor no existe, se crea
        if (dir == NULL){
            strcat(cmd_mkdir, nueva_ubi);       // mkdir ./libros/top_cat/autor
            //Creo la carpeta ./libros/top_cat/autor
            system(cmd_mkdir);
            strcpy(cmd_mkdir, "mkdir ");

            strcat(cmd_move, ubi);              // mv ./libros/libro.txt
            strcat(cmd_move, " ");              // mv ./libros/libro.txt 
            strcat(cmd_move, nueva_ubi);        // mv ./libros/libro.txt ./libros/top_cat/autor
            //Muevo el archivo a su nueva ubicacion
            system(cmd_move);
            strcpy(cmd_move, "mv ");
        }
        
        //Si ./libros/top_cat/autor ya existe, solo mueve el libro
        else{
            strcat(cmd_move, ubi);              // mv ./libros/libro.txt
            strcat(cmd_move, " ");              // mv ./libros/libro.txt 
            strcat(cmd_move, nueva_ubi);        // mv ./libros/libro.txt ./libros/top_cat/autor
            //Muevo el archivo a su nueva ubicacion
            system(cmd_move);
            strcpy(cmd_move, "mv ");
        }
    }
    closedir(dir);
}

void menu_principal(int *opcion1, char home[], char *from, char *current, int *lvl, Autor lista[], int cant_autores){
    printf("*****************************************************\n");
    printf("************** Bienvenid@ a la consola **************\n");
    printf("*****************************************************\n");

    printf("Las opciones son las siguientes:\n");
    printf("1: Navegar por los directorios y archivos\n");
    printf("2: Recomendarme un libro de mi categoria favorita\n");
    printf("3: Recomendarme un libro al azar\n");
    printf("0: Salir\n");

    printf("\nIngrese una opcion: ");
    scanf("%i", opcion1);
    printf("-----------------------------------------------------\n");
    
    while (*opcion1!=0){
        //Navegar por las carpetas
        if (*opcion1 == 1){
            printDirArch(home);
            dir_manager(opcion1, home, from, current, lista, cant_autores, lvl);
        }
        //Recomendar libro por categoria
        else if (*opcion1 == 2){
            recomendar_categoria(opcion1, home, lista, cant_autores);
        }
        //Recomendar libro al azar
        else if (*opcion1 == 3){
            recomendar_azar(opcion1, home, lista, cant_autores);
        }
        else if (*opcion1 == 0){
            return;
        }
        //Cuando vuelvo del dir_manager pero en un nuevo directorio
        else if (*opcion1 == -1){
            printf("Las opciones son las siguientes:\n");
            printf("1: Navegar por los directorios y archivos\n");
            printf("2: Recomendarme un libro de mi categoria favorita\n");
            printf("3: Recomendarme un libro al azar\n");
            printf("0: Salir\n");

            printf("\nIngrese una opcion: ");
            scanf("%i", opcion1);
            printf("-----------------------------------------------------\n");
        }
        //Opcion no valida
        else if ((*opcion1!=0) && (*opcion1!=1) && (*opcion1!=2) && (*opcion1!=3)){
            printf("[ERROR] Ingrese una opcion valida\n");
            printf("-----------------------------------------------------\n");
            printf("Las opciones son las siguientes:\n");
            printf("1: Navegar por los directorios y archivos\n");
            printf("2: Recomendarme un libro de mi categoria favorita\n");
            printf("3: Recomendarme un libro al azar\n");
            printf("0: Salir\n");

            printf("\nIngrese una opcion: ");
            scanf("%i", opcion1);
            printf("-----------------------------------------------------\n");
        }
    }
}

void dir_manager(int *opcion1, char home[], char *from, char *current, Autor lista[], int cant_autores, int *lvl){
    int opcion2 = -1;
    char cmd_cd[250] = "cd ";
    char cmd_cat[250] = "cat ";
    int enc;     

    while (opcion2!=9){
        printf("\nLas opciones son las siguientes:\n");
        printf("1: Entrar a un directorio\n");
        printf("2: Salir del directorio (Subir de nivel)\n");
        printf("3: Abrir archivo\n");
        printf("9: Menu principal\n"); 

        printf("\nIngrese una opcion: ");
        scanf("%i", &opcion2);
        printf("-----------------------------------------------------\n");

        //Proceso de directorios
        //Aca hice enfasis en que el nivel de organizacion de las carpetas establece 3 niveles:
        //Nivel 0: Directorio raiz ./libros/: El usuario puede ver todas las categorias
        //Nivel 1: Directorio ./libros/categoriaX: El usuario puede ver los autores que han publicado en esa categoria
        //Nivel 2: Directorio ./libros/categoriaX/autorY: El usuario puede ver los libros del autor Y
        //Por lo que no hay mas carpetas arriba del nivel 0 ni abajo del nivel 2

        //Entrar a un directorio
        if (opcion2 == 1){
            //Si el usuario quiere entrar a un directorio estando en el nivel 2
            if (*lvl == 2){
                printf("[ERROR] Estas en el ultimo nivel del directorio\n");
                printf("-----------------------------------------------------\n");
                //Imprimo los directorios/archivos que hay en el actual directorio
                printDirArch(current);
            }
            else{
                char directorio[50];
                printf("Escriba nombre del directorio: ");
                scanf("%s", directorio);
                printf("-----------------------------------------------------\n");

                //Si el usuario quiere entrar a un directorio que no existe imprime error
                if ((enc = nameExists(directorio, current)) == -1){
                    printf("[ERROR] El directorio no existe no existe\n");
                    printf("-----------------------------------------------------\n");
                    //Imprimo los directorios/archivos que hay en el actual directorio
                    printDirArch(current);
                }
                //Si ingresa un directorio correcto actualizo los caminos from y current
                else{
                    strcpy(from, current);
                    strcat(current, directorio);
                    strcat(current, "/");
                    
                    //Bajo un nivel
                    *lvl+=1;

                    //Entro al directorio con una llamada al sistema
                    strcpy(cmd_cd, "cd ");
                    strcat(cmd_cd, current);
                    system(cmd_cd);
                    
                    //Imprimo los directorios/archivos que hay en el nuevo directorio
                    printDirArch(current);

                    //Dejo opcion1 = 1 para que al regresar al menu principal vuelva a entrar a dir_manager
                    *opcion1 = 1;
                }
            }
        }
        //Salir del directorio
        else if (opcion2 == 2){
            //Si el usuario quiere salir del directorio estando en el nivel 0
            if (*lvl == 0){
                printf("[ERROR] Estas en el directorio raiz\n");
                printf("-----------------------------------------------------\n");

                //Imprimo los directorios/archivos que hay en el actual directorio
                printDirArch(current);
            }
            else{
                //Si esta en el nivel 1 lo regreso al nivel 0
                if (*lvl == 1){
                    strcpy(current, home);
                    
                    //Subo un nivel
                    *lvl-=1;

                    //Entro al directorio con una llamada al sistema
                    strcpy(cmd_cd, "cd ");
                    strcat(cmd_cd, from);
                    system(cmd_cd);
                    
                    //Imprimo los directorios/archivos que hay en el nuevo directorio
                    printDirArch(current);

                }
                //Si sale del nivel 2 lo regreso a la carpeta de donde vino
                else{
                    strcpy(current, from);
                    
                    //Subo un nivel
                    *lvl-=1;

                    //Entro al directorio con una llamada al sistema
                    strcpy(cmd_cd, "cd ");
                    strcat(cmd_cd, from);
                    system(cmd_cd);

                    //Imprimo los directorios/archivos que hay en el nuevo directorio
                    printDirArch(current);

                    //Dejo opcion1 = 1 para que al regresar al menu principal vuelva a entrar a dir_manager
                    *opcion1 = 1;
                }
            }
        }
        //Abrir archivo
        else if (opcion2 == 3){
            //Si no estoy en el nivel 2 no habran archivos que abrir
            if (*lvl != 2){
                printf("[ERROR] No hay archivos que abrir\n");
                printf("-----------------------------------------------------\n");

                //Imprimo los directorios/archivos que hay en el actual directorio
                printDirArch(current);  
            }

            //En caso de estar en el nivel 2, se permite la lectura de archivos
            else{
                char archivo[50];
                printf("Escriba nombre del archivo: ");
                scanf("%s", archivo);
                printf("-----------------------------------------------------\n");
                
                //Busco si el archivo existe, sino imprime un error
                if ((enc = nameExists(archivo, current)) == -1){
                    printf("[ERROR] El archivo no existe\n");
                    printf("-----------------------------------------------------\n");
                    //Imprimo los directorios/archivos que hay en el actual directorio
                    printDirArch(current);
                }
                
                //Abro el archivo con una llamada al sistema
                else{
                    printf("Mostrando archivo: %s\n", archivo);
                    strcat(cmd_cat, current);
                    strcat(cmd_cat, archivo);
                    system(cmd_cat);
                    strcpy(cmd_cat, "cat ");
                    printf("-----------------------------------------------------\n");

                    //Imprimo los directorios/archivos que hay en el actual directorio
                    printDirArch(current);
                }
            }

        }
        else if (opcion2 == 9){
            strcpy(current, home);
            strcpy(from, home);
            strcpy(cmd_cd, "cd ");
            *lvl = 0;
            *opcion1 = -1;
        }

        //Opcion no valida
        else if ((opcion2!=9) && (opcion2!=1) && (opcion2!=2) && (opcion2!=3)){
            printf("[ERROR] Ingrese una opcion valida\n");
            printf("-----------------------------------------------------\n");
        }
    }
    return;
}

void recomendar_categoria(int *opcion1, char home[], Autor lista[], int cant_autores){
    char categoria_favorita[50];
    printf("Escriba el nombre de la categoria deseada: ");
    scanf("%s", categoria_favorita);
    printf("-----------------------------------------------------\n");

    int i = rand() % cant_autores;
    int cant_libros = lista[i].slot;
    int j = rand() % cant_libros;
    char candidato[50];
    strcpy(candidato, lista[i].lista_libros[j].titulo);

    //Verifica que la categoria deseada exista
    DIR *dir;                               //Manejo de directorios
    struct dirent *ent;                     //Manejo de archivos

    dir = opendir(home);
    while ((ent = readdir (dir)) != NULL){
        if ((strcmp(ent->d_name, categoria_favorita) == 0)){
             //Verifica si el candidato es de la categoria deseada
            while (strcmp(lista[i].lista_libros[j].categoria, categoria_favorita) != 0){
                i = rand() % cant_autores;
                j = rand() % cant_libros;
            }
            printf("El libro recomendado es: %s, del autor %s y del genero %s (guardado en %s)\n", candidato, lista[i].nombre, lista[i].lista_libros[j].categoria, lista[i].cat_mas_publicada);
            printf("-----------------------------------------------------\n");
            *opcion1 = -1;
            return;
        }
    }
    //Si no existe, preguntar por otra
    printf("Lo siento, no tenemos libros de la categoria %s\n", categoria_favorita);
    recomendar_categoria(opcion1, home, lista, cant_autores);
}

void recomendar_azar(int *opcion1, char home[], Autor lista[], int cant_autores){
    int i = rand() % cant_autores;
    int cant_libros = lista[i].slot;
    int j = rand() % cant_libros;
    char candidato[50];
    strcpy(candidato, lista[i].lista_libros[j].titulo);
    printf("El libro recomendado es: %s, del autor %s y del genero %s (guardado en %s)\n", candidato, lista[i].nombre, lista[i].lista_libros[j].categoria, lista[i].cat_mas_publicada);
    printf("-----------------------------------------------------\n");
    *opcion1 = -1;
}

void printDirArch(char current[]){
    DIR *dir;                               //Manejo de directorios
    struct dirent *ent;                     //Manejo de archivos

    //Intento abrir la carpeta
    dir = opendir(current);
    
    if (dir == NULL){
        printf("[ERROR] El directorio no se pudo abrir\n");
        return;
    }
    //Recorro la carpeta para mostrar los archivos y carpetas
    printf("Las carpetas y archivos del directorio %s son:\n", current);
    while ((ent = readdir (dir)) != NULL){
        if ((strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0)){
            printf("%s\n", ent->d_name);
        }
    } 
    closedir(dir); 
}
