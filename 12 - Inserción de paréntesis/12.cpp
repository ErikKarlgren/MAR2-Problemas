// Grupo 15 - Erik Karlgren Domercq

/*
   
*/

#include <iostream>
#include <fstream>
#include <algorithm>

#include "Matriz.h"

using namespace std;

inline bool sePuedeFormarA(const Matriz<bool> &solsA,
                           const Matriz<bool> &solsB,
                           const Matriz<bool> &solsC,
                           int i, int k, int j)
{
   return (solsA[i][k] && solsC[k + 1][j]) ||
          (solsB[i][k] && solsC[k + 1][j]) ||
          (solsC[i][k] && solsA[k + 1][j]);
}

inline bool sePuedeFormarB(const Matriz<bool> &solsA,
                           const Matriz<bool> &solsB,
                           int i, int k, int j)
{
   return (solsA[i][k] && solsA[k + 1][j]) ||
          (solsA[i][k] && solsB[k + 1][j]) ||
          (solsB[i][k] && solsB[k + 1][j]);
}
inline bool sePuedeFormarC(const Matriz<bool> &solsA,
                           const Matriz<bool> &solsB,
                           const Matriz<bool> &solsC,
                           int i, int k, int j)
{
   return (solsB[i][k] && solsA[k + 1][j]) ||
          (solsC[i][k] && solsB[k + 1][j]) ||
          (solsC[i][k] && solsC[k + 1][j]);
}

bool resolver(const string &cadena)
{
   Matriz<bool> solsA(cadena.length(), cadena.length());
   Matriz<bool> solsB(cadena.length(), cadena.length());
   Matriz<bool> solsC(cadena.length(), cadena.length());
   bool hay_sol = false;

   // casos base
   for (int i = 0; i < (int)cadena.length(); i++)
   {
      solsA[i][i] = (cadena[i] == 'a');
      solsB[i][i] = (cadena[i] == 'b');
      solsC[i][i] = (cadena[i] == 'c');
   }

   // casos recursivos: nos desplazamos por diagonales
   for (int d = 1; d < (int)cadena.size(); d++)
      for (int i = 0; i < (int)cadena.size() - d; i++)
      {
         int j = i + d;
         for (int k = i; k < j; k++)
         {
            solsA[i][j] = solsA[i][j] || sePuedeFormarA(solsA, solsB, solsC, i, k, j);
            solsB[i][j] = solsB[i][j] || sePuedeFormarB(solsA, solsB, i, k, j);
            solsC[i][j] = solsC[i][j] || sePuedeFormarC(solsA, solsB, solsC, i, k, j);
         }
      }

   /*cout << solsA << endl
        << solsB << endl
        << solsC << endl;*/

   hay_sol = solsA[0][cadena.length() - 1];
   return hay_sol;
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso()
{
   string cadena;
   cin >> cadena;

   if (!cin)
      return false;
   else if (resolver(cadena))
      cout << "SI" << endl;
   else
      cout << "NO" << endl;

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
