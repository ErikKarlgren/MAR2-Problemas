// Grupo 15 - Erik Karlgren Domercq

/*

*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <queue>
#include <climits>

#include "Matriz.h"
#include "mar_utils.h"

//#define DEBUG

using namespace std;

/* Representa los datos de una solución parcial para el problema. */
struct Nodo
{
    vector<int> tareasParaCadaTrabajador;
    vector<bool> tareasDisponibles; // false -> no disponible | true -> disponible
    int k;                          // último trabajador que hemos considerado hasta el momento
    int coste_real;
    int coste_estimado;
    bool operator<(Nodo const &otro) const { return otro.coste_estimado < coste_estimado; }
};

/* sumas_costes_minimos[i] = suma de los tiempos de las tareas con menor tiempo para cada trabajador j (0 < i+1 < j <= n) */
void crearVectorDeSumaDeCostesMinimos(int n, const Matriz<int> &matriz, vector<int> &sumas_costes_minimos)
{
    vector<int> tiempos_minimos(n, INT_MAX);
    for (int trabajador = 0; trabajador < n; trabajador++)
        for (int tarea = 0; tarea < n; tarea++)
            tiempos_minimos[trabajador] = min(tiempos_minimos[trabajador], matriz[trabajador][tarea]);

    sumas_costes_minimos = vector<int>(n, 0);
    for (int i = n - 2; i >= 0; i--)
        sumas_costes_minimos[i] = tiempos_minimos[i + 1] + sumas_costes_minimos[i + 1];

#ifdef DEBUG
    mostrarVector(sumas_costes_minimos, "Vector sumas costes minimos: ");
#endif
}

/* Calcula el coste estimado de la siguiente forma: coste real + peor coste posible de cada tarea disponible. */
inline int costeEstimado(const Nodo &nodo, const vector<int> &sumas_costes_minimos)
{
    return nodo.coste_real + sumas_costes_minimos[nodo.k];
}

/* Actualiza la solución asignando la tarea 'tarea' al trabajador nodo.k */
inline void actualizarNodo(Nodo &nodo, const vector<int> &sumas_costes_minimos, const Matriz<int> &matriz, int tarea)
{
    nodo.k++;
    nodo.tareasParaCadaTrabajador[nodo.k] = tarea;
    nodo.tareasDisponibles[tarea] = false;
    nodo.coste_real += matriz[nodo.k][tarea];
    nodo.coste_estimado = costeEstimado(nodo, sumas_costes_minimos);
}

/* Continuación de la función resolver(). Es en esta parte donde vamos explorando el árbol de soluciones. */
void resolverLoop(int n, const Matriz<int> &matriz, const vector<int> &sumas_costes_minimos, priority_queue<Nodo> &queue,
                  int &mejor_coste, vector<int> &mejor_solucion)
{
    mejor_coste = INT_MAX;

    while (!queue.empty() && queue.top().coste_estimado < mejor_coste)
    {
        Nodo nodo = queue.top();
        queue.pop();
        // Probamos con cada tarea
        for (int i = 0; i < n; i++)
        {
            // Hacemos poda: solo nos sirven las soluciones en las que cada tarea se asigne a una
            // única persona, así que solo consideramos las tareas disponibles
            if (nodo.tareasDisponibles[i])
            {
                Nodo nuevo_nodo(nodo);
                actualizarNodo(nuevo_nodo, sumas_costes_minimos, matriz, i);

                // Solo añadimos nodos si podríamos conseguir una mejor solución
                if (nuevo_nodo.coste_estimado < mejor_coste)
                {
                    if (nuevo_nodo.k == n - 1) // Ya hemos elegido todas las tareas
                    {
                        mejor_coste = nuevo_nodo.coste_real;
                        mejor_solucion = nuevo_nodo.tareasParaCadaTrabajador;
                    }
                    else
                        queue.push(nuevo_nodo);
                }
            }
        }
    }
}

/* Calcula el mínimo tiempo necesario en total para realizar todas las n tareas al repartirlas entre n trabajadores */
int resolver(int n, const Matriz<int> &matriz)
{
    vector<int> mejor_solucion;
    int mejor_coste;

    priority_queue<Nodo> queue;
    vector<int> sumas_costes_minimos;
    Nodo primer_nodo;

    //crearVectorDePeoresCostes(n, matriz, sumas_costes_minimos);
    crearVectorDeSumaDeCostesMinimos(n, matriz, sumas_costes_minimos);

    primer_nodo.tareasParaCadaTrabajador = vector<int>(n, -1);
    primer_nodo.tareasDisponibles = vector<bool>(n, true);
    primer_nodo.k = -1;
    primer_nodo.coste_real = 0;
    primer_nodo.coste_estimado = costeEstimado(primer_nodo, sumas_costes_minimos);

    queue.push(primer_nodo);

    resolverLoop(n, matriz, sumas_costes_minimos, queue, mejor_coste, mejor_solucion);
    return mejor_coste;
}

bool resuelveCaso()
{
    // Número de funcionarios y de trabajos
    int n;
    // matriz[i][j] es el tiempo que necesita el trabajador i para hacer la tarea j
    Matriz<int> matriz;

    cin >> n;
    if (n == 0)
        return false;
    leerMatriz(matriz, n, n);
    cout << resolver(n, matriz) << endl;

    return true;
}

int main()
{
    // ajustes para que cin extraiga directamente de un fichero

#ifndef DOMJUDGE
    std::ifstream in("1.in");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

    while (resuelveCaso())
        ;

// para dejar todo como estaba al principio
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
#endif

    return 0;
}
