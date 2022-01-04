// Grupo 15 - Erik Karlgren Domercq

/*
   
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <deque>

#include "Matriz.h"

using namespace std;

const int NOT_INITIALIZED = -1;

struct Cofre
{
   int profundidad;
   int oro;
   int tiempo_en_recoger() const { return 3 * profundidad; };
};

struct Datos
{
   int max_segundos;
   int num_cofres;
   vector<Cofre> cofres;
};

struct Solucion
{
   int max_oro = 0;
   int num_cofres = 0;
   deque<Cofre> cofres;
};

int algCofresRec(const vector<Cofre> &cofres, Matriz<int> &costes, int max_segundos, int i, int t)
{
   int &res = costes[i][t];

   if (res == NOT_INITIALIZED)
   {
      if (i == 0 || t == 0)
         res = 0;
      else if (cofres[i - 1].tiempo_en_recoger() > t)
         res = algCofresRec(cofres, costes, max_segundos, i - 1, t);
      else
         res = max(algCofresRec(cofres, costes, max_segundos, i - 1, t),
                   algCofresRec(cofres, costes, max_segundos, i - 1, t - cofres[i - 1].tiempo_en_recoger()) + cofres[i - 1].oro);
   }

   return res;
}

void reconstruirSolucion(const Datos &datos, const Matriz<int> &costes, Solucion &sol)
{
   const auto &cofres = datos.cofres;
   int i = datos.num_cofres;
   int t = datos.max_segundos;

   while (i > 0 && t > 0)
   {
      if (costes[i][t] != costes[i - 1][t])
      {
         sol.cofres.push_front(cofres[i - 1]);
         t = t - cofres[i - 1].tiempo_en_recoger();
         sol.num_cofres++;
         sol.max_oro += cofres[i - 1].oro;
      }
      i--;
   }
}

void algCofres(const Datos &datos)
{
   Solucion sol;
   Matriz<int> costes(datos.num_cofres + 1, datos.max_segundos + 1, NOT_INITIALIZED);

   costes[datos.num_cofres][datos.max_segundos] =
       algCofresRec(datos.cofres, costes, datos.max_segundos, datos.num_cofres, datos.max_segundos);

   reconstruirSolucion(datos, costes, sol);

   cout << sol.max_oro << endl
        << sol.num_cofres << endl;

   for (auto c : sol.cofres)
      cout << c.profundidad << ' ' << c.oro << endl;
   cout << "---" << endl;
}

bool resuelveCaso()
{
   // leer los datos de la entrada
   Datos datos;

   cin >> datos.max_segundos;
   if (!std::cin) // fin de la entrada
      return false;

   cin >> datos.num_cofres;
   datos.cofres = vector<Cofre>(datos.num_cofres);
   
   for (int i = 0; i < datos.num_cofres; i++)
      cin >> datos.cofres[i].profundidad >> datos.cofres[i].oro;

   algCofres(datos);

   return true;
}

int main()
{
   // ajustes para que cin extraiga directamente de un fichero

#ifndef DOMJUDGE
   std::ifstream in("/home/erik/Documents/University/2020-2021/2nd semester/MAR2/Problemas/05 - El cazatesoros/1.in");
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
