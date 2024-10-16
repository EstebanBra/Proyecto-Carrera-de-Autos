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

// Estructura para almacenar el resultado de cada auto
struct Resultado {
    int id;
    int posicion;
};

// Variables globales para almacenar el orden de llegada
vector<Resultado> resultados;
int posicion_actual = 1; // Posición de llegada de los autos

// Función que simula la carrera de autos
void carreraAuto(int id, int meta) {
    int trayectoRecorrido = 0;
    random_device rd; // Generador de números aleatorios
    mt19937 gen(rd()); // Algoritmo de generación numérica Mersenne Twister
    uniform_int_distribution<> avanceAleatorio(1, 10);     // Avance aleatorio entre 1 y 10 metros
    uniform_int_distribution<> retardoAvance(100, 500);  // Pausa aleatoria entre 100 y 500 ms

    while (trayectoRecorrido < meta) {
        // Calcular avance aleatorio
        int avance = avanceAleatorio(gen);
        trayectoRecorrido += avance;

        if (trayectoRecorrido > meta) {
            trayectoRecorrido = meta; // Ajustar si se sobrepasa la meta
        }

        // Asegura que solo un auto (hebra) se muestre por la consola a la vez
        {
            lock_guard<mutex> lock(mtx); // Bloquea el mutex para evitar condiciones de carrera
            cout << "Auto " << id << " avanzó " << avance << " metros. Total: " 
                 << trayectoRecorrido << "/" << meta << " metros.\n";
        }

        // Dormir por un tiempo aleatorio (simula la velocidad del auto)
        this_thread::sleep_for(chrono::milliseconds(retardoAvance(gen)));
    }

    // Cuando el auto termina, registrar su posición de llegada
    {
        lock_guard<mutex> lock(mtx); // Bloquear el mutex para la sección crítica
        resultados.push_back({id, posicion_actual});
        cout << "Auto " << id << " ha terminado la carrera en la posición " << posicion_actual << "!\n";
        posicion_actual++;
    }
}

int main() {
    int meta, numAutos;

    // Solicitar información al usuario
    cout << "Introduce la distancia total de la carrera (en metros): ";
    cin >> meta;
    cout << "Introduce el número de autos: ";
    cin >> numAutos;

    // Crear un vector para almacenar los hilos
    vector<thread> autos;

    // Crear los hilos (uno para cada auto)
    for (int i = 1; i <= numAutos; ++i) {
        autos.push_back(thread(carreraAuto, i, meta)); // Crear un hilo para cada auto
    }

    // Esperar a que todos los hilos terminen
    for (auto& t : autos) {
        t.join(); // Unir el hilo, esperando su finalización
    }

    // Mostrar el resultado final de la carrera
    cout << "\nLa carrera ha terminado! Resultados finales:\n";
    
    // Ordenar los resultados basados en la posición de llegada
    sort(resultados.begin(), resultados.end(), [](Resultado a, Resultado b) { return a.posicion < b.posicion; });
    
    // Mostrar los resultados ordenados
    for (const auto& res : resultados) {
        cout << "Auto " << res.id << " terminó en la posición " << res.posicion << ".\n";
    }

    return 0;
}
