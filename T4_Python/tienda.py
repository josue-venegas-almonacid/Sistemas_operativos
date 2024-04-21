import threading, time, logging

capacidad_disponible = threading.Semaphore(30)
candado = threading.Semaphore(1)
baño = threading.Semaphore(1)

def entrarTienda(id):
    candado.acquire()
    archivo_clientes.write(f"[{time.ctime()}] Cliente {id} está en la fila\n")
    candado.release()
    
    capacidad_disponible.acquire()  #El cliente intenta entrar a la tienda

    candado.acquire()
    archivo_clientes.write(f"[{time.ctime()}] Cliente {id} entró a tienda\n")
    candado.release()
  
    time.sleep(4)   #Buscando productos

    candado.acquire()
    n_caja = buscarCaja()   #Buscando caja con menos clientes en la fila
    lista_cajas[n_caja].acquire()
    candado.release()

    candado.acquire()
    if n_caja not in clientes_a_atender:
        clientes_a_atender[n_caja] = []
    clientes_a_atender[n_caja].append(id)   #Agrego al cliente a la lista de clientes a ser atendidos
    candado.release()

    candado.acquire()
    archivo_clientes.write(f"[{time.ctime()}] Cliente {id} entró a la fila de la caja {n_caja}\n")
    candado.release()
    
    meson_pago_disponibles[n_caja].release() #Desde comenzarTurno, el cajero libera el mesón de pago para que el cliente sea atendido
    meson_pago_solicitados[n_caja].acquire()
    lista_cajas[n_caja].release()   #El cliente es atendido

    capacidad_disponible.release()  #El cliente sale de la tienda
   
    candado.acquire()
    archivo_clientes.write(f"[{time.ctime()}] Cliente {id} saliendo de tienda\n")
    candado.release()

 
def buscarCaja():
    cupos_disponibles = 0
    n_caja = -1
    for index, caja in enumerate(lista_cajas):
        if (caja._value > cupos_disponibles):
            cupos_disponibles = caja._value
            n_caja = index
    return n_caja


def comenzarTurno(id):
    while (lista_cajas[id]._value != 0):
        meson_pago_disponibles[id].acquire()    #Mientras haya gente esperando ser atendida, atender
        if (restantes_para_ir_baño[id]._value == 0):  #Si el cajero ya atendió 5 clientes
            if (baño._value != 0):  #Si el baño está desocupado
                baño.acquire()  #El cajero entra al baño

                candado.acquire()
                archivo_cajeros.write(f"[{time.ctime()}] Cajero {id} entró al baño\n")
                candado.release()
                time.sleep(4)   #Ocupa el baño
                
                baño.release() #Sale del baño
                
                restantes_para_ir_baño[id] = threading.Semaphore(5) #Reinicio su contador
                
                #Vuelvo a atender
                candado.acquire()
                archivo_cajeros.write(f"[{time.ctime()}] Cajero {id} atendió al cliente {clientes_a_atender[id][0]}\n")
                candado.release()
                
                time.sleep(5) #Atendiendo cliente
                restantes_para_ir_baño[id].acquire()
                
                candado.acquire()
                clientes_a_atender[id].remove(clientes_a_atender[id][0])    #Quito al cliente atendido
                candado.release()

                meson_pago_solicitados[id].release()
                

        else:   #El cajero sigue atendiendo clientes
            candado.acquire()
            archivo_cajeros.write(f"[{time.ctime()}] Cajero {id} atendió al cliente {clientes_a_atender[id][0]}\n")
            candado.release()
            
            time.sleep(5) #Atendiendo cliente
            restantes_para_ir_baño[id].acquire()
            
            candado.acquire()
            clientes_a_atender[id].remove(clientes_a_atender[id][0])    #Quito al cliente atendido
            candado.release()
            
            meson_pago_solicitados[id].release()            
                
            
            


#Programa principal
clientes_totales = int(input("¿Cuántos clientes recibirá el día de hoy?: "))
while (clientes_totales <= 50):
    print("Error, ingrese un número superior a 50")
    clientes_totales = int(input("¿Cuántos clientes recibirá el día de hoy?: "))

print(f"[{time.ctime()}] Tienda abierta\n")
lista_clientes = []
lista_cajas = []
lista_cajeros = []
clientes_a_atender = {}
meson_pago_disponibles = []
meson_pago_solicitados = []
restantes_para_ir_baño = []

archivo_clientes = open("clientes.txt", 'w')
archivo_cajeros = open("cajeros.txt", 'w')


#Agrego 5 cajas de atención, con 6 cupos cada una
for i in range(5):
    caja = threading.Semaphore(6)
    lista_cajas.append(caja)

#Para cada caja, creo un mesón de pago donde el cajero atiende
for i in range(5):
    atendiendo = threading.Semaphore(0)
    meson_pago_disponibles.append(atendiendo)

#Para cada caja, creo un mesón de pago donde el cliente pide ser atendido
for i in range(5):
    ser_atendido = threading.Semaphore(0)
    meson_pago_solicitados.append(ser_atendido)

#Para cada caja, creo un contador de cuántos clientes faltan para que el cajero vaya al baño
for i in range(5):
    restantes = threading.Semaphore(5)
    restantes_para_ir_baño.append(restantes)

#Creo 5 cajeros que atiendan las cajas
for index in range(5):
    cajero = threading.Thread(target=comenzarTurno, args=(index,), daemon = True)
    lista_cajeros.append(cajero)
    cajero.start()

#Creo clientes_totales clientes
for index in range(clientes_totales):
    clienteEntrante = threading.Thread(target=entrarTienda, args=(index,))
    lista_clientes.append(clienteEntrante)
    clienteEntrante.start()
    
#Espero a que todos los clientes salgan de la tienda y la cierro
for hebra in lista_clientes:
    hebra.join()


print(f"[{time.ctime()}] Tienda cerrada\n")
archivo_clientes.close()
archivo_cajeros.close()
