// Grupo 15 - Erik Karlgren Domercq

/*
   
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <set>

#include "Matriz.h"
#include "EnterosInf.h"
#include "mar_utils.h"

using namespace std;

struct Solucion
{
   bool hay_sol;
   EntInf n_monedas;
};

struct DatosMoneda
{
   int valor;
   int cantidad;
   bool operator<(const DatosMoneda &d) { return valor < d.valor; }
};

Solucion resolver(const vector<DatosMoneda> &datos, int precio)
{
   Solucion sol = {.hay_sol = false, .n_monedas = 0};
   vector<EntInf> monedas(precio + 1, Infinito);
   vector<int> num_monedas_usadas(precio + 1, 0);
   int num_monedas = datos.size();

   monedas[0] = 0;

   for (int i = 0; i <= precio; i++)
      cout << setw(4) << i << ' ';
   cout << endl;

   for (int i = 0; i < num_monedas; i++)
   {
      for (int j = 1; j <= precio; j++)
      {
         num_monedas_usadas[j] = 0;
         // el valor de la moneda no se pasa, nos quedan monedas todavía,
         // y usaríamos menos monedas
         if ((datos[i].valor <= j) &&
             (num_monedas_usadas[j - datos[i].valor] < datos[i].cantidad) &&
             (monedas[j - datos[i].valor] + 1 < monedas[j]))
         {
            num_monedas_usadas[j] = num_monedas_usadas[j - datos[i].valor] + 1;
            monedas[j] = monedas[j - datos[i].valor] + 1;
         }
      }

      /*for (int k = 0; k <= precio; k++)
      {
         cout << setw(4) << num_monedas_usadas[k] << ' ';
      }
      cout << endl;*/
      for (int k = 0; k <= precio; k++)
      {
         cout << setw(4) << monedas[k] << ' ';
      }
      cout << endl;
   }
   sol.hay_sol = monedas.back() != Infinito;
   sol.n_monedas = monedas.back();
   return sol;
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso()
{
   int N, precio;
   vector<int> valores;
   vector<int> cantidades;
   vector<DatosMoneda> datos;

   // leer los datos de la entrada
   cin >> N;

   if (!std::cin) // fin de la entrada
      return false;
   leerVector(valores, N);
   leerVector(cantidades, N);
   cin >> precio;

   for (int i = 0; i < N; i++)
   {
      DatosMoneda dm = {.valor = valores[i], .cantidad = cantidades[i]};
      datos.push_back(dm);
   }
   sort(datos.begin(), datos.end());

   Solucion sol = resolver(datos, precio);

   // escribir sol
   if (sol.hay_sol)
   {
      cout << "SI " << sol.n_monedas << endl;
   }
   else
   {
      cout << "NO" << endl;
   }

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
