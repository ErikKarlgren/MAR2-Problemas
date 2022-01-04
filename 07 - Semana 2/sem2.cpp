// Grupo 15 - Erik Karlgren Domercq

/*
   
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdint>
//#include <string>

using namespace std;
#include "Matriz.h" // propios o los de las estructuras de datos de clase

#define int int_fast32_t

int subsecuencia(string const &palabra1, string const &palabra2, int i, int j, Matriz<int> &m)
{
   int &res = m[i][j];
   if (res == -1)
   {
      if (i == 0 || j == 0)
         res = 0;
      else if (palabra1[i - 1] == palabra2[j - 1])
         res = subsecuencia(palabra1, palabra2, i - 1, j - 1, m) + 1;
      else
         res = max(subsecuencia(palabra1, palabra2, i - 1, j, m), subsecuencia(palabra1, palabra2, i, j - 1, m));
   }
   return res;
}

void reconstruir(string const &palabra1, string const &palabra2, Matriz<int> const &m, int i, int j, string &sol)
{
   if (i != 0 && j != 0)
   {
      if (palabra1[i - 1] == palabra2[j - 1])
      {
         reconstruir(palabra1, palabra2, m, i - 1, j - 1, sol);
         sol.push_back(palabra1[i - 1]);
      }
      else if (m[i][j] == m[i][j - 1])
         reconstruir(palabra1, palabra2, m, i, j - 1, sol);
      else
         reconstruir(palabra1, palabra2, m, i - 1, j, sol);
   }
}

bool resuelveCaso()
{
   // leer los datos de la entrada
   string p1, p2;
   cin >> p1;

   if (!std::cin) // fin de la entrada
      return false;
   cin >> p2;

   Matriz<int> m((int)p1.size() + 1, (int)p2.size() + 1, -1);

   int sol = subsecuencia(p1, p2, (int)p1.size(), (int)p2.size(), m);

   //cerr << m << endl;

   if (sol != 0)
   {
      string pFinal;
      reconstruir(p1, p2, m, (int)p1.size(), (int)p2.size(), pFinal);
      cout << pFinal << '\n';
   }
   else
      cout << endl;

   return true;
}

#undef int

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
