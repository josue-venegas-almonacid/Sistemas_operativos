import java.util.Scanner;

public class SinThreads {
    public static void main(String[] args) {

        //Pregunto al usuario el rango de números a considerar
        System.out.println("[SIN THREADS] Buscar el número con la mayor cantidad de divisores entre 1 y: ");
        Scanner scanner = new Scanner(System.in);
        String respuesta = scanner.nextLine();

        //Verifico que la entrada sea válida. En caso contrario, muestro un error y pregunto de nuevo
        while ((!isNumeric(respuesta)) || (Integer.parseInt(respuesta)<=0)){
            System.out.println("Entrada no válida, intente nuevamente");
            System.out.println("[SIN THREADS] Buscar el número con la mayor cantidad de divisores entre 1 y: ");
            respuesta = scanner.nextLine();
        }

        //Atributos para calcular el número con mayor cantidad de divisores
        int n = Integer.parseInt(respuesta);
        int n_mas_divisores = 0;
        int maximo = 0;

        //Inicia el programa
        long tiempo_inicial = System.nanoTime();

        //Recorro los i números desde 1 hasta n
        for (int i = 1; i <= n; i++) {
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
            if (cantidad > maximo) {
                maximo = cantidad;
                n_mas_divisores = i;
            }
        }

        //Termina el programa
        long tiempo_final = System.nanoTime();
        double tiempo_total = (tiempo_final-tiempo_inicial) / Math.pow(10,9);

        System.out.println("Número: "+n_mas_divisores+" (con "+maximo+" divisores)");
        System.out.println("Tarea realizada en "+tiempo_total+" segundos");
    }

    /*Nombre: isNumeric()
     *Función: Verifica si la entrada es un número
     *Inputs: String entrada = input del usuario
     *Returns: Boolean true = en caso de que la entrada sea un número
     *         Boolean false = en caso contrario*/
    public static boolean isNumeric(String entrada) {
        if (entrada == null) {
            return false;
        }
        try {
            double d = Double.parseDouble(entrada);
        }
        catch (NumberFormatException nfe) {
            return false;
        }
        return true;
    }
}

