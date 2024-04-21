import java.util.Scanner;

public class ConThreads {
    public static void main(String[] args) {

        //Pregunto al usuario el rango de números a considerar
        System.out.println("[CON 2 THREADS] Buscar el número con la mayor cantidad de divisores entre 1 y: ");
        Scanner scanner = new Scanner(System.in);
        String respuesta = scanner.nextLine();

        //Verifico que la entrada sea válida. En caso contrario, muestro un error y pregunto de nuevo
        while ((!isNumeric(respuesta)) || (Integer.parseInt(respuesta)<=0)){
            System.out.println("Entrada no válida, intente nuevamente");
            System.out.println("[CON 2 THREADS] Buscar el número con la mayor cantidad de divisores entre 1 y: ");
            respuesta = scanner.nextLine();
        }

        //Inicia el programa
        int n = Integer.parseInt(respuesta);
        long tiempo_inicial = System.nanoTime();

        //Creo dos hebras. La primera tendrá que buscar el mayor número dentro del 60% de los datos
        Hebra hebra1 = new Hebra();
        hebra1.setEnd(60*n/100);
        hebra1.start();

        //La segunda tendrá que buscar dentro del restante 40% y compararlo con el número con más divisores del 60%
        Hebra hebra2 = new Hebra();
        hebra2.setMaximo(hebra1.getMaximo());
        hebra2.setNumero_Mas_Divisores(hebra1.getNumero_Mas_Divisores());
        hebra2.setStart((60*n/100)+1);
        hebra2.setEnd((n));
        hebra2.start();

        //Espero a que ambas hebras terminen de ejecutarse
        try {
            hebra1.join();
            hebra2.join();
        }

        //En caso de error, muestro un mensaje
        catch (InterruptedException e) {
            e.printStackTrace();
            return;
        }

        //Termina el programa
        long tiempo_final = System.nanoTime();
        double tiempo_total = (tiempo_final-tiempo_inicial) / Math.pow(10,9);

        System.out.println("Número: "+hebra2.getNumero_Mas_Divisores()+" (con "+hebra2.getMaximo()+" divisores)");
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