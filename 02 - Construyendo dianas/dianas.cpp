
// Nombre y Apellidos: Erik Karlgren Domercq

// Comentario general sobre la solución,
// explicando cómo se resuelve el problema

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include "EnterosInf.h" // propios o los de las estructuras de datos de clase
using namespace std;

using Datos = struct
{
   unsigned puntos_a_conseguir;
   unsigned num_sectores;
   vector<unsigned> puntos_sectores;
};

using Solucion = struct
{
   bool hay_solucion;
   EntInf min_num_dardos;
   list<unsigned> puntos_necesarios_ordenados_decrecientemente;
};

/*
   Sean X la puntuación que queremos conseguir y S el número de sectores.

   Mediante un algoritmo basado en programación dinámica ascendente, rellenamos
   un vector de X+1 elementos de la siguiente forma: para cada sector s (0<=s<=S)
   calculamos el número mínimo de dardos necesario para conseguir una puntuación
   p (0<=p<=X), y guardamos el resultado en la posición [p] del vector.

   ...
*/
Solucion resolver(Datos datos)
{
   Solucion sol = {.hay_solucion = false, .min_num_dardos = Infinito};
   // Vector con el minimo numero de dardos necesarios para cada puntuacion p 0<=p<=X
   vector<EntInf> vec_dardos(datos.puntos_a_conseguir + 1, Infinito);

   // Caso base
   vec_dardos[0] = 0;

   // Rellenamos el vector vec_dardos: O(X * S)
   for (unsigned s = 1; s <= datos.num_sectores; s++)
      for (unsigned p = datos.puntos_sectores[s]; p <= datos.puntos_a_conseguir; p++)
         vec_dardos[p] = min(vec_dardos[p],
                             vec_dardos[p - datos.puntos_sectores[s]] + 1);

   // Reconstruimos la solución: O(X + S)
   if (vec_dardos[datos.puntos_a_conseguir] != Infinito)
   {
      unsigned p = datos.puntos_a_conseguir;
      unsigned s = datos.num_sectores;
      while (p > 0) // Al llegar a 0 puntos (p==0) habremos calculado las puntuaciones necesarias
      {
         if (datos.puntos_sectores[s] <= p && vec_dardos[p] > vec_dardos[p - datos.puntos_sectores[s]])
         {
            sol.puntos_necesarios_ordenados_decrecientemente.push_back(datos.puntos_sectores[s]);
            p -= datos.puntos_sectores[s];
         }
         else
            s--;
      }
      sol.hay_solucion = true;
      sol.min_num_dardos = vec_dardos[datos.puntos_a_conseguir];
   }

   return sol;
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso()
{
   Datos datos;
   Solucion sol;

   // leer los datos de la entrada
   cin >> datos.puntos_a_conseguir;

   if (!std::cin) // fin de la entrada
      return false;

   cin >> datos.num_sectores;
   datos.puntos_sectores = vector<unsigned>(datos.num_sectores + 1);
   datos.puntos_sectores[0] = 0;
   for (size_t i = 1; i <= datos.num_sectores; i++)
      cin >> datos.puntos_sectores[i];

   sol = resolver(datos);

   // escribir sol
   if (sol.hay_solucion)
   {
      cout << sol.min_num_dardos << ':';
      for (unsigned puntos : sol.puntos_necesarios_ordenados_decrecientemente)
         cout << ' ' << puntos;
      cout << endl;
   }
   else
      cout << "Imposible" << endl;

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
   //system("PAUSE");
#endif
   return 0;
}
