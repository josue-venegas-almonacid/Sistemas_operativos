class Hebra extends Thread {
    //Atributos para calcular el número con mayor cantidad de divisores
    private int start = 1;
    private int end = 0;
    private int n_mas_divisores = 0;
    private int maximo = 0;

    //Setters y Getters
    public void setStart(int valor){
        this.start = valor;
    }
    public void setEnd(int valor){
        this.end = valor;
    }
    public void setNumero_Mas_Divisores(int valor){
        this.n_mas_divisores = valor;
    }
    public void setMaximo(int valor){
        this.maximo = valor;
    }
    public int getNumero_Mas_Divisores(){
        return this.n_mas_divisores;
    }
    public int getMaximo(){
        return this.maximo;
    }

    /*Nombre: run()
     *Función: Verifica la cantidad de divisores del intervalo [start, end] y guarda el mayor
     *Inputs: Ninguno
     *Returns: Ninguno*/
    public void run() {
        try {
            //Recorro los i números desde start hasta end
            for (int i = this.start; i <= this.end; i++) {
                //Cada número empieza con 0 divisores
                int cantidad = 0;
                //Recorro los j números desde 1 hasta i
                for (int j = 1; j <= i; j++) {
                    //Si el número es divisor, aumento el contador
                    if (i % j == 0) {
                        cantidad += 1;
                    }
                }

                //Si el número i tiene más divisores que el máximo, lo actualizo
                if (cantidad > this.maximo) {
                    this.maximo = cantidad;
                    this.n_mas_divisores = i;
                }
            }
        }

        //En caso de error, muestro un mensaje
        catch (Exception e) {
            System.out.println ("Error desconocido");
        }
    }
}
