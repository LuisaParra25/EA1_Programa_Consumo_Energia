/*
 * =====================================================================
 * PROGRAMA: MiConsumo - cuanto me cuesta la energia de mi casa
 * CURSO:    Programacion Orientada a Objetos - IU Digital - 2026-2
 * =====================================================================
 *
 * ENUNCIADO:
 * Con El Nino 2026 la energia subio. Este programa calcula cuanto
 * consume y cuanto cuesta cada equipo de una casa durante una semana:
 *   1. Clase Equipo: nombre, potencia y un VECTOR con las horas de uso
 *      de cada dia de la semana.
 *   2. Clase Hogar: tiene un VECTOR DE OBJETOS Equipo y una MATRIZ con
 *      el consumo en kWh de cada equipo (columna) en cada dia (fila).
 *   3. Reporte: consumo y costo por dia, por equipo, dia mas caro,
 *      equipo que mas gasta y proyeccion mensual.
 *
 * NOTA: los mensajes no llevan tildes ni enie porque la consola de
 * Windows puede mostrarlos mal.
 * =====================================================================
 */

// ---------------------------------------------------------------------
// LIBRERIAS (#include): codigo ya escrito que reutilizamos
// ---------------------------------------------------------------------
#include <iostream>   // cout (escribir en pantalla) y cin (leer del teclado)
#include <string>     // el tipo string, para guardar textos
#include <iomanip>    // setw (ancho de columna) y setprecision (decimales):
                      // sirven para que la tabla quede alineada

// Sin esta linea habria que escribir std::cout, std::string, etc.
using namespace std;

// ---------------------------------------------------------------------
// CONSTANTES: tamanos de los vectores y de la matriz
// const = su valor NO puede cambiar mientras el programa corre.
// Si se cambia aqui, cambia en todo el programa.
// ---------------------------------------------------------------------
const int DIAS = 7;          // filas de la matriz: lunes a domingo
const int MAX_EQUIPOS = 4;   // columnas de la matriz: equipos de la casa

// VECTOR de textos con el nombre de cada dia.
// Sus posiciones (indices) van de 0 a 6: NOMBRE_DIA[0] es "Lun".
const string NOMBRE_DIA[DIAS] = {"Lun", "Mar", "Mie", "Jue", "Vie", "Sab", "Dom"};


// =====================================================================
// CLASE Equipo: un aparato electrico de la casa
// ---------------------------------------------------------------------
// Una CLASE es un MOLDE: define que DATOS (atributos) y que ACCIONES
// (metodos) tendra cada objeto que se cree a partir de ella.
// =====================================================================
class Equipo {

// private: estos atributos solo se pueden usar DENTRO de la clase.
// Desde el main no se pueden tocar directamente (ENCAPSULAMIENTO).
private:
    string nombre;        // Ej.: "Ducha electrica"
    float potenciaW;      // Potencia en vatios (W), tomada de la etiqueta
    float horas[DIAS];    // VECTOR de 7 casillas: horas de uso de cada dia
                          // horas[0] = lunes ... horas[6] = domingo

// public: lo que SI se puede usar desde afuera (desde el main o desde
// otra clase). Son las "puertas" del objeto.
public:

    // -----------------------------------------------------------------
    // CONSTRUCTOR POR DEFECTO (sin parametros)
    // Se ejecuta al crear un Equipo sin datos. Lo EXIGE el vector de
    // objetos de la clase Hogar, que crea 4 equipos "vacios" al inicio.
    // -----------------------------------------------------------------
    Equipo() {
        nombre = "";                    // sin nombre
        potenciaW = 0;                  // sin potencia
        for (int d = 0; d < DIAS; d++) {
            horas[d] = 0;               // cada casilla del vector en cero
        }
    }

    // -----------------------------------------------------------------
    // CONSTRUCTOR CON PARAMETROS
    // n = nombre, p = potencia en W, h = vector con las 7 horas.
    // const float h[DIAS]: recibe un vector y promete NO modificarlo.
    // -----------------------------------------------------------------
    Equipo(string n, float p, const float h[DIAS]) {
        nombre = n;                     // guarda el nombre recibido
        potenciaW = p;                  // guarda la potencia recibida
        for (int d = 0; d < DIAS; d++) {
            horas[d] = h[d];            // copia el vector casilla por casilla
        }
    }

    // GETTER: devuelve el nombre (el atributo es privado, por eso
    // se necesita un metodo publico para leerlo desde afuera)
    string getNombre() {
        return nombre;
    }

