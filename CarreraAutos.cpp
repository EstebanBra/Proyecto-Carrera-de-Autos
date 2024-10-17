#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;
// Mutex para sincronizar la salida a la consola y la llegada de los autos
mutex mtx;

struct Resultado {
    int id;
    int posicion;
};

// Variables globales para almacenar el orden de llegada
vector<Resultado> resultados;
int posicion_actual = 1;

// Función que simula la carrera  autos
void carreraAuto(int id, int meta) {
    int trayectoRecorrido = 0;
    random_device rd; // Generador de numeros aleatorios
    mt19937 gen(rd()); // Algorimo de generacion num aleatorios llamado Mersenne twister
    uniform_int_distribution<> avanceAleatoreo(1, 10);     // Avance aleatorio entre 1 y 10 metros
    uniform_int_distribution<> retardoAvance(100, 500);  // Pausa aleatoria entre 100 y 500 ms

    while (trayectoRecorrido < meta) {
        
        // Calcular avance aleatorio
        int avance = avanceAleatoreo(gen);
        trayectoRecorrido += avance;
        
        if (trayectoRecorrido > meta){
          trayectoRecorrido = meta;
        }
        // Esto asegura que solo un auto(hebra) se muestre por la consola,
        // es decir evita que varios autos escriban a mismo tiempo en la consola
        {  
            lock_guard<mutex> lock(mtx);
            // Se imprime el id, el avance, el trayecto recorrido y la meta
            cout << "Auto " << id << " avanzó " << avance << " metros. Total: " 
                 << trayectoRecorrido << "/" << meta << " metros.\n";
        }

        // Dormir por un tiempo aleatorio (simula la velocidad del auto)
        this_thread::sleep_for(chrono::milliseconds(retardoAvance(gen)));
    }

    // Cuando el auto termina, registrar su posición de llegada
    {
        lock_guard<mutex> lock(mtx);
        resultados.push_back({id, posicion_actual});
        cout << "Auto " << id << " ha terminado la carrera en la posición " << posicion_actual << "!\n";
        posicion_actual++;
    }
}

int main(int argc, char* argv[]) {

    // Solicitar información al usuario
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " <distancia> <num_autos>\n";
        return 1;
    }

    int meta = stoi(argv[1]);
    int numAutos = stoi(argv[2]);
    
    // Crear un vector para almacenar los hilos
    vector<thread> autos;

    // Crea los hilos (uno para cada auto)
    for (int i = 1; i <= numAutos; ++i) {
        autos.push_back(thread(carreraAuto, i, meta)); //
    }

    // Esperar a que todos los hilos terminen
    for (auto& t : autos) {
        t.join(); // t, con referencia thread y join() es un metodo que se encarga de terminar la ejecucion
    }

    // Mostrar el resultado final de la carrera
    cout << "\nLa carrera ha terminado! Resultados finales:\n";
    
    // Sort, es una fucion de c++ que se incluye con <algorithm>, esta se utilisa para ordenar contenedores
    // Ordena los autos comparando sus pociciones de llegada, y ver en que pocicion termino la carrera
    sort(resultados.begin(), resultados.end(), [](Resultado a, Resultado b){ return a.posicion < b.posicion; });
    
    // Muestra los resultados y las pocicion de los autos ordenados por el sort
    for (const auto& res : resultados) {
        cout << "Auto " << res.id << " terminó en la posición " << res.posicion << ".\n";
    }

    return 0;
}