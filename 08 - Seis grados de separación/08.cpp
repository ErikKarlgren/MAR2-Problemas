// Grupo 15 - Erik Karlgren Domercq

/*
   Para buscar el grado de separación de la red usamos búsqueda en anchura desde cada nodo,
   pues encontrará la mínima distancia entre un nodo v y todos los nodos w de la red, y
   mantendremos un entero con el grado de separación calculado hasta el momento. Si cualquiera
   de las distancias calculadas con búsqueda en anchura resulta ser mayor que el grado de
   separación actual, este se actualizará a dicha distancia.

   Si por búsqueda en anchura no recorremos todos los nodos es que la red está deconectada.

   V: número de vértices
   A: número de aristas

   Coste de la búsqueda por anchura sobre un nodo: O(V + A)
   Coste por hacerla para cada vértice: O(V^2 + V*A)
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <queue>

#include "Grafo.h"

using namespace std;

struct Solucion
{
   bool red_conectada;
   int grado_separacion;
};

enum visitState
{
   NOT_VISITED,
   VISITED,
   CLOSED
};

Solucion resolver(const Grafo &grafo)
{
   Solucion sol = {.red_conectada = true, .grado_separacion = 0};

   // Búsqueda en anchura V veces (V = número de vértices)
   for (int v = 0; v < grafo.V() && sol.red_conectada; v++)
   {
      vector<visitState> nodes_visit_state(grafo.V(), NOT_VISITED);
      vector<int> distances(grafo.V(), 0);
      queue<int> queue;
      int num_of_visited_nodes = 1;

      queue.push(v);
      nodes_visit_state[v] = VISITED;

      while (!queue.empty())
      {
         int w = queue.front();
         queue.pop();

         for (int adj : grafo.ady(w))
         {
            if (nodes_visit_state[adj] == NOT_VISITED)
            {
               nodes_visit_state[adj] = VISITED;
               distances[adj] = distances[w] + 1;
               queue.push(adj);

               sol.grado_separacion = max(sol.grado_separacion, distances[adj]);
               num_of_visited_nodes++;
            }
         }
         nodes_visit_state[w] = CLOSED;
      }
      if (num_of_visited_nodes != grafo.V())
         sol.red_conectada = false;
   }
   return sol;
}

bool resuelveCaso()
{
   Solucion sol;
   unordered_map<string, int> nameToId;
   int num_personas;
   int num_relaciones;
   int next_id = 0;

   cin >> num_personas;
   if (!std::cin) // fin de la entrada
      return false;
   cin >> num_relaciones;

   Grafo grafo(num_personas);

   for (int i = 0; i < num_relaciones; i++)
   {
      string pers1, pers2;
      cin >> pers1 >> pers2;

      if (nameToId.find(pers1) == nameToId.end())
         nameToId[pers1] = next_id++;

      if (nameToId.find(pers2) == nameToId.end())
         nameToId[pers2] = next_id++;

      grafo.ponArista(nameToId[pers1], nameToId[pers2]);
   }

   sol = resolver(grafo);

   if (sol.red_conectada)
      cout << sol.grado_separacion << endl;
   else
      cout << "DESCONECTADA" << endl;

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