    // Muestra el nombre, la potencia y las 7 horas del vector
    void mostrar() {
        cout << nombre << " (" << potenciaW << " W) - horas:";
        for (int d = 0; d < DIAS; d++) {
            cout << " " << horas[d];    // escribe cada casilla del vector
        }
        cout << endl;                   // salto de linea al final
    }
    // -----------------------------------------------------------------
    // Energia de UN dia en kWh = potencia (W) x horas / 1000
    // d es el INDICE del dia: 0 = lunes, 6 = domingo.
    // Se divide entre 1000 porque 1 kW = 1000 W.
    // -----------------------------------------------------------------
    float energiaDia(int d) {
        return potenciaW * horas[d] / 1000;
    }

    // -----------------------------------------------------------------
    // Energia de la SEMANA: recorre el vector y ACUMULA
    // -----------------------------------------------------------------
    float energiaSemana() {
        float total = 0;                // ACUMULADOR: empieza en cero
        for (int d = 0; d < DIAS; d++) {
            total += energiaDia(d);     // suma la energia de cada dia
        }
        return total;                   // entrega el total de la semana
    }

    float horasSemana() {
        float total = 0;
        for (int d = 0; d < DIAS; d++) {
            total += horas[d];
        }
        return total;
    }

    int diaDeMayorUso() {
        int mayor = 0;
        for (int d = 1; d < DIAS; d++) {
            if (horas[d] > horas[mayor]) {
                mayor = d;
            }
        }
        return mayor;
    }

    void reducirHoras(float porcentaje) {
        if (porcentaje < 1 || porcentaje > 100) {
            return;
        }
        for (int d = 0; d < DIAS; d++) {
            horas[d] *= (100 - porcentaje) / 100;
        }
    }

};  // <- la clase termina en llave y PUNTO Y COMA


// =====================================================================
// CLASE Hogar: la casa, con sus equipos y la matriz de consumo
// ---------------------------------------------------------------------
// COMPOSICION: un Hogar TIENE varios objetos Equipo adentro.
// =====================================================================
class Hogar {
private:
    string nombre;                         // Ej.: "Casa de ejemplo"
    float tarifa;                          // COP por kWh (de la factura)
    Equipo equipos[MAX_EQUIPOS];           // VECTOR DE OBJETOS: 4 casillas,
                                           // cada una es un Equipo completo
    int cantidad;                          // cuantas casillas estan ocupadas
    float consumo[DIAS][MAX_EQUIPOS];      // MATRIZ 7 x 4 con kWh:
                                           // consumo[dia][equipo]

public:
    // CONSTRUCTOR: recibe el nombre de la casa y la tarifa
    Hogar(string n, float t) {
        nombre = n;
        tarifa = t;
        cantidad = 0;                      // la casa empieza sin equipos
    }

    // -----------------------------------------------------------------
    // Cambia el nombre y la tarifa (los digita el usuario en el menu)
    // -----------------------------------------------------------------
    void setDatos(string n, float t) {
        nombre = n;
        tarifa = t;
    }

    // GETTER: cuantos equipos hay guardados en el vector
    int getCantidad() {
        return cantidad;
    }

    // Deja el vector de equipos vacio (para volver a empezar)
    void vaciar() {
        cantidad = 0;
    }

    // Recorre el VECTOR DE OBJETOS y le pide a cada equipo mostrarse
    void mostrarEquipos() {
        cout << defaultfloat << setprecision(6);  // formato normal: quita el que dejan
                                           // mostrarMatriz() y reporte()
        if (cantidad == 0) {
            cout << "Todavia no hay equipos" << endl;
        }
        for (int e = 0; e < cantidad; e++) {
            cout << e << ". ";             // numero de la casilla del vector
            equipos[e].mostrar();          // cada objeto se muestra solo
        }
    }

    // -----------------------------------------------------------------
    // Guarda un equipo en la siguiente casilla libre del vector
    // -----------------------------------------------------------------
    void agregarEquipo(Equipo e) {
        if (cantidad < MAX_EQUIPOS) {      // si todavia hay espacio...
            equipos[cantidad] = e;         // ...lo guarda en la casilla libre
            cantidad++;                    // ...y cuenta un equipo mas
        } else {
            cout << "No caben mas equipos" << endl;  // vector lleno
        }
    }

