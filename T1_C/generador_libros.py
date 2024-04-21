import random
valido = 0
while(valido == 0):
    cantidad = input("Cantidad de libros a generar: ")
    
    lista_autores = ["Raul_Guantecillo","Juanin_Juan_Harry","Tenison",
                     "Tulio_Trivino","Juan_Carlos_Bodoque","Patana",
                     "Mario_Hugo","Chico_Terry","Carlitos_Lechuga"]

    
    lista_categorias = ["Deportes","Accion","Ciencia","Tecnologia",
                        "Drama", "Geologia", "Turismo", "Romance"]

    for i in range (1,int(cantidad)+1):
        titulo = "libro"+str(i)
        n_autor = random.randrange(len(lista_autores))
        n_categoria = random.randrange(len(lista_categorias))
        anno = random.randrange(1800,2021)
        calif = random.randrange(11)

        if calif<10:
            calificacion = "0"+str(calif)

        elif calif==10:
            calificacion = str(calif)

        
        try:
            archivo = open("./libros/"+titulo+".txt", "w")
            archivo.write(titulo+"\n")
            archivo.write(lista_autores[n_autor]+"\n")
            archivo.write(lista_categorias[n_categoria]+"\n")
            archivo.write(str(anno)+"\n")
            archivo.write(calificacion+"/10"+"\n")

            archivo.close()

        except IOError as e:
            print("[ERROR] La carpeta libros no existe")
            valido = -1
            break
        
    if (valido == 0):
        print(i, "libros generados")

    
