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
    //vector<int> carroDeCadaCamino; no es necesario porque no nos interesa qué carro corresponde a cada camino, solo la calidad
    vector<bool> carros_disponibles; // false -> no disponible | true -> disponible
    int k;                           // último camino que hemos considerado hasta el momento
    int calidad_real;                // suma de calidades para los caminos 0..k
    int calidad_estimada;            // calidad estimada
    bool operator<(Nodo const &otro) const { return calidad_estimada < otro.calidad_estimada; }
};

void vectorDeCalidadesMaximasAcumuladas(const vector<int> &anchuras_carros,
                                        const vector<int> &anchuras_caminos,
                                        const Matriz<int> &calidades,
                                        vector<int> &calidades_max_acumuladas) {
    int num_carros = anchuras_carros.size();
    int num_caminos = anchuras_caminos.size();
    // Calidad máxima que puede tener cada camino
    vector<int> calidades_maximas(num_caminos, 0);

    for (int camino = 0; camino < num_caminos; camino++)
        for (int carro = 0; carro < num_carros; carro++)
            if (anchuras_carros[carro] <= anchuras_caminos[camino]) // solo nos interesa si cabe el carro en el camino
                calidades_maximas[camino] = max(calidades_maximas[camino], calidades[carro][camino]);

    // calidades_max_acumuladas[i] = calidades_maximas[i+1] + ... + calidades_maximas[num_caminos - 1]
    calidades_max_acumuladas = vector<int>(num_caminos, 0);

    for (int camino = num_caminos - 2; camino >= 0; camino--)
        calidades_max_acumuladas[camino] = calidades_max_acumuladas[camino + 1] + calidades_maximas[camino + 1];
}

// Añadimos un nodo a la cola si la calidad estimada es mejor que la mejor calidad hasta el momento
inline void addNodeToQueue(priority_queue<Nodo> &queue, const Nodo &nuevo_nodo, int &mejor_calidad, int num_caminos) {
    if (nuevo_nodo.calidad_estimada > mejor_calidad) {
        if (nuevo_nodo.k == num_caminos - 1)
            mejor_calidad = nuevo_nodo.calidad_real;
        else
            queue.push(nuevo_nodo);
    }
}

/* Calcula la máxima calidad posible dadas las anchuras de los carros, de los caminos y las diferentes posibles calidades */
int resolver(const vector<int> &anchuras_carros,
             const vector<int> &anchuras_caminos,
             const Matriz<int> &calidades) {
    const int num_carros = anchuras_carros.size();
    const int num_caminos = anchuras_caminos.size();
    int mejor_calidad = -1;
    vector<int> calidades_max_acumuladas;
    priority_queue<Nodo> queue;

    Nodo primer_nodo = {.carros_disponibles = vector<bool>(num_carros, true),
            .k = -1,
            .calidad_real = 0,
            .calidad_estimada = 0 /* al ser el primer nodo la calidad estimada es irrelevante */};
    queue.push(primer_nodo);
    vectorDeCalidadesMaximasAcumuladas(anchuras_carros, anchuras_caminos, calidades, calidades_max_acumuladas);

    while (!queue.empty() && queue.top().calidad_estimada > mejor_calidad) {
        const Nodo nodo = queue.top();
        queue.pop();

        // Probamos con cada carro disponible
        for (int i = 0; i < num_carros; i++)
            if (nodo.carros_disponibles[i] && anchuras_carros[i] <= anchuras_caminos[nodo.k + 1]) {
                Nodo nuevo_nodo(nodo);
                nuevo_nodo.k++;
                nuevo_nodo.carros_disponibles[i] = false;
                nuevo_nodo.calidad_real += calidades[i][nuevo_nodo.k];
                nuevo_nodo.calidad_estimada = nuevo_nodo.calidad_real + calidades_max_acumuladas[nuevo_nodo.k];
                // Añadimos el nodo si parece mejorar la solución
                addNodeToQueue(queue, nuevo_nodo, mejor_calidad, num_caminos);
            }

        // Probamos a no asignar ningún carro al camino actual
        Nodo nuevo_nodo(nodo);
        nuevo_nodo.k++;
        nuevo_nodo.calidad_estimada = nuevo_nodo.calidad_real + calidades_max_acumuladas[nuevo_nodo.k];
        // Añadimos el nodo si parece mejorar la solución
        addNodeToQueue(queue, nuevo_nodo, mejor_calidad, num_caminos);
    }
    return mejor_calidad;
}

void resuelveCaso() {
    int num_carros;  // n
    int num_caminos; // m
    vector<int> anchuras_carros;
    vector<int> anchuras_caminos;
    Matriz<int> calidades; // calidades[i][j] es la calidad conseguida con el carro i y el camino j

    cin >> num_carros >> num_caminos;
    leerVector(anchuras_carros, num_carros);
    leerVector(anchuras_caminos, num_caminos);
    leerMatriz(calidades, num_carros, num_caminos);

    cout << resolver(anchuras_carros, anchuras_caminos, calidades) << endl;
}

int main() {
    // ajustes para que cin extraiga directamente de un fichero

#ifndef DOMJUDGE
    std::ifstream in(
            "/home/erik/Documents/University/2020-2021/2nd semester/MAR2/Problemas/15 - Quitanieves en Invernalia/1.in");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif
    int num_casos;
    cin >> num_casos;
    for (int i = 0; i < num_casos; i++)
        resuelveCaso();

// para dejar todo como estaba al principio
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
#endif

    return 0;
}