    // -----------------------------------------------------------------
    // Llena la matriz con CICLOS ANIDADOS:
    // el for de afuera recorre las FILAS (dias) y el de adentro las
    // COLUMNAS (equipos). Cada casilla pide la energia a su objeto Equipo.
    // -----------------------------------------------------------------
    void calcularMatriz() {
        for (int d = 0; d < DIAS; d++) {            // cada fila (dia)
            for (int e = 0; e < cantidad; e++) {    // cada columna (equipo)
                consumo[d][e] = equipos[e].energiaDia(d);
            }
        }
    }

    // -----------------------------------------------------------------
    // Suma de una FILA: se fija el dia d y se recorren las columnas.
    // Resultado: todo lo que consumio la casa ese dia.
    // -----------------------------------------------------------------
    float totalDia(int d) {
        float total = 0;
        for (int e = 0; e < cantidad; e++) {
            total += consumo[d][e];
        }
        return total;
    }

    // -----------------------------------------------------------------
    // Suma de una COLUMNA: se fija el equipo e y se recorren las filas.
    // Resultado: lo que consumio ese equipo en la semana.
    // -----------------------------------------------------------------
    float totalEquipo(int e) {
        float total = 0;
        for (int d = 0; d < DIAS; d++) {
            total += consumo[d][e];
        }
        return total;
    }

    // Suma de TODA la matriz: suma los totales de los 7 dias
    float totalSemana() {
        float total = 0;
        for (int d = 0; d < DIAS; d++) {
            total += totalDia(d);
        }
        return total;
    }

    // -----------------------------------------------------------------
    // Muestra la matriz con el total de cada fila y de cada columna.
    // fixed + setprecision(2): siempre 2 decimales.
    // setw(n): reserva n espacios para el siguiente dato (alinea).
    // -----------------------------------------------------------------
    void mostrarMatriz() {
        cout << fixed << setprecision(2);
        cout << endl << "Consumo en kWh (filas = dias, columnas = equipos)" << endl;

        // Encabezado: "Dia", el nombre de cada equipo y "Total"
        cout << setw(5) << "Dia";
        for (int e = 0; e < cantidad; e++) {
            cout << setw(18) << equipos[e].getNombre();
        }
        cout << setw(10) << "Total" << endl;

        // Una linea por cada fila (dia) de la matriz
        for (int d = 0; d < DIAS; d++) {
            cout << setw(5) << NOMBRE_DIA[d];          // nombre del dia
            for (int e = 0; e < cantidad; e++) {
                cout << setw(18) << consumo[d][e];     // cada casilla
            }
            cout << setw(10) << totalDia(d) << endl;   // suma de la fila
        }

        // Ultima linea: la suma de cada columna (equipo)
        cout << setw(5) << "Total";
        for (int e = 0; e < cantidad; e++) {
            cout << setw(18) << totalEquipo(e);
        }
        cout << setw(10) << totalSemana() << endl;     // total de la matriz
    }

    // -----------------------------------------------------------------
    // Busca la fila con la MAYOR suma.
    // Supone que el mayor es el dia 0 y lo compara con los demas.
    // Devuelve el INDICE del dia, no el valor.
    // -----------------------------------------------------------------
    int diaMasCaro() {
        int mayor = 0;                             // supone: lunes es el mayor
        for (int d = 1; d < DIAS; d++) {           // compara desde el martes
            if (totalDia(d) > totalDia(mayor)) {
                mayor = d;                         // encontro uno mayor
            }
        }
        return mayor;
    }

    // Busca la columna con la MAYOR suma (mismo metodo que diaMasCaro)
    int equipoQueMasGasta() {
        int mayor = 0;
        for (int e = 1; e < cantidad; e++) {
            if (totalEquipo(e) > totalEquipo(mayor)) {
                mayor = e;
            }
        }
        return mayor;
    }

    int diaMasBarato() {
        int menor = 0;
        for (int d = 1; d < DIAS; d++) {
            if (totalDia(d) < totalDia(menor)) {
                menor = d;
            }
        }
        return menor;
    }

    float porcentajeEquipo(int e) {
        float total = totalSemana();
        if (e < 0 || e >= cantidad || total <= 0) {
            return 0;
        }
        return totalEquipo(e) * 100 / total;
    }

    float promedioDiario() {
        return totalSemana() / DIAS;
    }

