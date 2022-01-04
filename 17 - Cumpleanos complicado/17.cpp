// Grupo 15 - Erik Karlgren Domercq

/*

*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

#include "Matriz.h"
#include "mar_utils.h"

using namespace std;

/* Representa los datos de una solución parcial para el problema. */
struct Nodo {
    vector<bool> asientos_libres;   // asientos_libres[i] = true si está libre, false si está ocupado
    int ult_nino;                   // ultimo niño al que se le ha dado asiento
    int problemas_por_supersticion; // veces que hemos asignado un asiento levantando supersticiones
    int satisfaccion;               // suma satisfacciones para los niños 0..ult_nino
    int satisfaccion_estimada;      // satisfaccion estimada
    bool operator<(Nodo const &otro) const { return satisfaccion_estimada < otro.satisfaccion_estimada; }
};

void vectorDeSatisfaccionesMaximasAcumuladas(int num_ninos, int num_asientos, const Matriz<int> &mat_satisfaccion,
                                             vector<int> &satisfacciones_max_acumuladas) {
    // Satisfacción máxima que puede conseguir cada niño
    vector<int> satisfacciones_maximas(num_ninos, 0);

    for (int nino = 0; nino < num_ninos; nino++)
        for (int asiento = 0; asiento < num_asientos; asiento++)
            satisfacciones_maximas[nino] = max(satisfacciones_maximas[nino], mat_satisfaccion[asiento][nino]);

    satisfacciones_max_acumuladas = vector<int>(num_ninos, 0);

    for (int nino = num_ninos - 2; nino >= 0; nino--)
        satisfacciones_max_acumuladas[nino] =
                satisfacciones_max_acumuladas[nino + 1] + satisfacciones_maximas[nino + 1];
}

// Añadimos un nodo a la cola si la satisfacción estimada es mejor que la mejor calidad hasta el momento
inline void
addNodeToQueue(priority_queue<Nodo> &queue, const Nodo &nuevo_nodo, int &mejor_satisfaccion, int num_ninos) {
    // problema de maximizar (por eso el >)
    if (nuevo_nodo.satisfaccion_estimada > mejor_satisfaccion) {
        if (nuevo_nodo.ult_nino == num_ninos - 1)
            mejor_satisfaccion = nuevo_nodo.satisfaccion;
        else
            queue.push(nuevo_nodo);
    }
}

int resolver(int num_ninos, int num_asientos, int id_hermano, const Matriz<int> &mat_satisfaccion) {

    int mejor_satisfaccion = 0;
    const int lim_por_supersticion = num_asientos / 3;

    vector<int> satisfacciones_max_acumuladas;
    priority_queue<Nodo> queue;

    const Nodo primer_nodo = {
            .asientos_libres = vector<bool>(num_asientos, true),
            .ult_nino = -1,
            .problemas_por_supersticion = 0,
            .satisfaccion = 0,
            .satisfaccion_estimada = 99999 /* al ser el primer nodo el valor de este campo es irrelevante */};
    queue.push(primer_nodo);

    // Calculamos el vector de satisfacciones máximas acumuladas (para reducir el tiempo de calcular la satisfacción estimada)
    vectorDeSatisfaccionesMaximasAcumuladas(num_ninos, num_asientos, mat_satisfaccion, satisfacciones_max_acumuladas);

    while (!queue.empty() && queue.top().satisfaccion_estimada > mejor_satisfaccion) {
        const Nodo top = queue.top();
        queue.pop();

        // Probamos con cada asiento disponible
        for (int asiento = 0; asiento < num_asientos; asiento++) {
            if (top.asientos_libres[asiento] && // el asiento asiento está libre
                ((top.ult_nino + 1) % num_asientos != asiento // y no hay problema de supersticion
                 || top.problemas_por_supersticion < lim_por_supersticion) && // o hay pero no demasiados
                mat_satisfaccion[asiento][top.ult_nino + 1] >= 0) // y la satisfaccion es positiva
            {
                Nodo nuevo(top);
                nuevo.ult_nino++;
                nuevo.asientos_libres[asiento] = false;
                nuevo.satisfaccion += mat_satisfaccion[asiento][nuevo.ult_nino];
                nuevo.satisfaccion_estimada = nuevo.satisfaccion + satisfacciones_max_acumuladas[nuevo.ult_nino];

                if (nuevo.ult_nino % num_asientos == asiento)
                    nuevo.problemas_por_supersticion++;

                // Añadimos el nodo si parece mejorar la solución
                addNodeToQueue(queue, nuevo, mejor_satisfaccion, num_ninos);
            }
        }
        // Probamos a no asignarle ningún asiento al siguiente niño, si es que no es nuestro hermano
        if (top.ult_nino + 1 != id_hermano) {
            Nodo nuevo(top);
            nuevo.ult_nino++;
            nuevo.satisfaccion_estimada = nuevo.satisfaccion + satisfacciones_max_acumuladas[nuevo.ult_nino];
            // Añadimos el nodo si parece mejorar la solución
            addNodeToQueue(queue, nuevo, mejor_satisfaccion, num_ninos);
        }
    }
    return mejor_satisfaccion;
}

bool resuelveCaso() {
    int num_ninos;    // n: 1..20
    int num_asientos; // m
    int id_hermano;
    Matriz<int> satisfaccion; // satisfaccion[i][j] = satisfacción niño j en puesto i

    cin >> num_ninos >> num_asientos >> id_hermano;
    if (num_ninos == -1)
        return false;
    leerMatriz(satisfaccion, num_asientos, num_ninos);

    cout << resolver(num_ninos, num_asientos, id_hermano, satisfaccion) << endl;
    return true;
}

int main() {
#ifndef DOMJUDGE
    std::ifstream in(
            "/home/erik/Documents/University/2020-2021/2nd semester/MAR2/Problemas/17 - Cumpleanos complicado/1.in");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif
    while (resuelveCaso());

#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
#endif

    return 0;
}
