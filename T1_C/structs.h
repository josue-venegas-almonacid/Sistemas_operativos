//Definiciones structs

typedef struct Libro{
    char titulo[50];                               //Titulo del libro
    char autor[50];                                //Autor
    char categoria[50];                            //Categoria
    char anno[5];                                  //Anno de publicacion
    int nota;                                      //Nota
}Libro;

typedef struct Categoria{
    char cat[50];
}Categoria;

typedef struct Ubicacion{
    char ruta[100];
}Ubicacion;

typedef struct Autor{
    char nombre[50];                                //Nombre del autor
    char cat_mas_publicada[50];                     //Categoria mas publicada por el autor
    int notas;                                      //Sumatoria de notas de sus libros
    int promedio;                                   //Promedio de notas del autor

    Libro lista_libros[100];                        //Lista vacía de elementos tipo Libro
    int slot;                                       //Posicion del ultimo libro guardado en la lista

    Categoria lista_categorias[100];                //Lista vacía de categorias
    int lista_cant_por_categoria[100];              //Lista vacía de enteros que representan las apariciones de una categoria
    int slot2;                                      //Posicion de la ultima categoria guardada en la lista

    Ubicacion ubicaciones[100];                     //Lista de ubicaciones de los archivos txt
    int slot3;                                      //Posicion de la ultima ubicacion guardada en la lista

}Autor;

//Definiciones metodos de los structs

//isAutorin: verifica si un autor esta en la lista de autores y retorna su posicion. En caso contrario, retorna -1
int isAutorin(char nom[], Autor lista[], int size){
    for (int i=0; i<size; i++){
        if (strcmp(lista[i].nombre, nom) == 0){
            return i;
        }
    }
    return -1;
}

//isCategoryin: Verificar si la categoria esta en la lista de categorias y retorna su posicion. En caso contrario, retorna -1
int isCategoryin(char nom[], Categoria lista[], int size){
    for (int i=0; i<size; i++){
        if (strcmp(lista[i].cat, nom) == 0){
            return i;
        }
    }
    return -1;
}

//getTopCategory: Retorna la posicion de la categoria con mas publicaciones de un autor
int getTopCategory(int n_categorias[], int size){
    int max = -1;
    int top;
    for (int i=0; i<size; i++){
        if (n_categorias[i] > max){
            max = n_categorias[i];
            top = i;
        }
    }
    return top;
}

//redondear: Retorna el entero aproximado
int redondear(double x)
{
    if (x < 0.0)
        return (int)(x - 0.5);
    else
        return (int)(x + 0.5);
}

//getPromedio: Retorna el promedio entre el sumatorio de notas y la cantidad de libros
int getPromedio(int notas, int cant){
    double n = (double) notas;
    double c = (double) cant;
    double prom = n/c;
    return redondear(prom);
}

//nameExists: Verifica si el nombre del directorio o archivo que se quiere acceder existe. En caso negativo, retorna -1
int nameExists(char name[], char current[]){
    DIR *dir;
    struct dirent *ent;
    
    dir = opendir(current);
    if (dir == NULL){
        printf("El directorio %s no existe\n", current);
        return -1;
    }
    else{
        while ((ent = readdir (dir)) != NULL){
            if ((strcmp(ent->d_name, name)==0)){
                return 0;
            }
        }
        return -1;
    }
}