    void simularAhorro(int e, float porcentaje) {
        if (e < 0 || e >= cantidad || porcentaje < 1 || porcentaje > 100) {
            cout << "Equipo o porcentaje fuera de rango" << endl;
            return;
        }

        float costoAntes = totalSemana() * tarifa;
        Equipo equipoOriginal = equipos[e];
        equipos[e].reducirHoras(porcentaje);
        calcularMatriz();
        float costoDespues = totalSemana() * tarifa;

        cout << fixed << setprecision(0);
        cout << "Costo semanal antes: " << costoAntes << " COP" << endl;
        cout << "Costo semanal despues: " << costoDespues << " COP" << endl;
        cout << "Ahorro semanal: " << costoAntes - costoDespues << " COP" << endl;

        equipos[e] = equipoOriginal;
        calcularMatriz();
    }

    void mostrarDetalleEquipo(int e) {
        if (e < 0 || e >= cantidad) {
            cout << "Numero de equipo fuera de rango" << endl;
            return;
        }
        cout << fixed << setprecision(2);
        cout << "Equipo: " << equipos[e].getNombre() << endl;
        cout << "Horas de uso en la semana: " << equipos[e].horasSemana() << " h" << endl;
        cout << "Dia de mayor uso: " << NOMBRE_DIA[equipos[e].diaDeMayorUso()] << endl;
        cout << "Porcentaje del consumo semanal: " << porcentajeEquipo(e) << "%" << endl;
    }

    // -----------------------------------------------------------------
    // Reporte: conclusiones a partir de la matriz.
    // Costo = kWh x tarifa.
    // -----------------------------------------------------------------
    void reporte() {
        int dia = diaMasCaro();                  // indice de la fila mayor
        int equipo = equipoQueMasGasta();        // indice de la columna mayor
        float costoSemana = totalSemana() * tarifa;

           cout << fixed << setprecision(2);
        cout << endl << "===== REPORTE: " << nombre << " =====" << endl;
        cout << "Tarifa: " << tarifa << " COP/kWh" << endl;
        cout << "Dia mas caro: " << NOMBRE_DIA[dia]
               << " (" << setprecision(0) << totalDia(dia) * tarifa << " COP)" << endl;
           cout << fixed << setprecision(2);
           cout << "Dia mas barato: " << NOMBRE_DIA[diaMasBarato()]
               << " (" << totalDia(diaMasBarato()) << " kWh)" << endl;
           cout << "Consumo promedio diario: " << promedioDiario() << " kWh" << endl;
           cout << fixed << setprecision(0);
        cout << "Equipo que mas gasta: " << equipos[equipo].getNombre()
             << " (" << totalEquipo(equipo) * tarifa << " COP en la semana)" << endl;
           cout << "Porcentaje del consumo semanal por equipo:" << endl;
           cout << fixed << setprecision(2);
           for (int e = 0; e < cantidad; e++) {
              cout << "  " << equipos[e].getNombre() << ": "
                  << porcentajeEquipo(e) << "%" << endl;
           }
           cout << fixed << setprecision(0);
        cout << "Costo de la semana: " << costoSemana << " COP" << endl;
        cout << "Proyeccion del mes (x 4): " << costoSemana * 4 << " COP" << endl;
    }
};  // <- fin de la clase Hogar


