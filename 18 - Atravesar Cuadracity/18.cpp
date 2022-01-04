// Grupo 15 - Erik Karlgren Domercq

/*
   
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstdint>

#include "Matriz.h"
#include "mar_utils.h"

const char PASADIZO = 'P';
const char INTERSEC_SEGURA = '.';

using namespace std;

/*
   Coste en tiempo: O(num_calles_este_oeste * num_calles_norte_sur)
   Coste en espacio: O(min(num_calles_este_oeste, num_calles_norte_sur))
*/
int64_t resolver(int num_calles_este_oeste, int num_calles_norte_sur, const Matriz<char> &intersecciones)
{
   const int min_calles = min(num_calles_este_oeste, num_calles_norte_sur);
   const int max_calles = max(num_calles_este_oeste, num_calles_norte_sur);
   const bool menos_calles_este_oeste = num_calles_este_oeste <= num_calles_norte_sur;
   auto leer_interseccion = /* asumimos que 0 <= i < max_calles y 0 <= j < min_calles */
       [menos_calles_este_oeste, intersecciones](int i, int j) { return menos_calles_este_oeste ? intersecciones[j][i] : intersecciones[i][j]; };

   vector<int64_t> num_caminos_minimos(min_calles);

   // Caso base: interseccion inicial
   num_caminos_minimos[0] = (leer_interseccion(0, 0) == INTERSEC_SEGURA) ? 1 : 0;

   // Caso recursivo: primera calle (no hay calles anteriores)
   for (int j = 1; j < min_calles; j++)
      if (leer_interseccion(0, j) == INTERSEC_SEGURA)
         num_caminos_minimos[j] = num_caminos_minimos[j - 1];
      else
         num_caminos_minimos[j] = 0;

   // Caso recursivo: segunda calle en adelante
   for (int i = 1; i < max_calles; i++)
   {
      // Caso recursivo: primera interseccion de la calle (solo se actualiza si encontramos un pasadizo)
      if (leer_interseccion(i, 0) == PASADIZO)
         num_caminos_minimos[0] = 0;

      for (int j = 1; j < min_calles; j++)
         if (leer_interseccion(i, j) == INTERSEC_SEGURA)
            num_caminos_minimos[j] = num_caminos_minimos[j] + num_caminos_minimos[j - 1];
         else
            num_caminos_minimos[j] = 0;
   }

   return num_caminos_minimos[min_calles - 1];
}

bool resuelveCaso()
{
   int num_calles_este_oeste;
   int num_calles_norte_sur;
   Matriz<char> intersecciones;

   cin >> num_calles_este_oeste >> num_calles_norte_sur;
   if (!cin)
      return false;

   leerMatriz(intersecciones, num_calles_este_oeste, num_calles_norte_sur);
   cout << resolver(num_calles_este_oeste, num_calles_norte_sur, intersecciones) << endl;
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
