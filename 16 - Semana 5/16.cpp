// Grupo 15 - Erik Karlgren Domercq

/*

*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <climits>

#include "Matriz.h"
#include "mar_utils.h"

using namespace std;

const int LIM_PRODUCTOS = 3;

/* Representa los datos de una solución parcial para el problema. */
struct Nodo {
    vector<int> supermercado_elegido; // supermercado_elegido[i]: supermercado elegido para el producto i
    vector<int> productos_comprados;  // productos_comprados[i]: productos comprados en super i
    int k;                            // productos comprados hasta ahora
    int precio_real;                  // suma de precios para los productos 0..k
    int precio_estimado;              // precio estimado
    bool operator<(Nodo const &otro) const { return precio_estimado > otro.precio_estimado; }
};

void vectorDePreciosMinimosAcumulados(int num_productos,
                                      int num_supermercados,
                                      const Matriz<int> &precios,
                                      vector<int> &precios_min_acumulados) {
    // Calidad máxima que puede tener cada camino
    vector<int> precios_minimos(num_productos, INT_MAX);

    for (int producto = 0; producto < num_productos; producto++)
        for (int supermercado = 0; supermercado < num_supermercados; supermercado++)
            precios_minimos[producto] = min(precios_minimos[producto], precios[supermercado][producto]);

    precios_min_acumulados = vector<int>(num_productos, 0);

    for (int producto = num_productos - 2; producto >= 0; producto--)
        precios_min_acumulados[producto] = precios_min_acumulados[producto + 1] + precios_minimos[producto + 1];
}

// Añadimos un nodo a la cola si la calidad estimada es mejor que la mejor calidad hasta el momento
inline void addNodeToQueue(priority_queue<Nodo> &queue, const Nodo &nuevo_nodo, int &mejor_precio, int num_productos) {
    // problema de minimizar (por eso el <)
    if (nuevo_nodo.precio_estimado < mejor_precio) {
        if (nuevo_nodo.k == num_productos - 1)
            mejor_precio = nuevo_nodo.precio_real;
        else
            queue.push(nuevo_nodo);
    }
}

int resolver(int num_supermercados,
             int num_productos,
             const Matriz<int> &precios) {

    int mejor_precio = INT_MAX;
    vector<int> precios_min_acumulados;
    priority_queue<Nodo> queue;

    // Creamos un primer nodo
    Nodo primer_nodo = {
            .supermercado_elegido=vector<int>(num_productos, -1),
            .productos_comprados = vector<int>(num_supermercados, 0),
            .k = -1,
            .precio_real = 0,
            .precio_estimado = 0};
    queue.push(primer_nodo);

    // Calculamos el vector de precios minimos acumulados (para reducir el tiempo para calcular el coste estimado)
    vectorDePreciosMinimosAcumulados(num_productos, num_supermercados, precios, precios_min_acumulados);

    while (!queue.empty() && queue.top().precio_estimado < mejor_precio) {
        const Nodo nodo = queue.top();
        queue.pop();

        // Probamos con cada super disponible
        for (int i = 0; i < num_supermercados; i++)
            if (nodo.productos_comprados[i] < LIM_PRODUCTOS) {
                Nodo nuevo_nodo(nodo);
                nuevo_nodo.k++;
                nuevo_nodo.supermercado_elegido[nuevo_nodo.k] = i;
                nuevo_nodo.productos_comprados[i]++;
                nuevo_nodo.precio_real += precios[i][nuevo_nodo.k];
                nuevo_nodo.precio_estimado = nuevo_nodo.precio_real + precios_min_acumulados[nuevo_nodo.k];
                // Añadimos el nodo si parece mejorar la solución
                addNodeToQueue(queue, nuevo_nodo, mejor_precio, num_productos);
            }
    }
    return mejor_precio;
}

void resuelveCaso() {
    int num_productos;
    int num_supermercados;
    Matriz<int> precios; //precios[i][j] es precio del producto j en supermercado i

    cin >> num_supermercados >> num_productos;
    leerMatriz(precios, num_supermercados, num_productos);

    cout << resolver(num_supermercados, num_productos, precios) << endl;
}

int main() {
#ifndef DOMJUDGE
    std::ifstream in("1.in");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif
    int num_casos;
    cin >> num_casos;
    for (int i = 0; i < num_casos; i++)
        resuelveCaso();

#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
#endif

    return 0;
}