// =====================================================================
// PROGRAMA PRINCIPAL (paso 9: menu con entrada de datos)
// =====================================================================
int main() {
    // Crea el OBJETO casa vacio: el nombre y la tarifa se digitan en el menu
    Hogar casa("Mi casa", 0);
    int opcion;

    // do-while: el menu se repite HASTA que el usuario elija 0
    do {
        cout << endl << "===== MiConsumo =====" << endl;
        cout << "1. Datos de la casa (nombre y tarifa)" << endl;
        cout << "2. Agregar un equipo" << endl;
        cout << "3. Ver los equipos" << endl;
        cout << "4. Ver la matriz de consumo" << endl;
        cout << "5. Ver el reporte" << endl;
        cout << "6. Cargar los datos de mi casa" << endl;
        cout << "7. Simular un ahorro" << endl;
        cout << "8. Ver un equipo" << endl;
        cout << "0. Salir" << endl;
        cout << "Opcion: ";
        cin >> opcion;

        // switch: ejecuta el case que coincide con la opcion digitada
        switch (opcion) {

        case 1: {                                  // llaves: variables propias del case
            string nombre;
            float tarifa;
            cout << "Nombre de la casa: ";
            getline(cin >> ws, nombre);            // lee el texto completo, con espacios
            do {                                   // VALIDACION: repite si es invalida
                cout << "Tarifa de su factura (COP por kWh): ";
                cin >> tarifa;
            } while (tarifa <= 0);
            casa.setDatos(nombre, tarifa);
            cout << "Datos guardados" << endl;
            break;                                 // sale del switch
        }

        case 2: {
            if (casa.getCantidad() == MAX_EQUIPOS) {
                cout << "Ya hay " << MAX_EQUIPOS << " equipos: no caben mas" << endl;
                break;
            }
            string nombre;
            float potencia;
            float horas[DIAS];                     // VECTOR que llena el usuario
            cout << "Nombre del equipo: ";
            getline(cin >> ws, nombre);
            do {
                cout << "Potencia en vatios (W), segun la etiqueta: ";
                cin >> potencia;
            } while (potencia <= 0);
            // RECORRER el vector para LEER cada casilla desde el teclado
            for (int d = 0; d < DIAS; d++) {
                do {
                    cout << "Horas de uso el " << NOMBRE_DIA[d] << " (0 a 24): ";
                    cin >> horas[d];
                    if (horas[d] < 0 || horas[d] > 24) {
                        cout << "Dato invalido: ingrese un valor entre 0 y 24" << endl;
                    }
                } while (horas[d] < 0 || horas[d] > 24);
            }
            // Crea el objeto con los datos digitados y lo guarda en la casa
            casa.agregarEquipo(Equipo(nombre, potencia, horas));
            cout << "Equipo agregado" << endl;
            break;
        }

        case 3:
            casa.mostrarEquipos();
            break;

        case 4:
            if (casa.getCantidad() == 0) {
                cout << "Primero agregue equipos (opcion 2 o 6)" << endl;
            } else {
                casa.calcularMatriz();             // llena la matriz con los datos actuales
                casa.mostrarMatriz();
            }
            break;

        case 5:
            if (casa.getCantidad() == 0) {
                cout << "Primero agregue equipos (opcion 2 o 6)" << endl;
            } else {
                casa.calcularMatriz();
                casa.reporte();
            }
            break;

        case 6: {
            const float tarifaFactura = 960.34f;
            const float potenciaNevera = 55.42f;
            const float potenciaLavadora = 159.80f;
            float horasNevera[DIAS] = {24, 24, 24, 24, 24, 24, 24};
            float horasLavadora[DIAS] = {59.0f / 60, 59.0f / 60, 0, 59.0f / 60,
                                         59.0f / 60, 0, 0};
            float horasTelevisor[DIAS] = {3, 3, 3, 3, 3, 3, 3};
            float horasFreidora[DIAS] = {20.0f / 60, 0, 0, 0, 0, 20.0f / 60, 0};
            casa.vaciar();
            casa.setDatos("Mi casa", tarifaFactura);
            casa.agregarEquipo(Equipo("Nevera Haceb", potenciaNevera, horasNevera));
            casa.agregarEquipo(Equipo("Lavadora Haceb", potenciaLavadora, horasLavadora));
            casa.agregarEquipo(Equipo("Televisor Samsung", 60, horasTelevisor));
            casa.agregarEquipo(Equipo("Freidora de aire", 1520, horasFreidora));
              cout << "Datos de mi casa cargados. Tarifa de factura: "
                  << tarifaFactura << " COP/kWh" << endl;
            break;
        }

        case 7: {
            if (casa.getCantidad() == 0) {
                cout << "Primero cargue los equipos (opcion 6)" << endl;
                break;
            }
            int equipo;
            float porcentaje;
            cout << "Numero del equipo (0 a " << casa.getCantidad() - 1 << "): ";
            cin >> equipo;
            if (equipo < 0 || equipo >= casa.getCantidad()) {
                cout << "Numero de equipo fuera de rango" << endl;
                break;
            }
            do {
                cout << "Porcentaje de reduccion (1 a 100): ";
                cin >> porcentaje;
                if (porcentaje < 1 || porcentaje > 100) {
                    cout << "Dato invalido: ingrese un porcentaje entre 1 y 100" << endl;
                }
            } while (porcentaje < 1 || porcentaje > 100);
            casa.calcularMatriz();
            casa.simularAhorro(equipo, porcentaje);
            break;
        }

        case 8: {
            if (casa.getCantidad() == 0) {
                cout << "Primero cargue los equipos (opcion 6)" << endl;
                break;
            }
            int equipo;
            cout << "Numero del equipo (0 a " << casa.getCantidad() - 1 << "): ";
            cin >> equipo;
            casa.calcularMatriz();
            casa.mostrarDetalleEquipo(equipo);
            break;
        }

        case 0:
            cout << "Hasta luego" << endl;
            break;

        default:                                   // cualquier otro numero
            cout << "Opcion no valida" << endl;
        }

    } while (opcion != 0);

    return 0;                // 0 = el programa termino sin errores
}
