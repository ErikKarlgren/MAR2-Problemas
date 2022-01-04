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

using namespace std;

/* Representa los datos de una solución parcial para el problema. */
struct Nodo
{
    vector<int> tareasParaCadaTrabajador;
    vector<bool> tareasDisponibles; // false -> no disponible | true -> disponible
    int ult_trabajador;             // último trabajador que hemos considerado hasta el momento
    int coste_real;                 // coste de la solución parcial
    int coste_optimista;            // coste en el mejor de los casos (el mínimo posible)
    int coste_pesimista;            // coste en el peor de los casos (el máximo posible)
    bool operator<(Nodo const &otro) const { return otro.coste_optimista < coste_optimista; }
};

/* sumas_costes_minimos[i] = suma de los tiempos de las tareas con menor tiempo para cada trabajador j (0 < i+1 < j <= n) */
void crearVectorDeSumaDeCostesOptimistas(int n, const Matriz<int> &matriz, vector<int> &sumas_costes_minimos)
{
    vector<int> tiempos_minimos(n, INT_MAX);
    for (int trabajador = 0; trabajador < n; trabajador++)
        for (int tarea = 0; tarea < n; tarea++)
            tiempos_minimos[trabajador] = min(tiempos_minimos[trabajador], matriz[trabajador][tarea]);

    sumas_costes_minimos = vector<int>(n, 0);
    for (int i = n - 2; i >= 0; i--)
        sumas_costes_minimos[i] = tiempos_minimos[i + 1] + sumas_costes_minimos[i + 1];
}

/* sumas_costes_maximos[i] = suma de los tiempos de las tareas con mayor tiempo para cada trabajador j (0 < i+1 < j <= n) */
void crearVectorDeSumaDeCostesPesimistas(int n, const Matriz<int> &matriz, vector<int> &sumas_costes_maximos)
{
    vector<int> tiempos_maximos(n, 0);
    for (int trabajador = 0; trabajador < n; trabajador++)
        for (int tarea = 0; tarea < n; tarea++)
            tiempos_maximos[trabajador] = max(tiempos_maximos[trabajador], matriz[trabajador][tarea]);

    sumas_costes_maximos = vector<int>(n, 0);
    for (int i = n - 2; i >= 0; i--)
        sumas_costes_maximos[i] = tiempos_maximos[i + 1] + sumas_costes_maximos[i + 1];
}

/* Calcula el coste optimista estimado */
inline int costeOptimistaEstimado(const Nodo &nodo, const vector<int> &sumas_costes_minimos)
{
    return nodo.coste_real + sumas_costes_minimos[nodo.ult_trabajador];
}
/* Calcula el coste pesimista estimado */
inline int costePesimistaEstimado(const Nodo &nodo, const vector<int> &sumas_costes_maximos)
{
    return nodo.coste_real + sumas_costes_maximos[nodo.ult_trabajador];
}

/* Calcula el mínimo tiempo necesario en total para realizar todas las n tareas al repartirlas entre n trabajadores */
int resolver(int n, const Matriz<int> &matriz)
{
    int mejor_coste;

    vector<int> mejor_solucion;
    priority_queue<Nodo> queue;
    vector<int> sumas_costes_minimos;
    vector<int> sumas_costes_maximos;

    Nodo primer_nodo = {
        .tareasParaCadaTrabajador = vector<int>(n, -1),
        .tareasDisponibles = vector<bool>(n, true),
        .ult_trabajador = -1,
        .coste_real = 0,
        .coste_optimista = 1, /* primer nodo de la cola, así que valor del coste optimista irrelevante */
        .coste_pesimista = INT_MAX};

    // mejor_coste tendrá el peor coste posible en cada momento, y al terminar el algoritmo tendrá el mejor coste posible
    mejor_coste = primer_nodo.coste_pesimista;

    crearVectorDeSumaDeCostesOptimistas(n, matriz, sumas_costes_minimos);
    crearVectorDeSumaDeCostesPesimistas(n, matriz, sumas_costes_maximos);

    queue.push(primer_nodo);

    while (!queue.empty() && queue.top().coste_optimista < mejor_coste)
    {
        Nodo top = queue.top();
        queue.pop();
        // Probamos con cada tarea
        for (int tarea = 0; tarea < n; tarea++)
        {
            // Hacemos poda: solo nos sirven las soluciones en las que cada tarea se asigne a una
            // única persona, así que solo consideramos las tareas disponibles
            if (top.tareasDisponibles[tarea])
            {
                Nodo nodo(top);
                nodo.ult_trabajador++;
                nodo.tareasParaCadaTrabajador[nodo.ult_trabajador] = tarea;
                nodo.tareasDisponibles[tarea] = false;
                nodo.coste_real += matriz[nodo.ult_trabajador][tarea];
                nodo.coste_optimista = costeOptimistaEstimado(nodo, sumas_costes_minimos);
                nodo.coste_pesimista = costePesimistaEstimado(nodo, sumas_costes_maximos);

                // Solo añadimos nodos si podríamos conseguir una mejor solución
                if (nodo.coste_optimista < mejor_coste)
                {
                    if (nodo.ult_trabajador == n - 1) // Ya hemos elegido tareas para todos los trabajadores
                    {
                        mejor_coste = nodo.coste_real;
                        mejor_solucion = nodo.tareasParaCadaTrabajador;
                    }
                    else
                    {
                        queue.push(nodo);
                        if (nodo.coste_pesimista < mejor_coste)
                            mejor_coste = nodo.coste_pesimista;
                    }
                }
            }
        }
    }
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
