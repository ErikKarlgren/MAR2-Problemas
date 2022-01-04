// Grupo 15 - Erik Karlgren Domercq

/*
   # DEFINICIONES Y ALGORITMO

   Sean las siguientes definiciones:

      num_cortes
         Número de cortes
         0 <= num_cortes

      longitud
         Longitud total del tablón
         0 < longitud
   
      esfuerzo(longitud, i, j)
         Esfuerzo necesario para hacer un solo corte si tenemos disponibles los cortes [i..j].
         La longitud del tablero se puede calcular a partir de la longitud del tablón y los cortes i y j
         0 <= i < num_cortes, 0 <= j < num_cortes

      tablon(i, j)
         Mínimo esfuerzo necesario para cortar un tablón teniendo disponibles los cortes [i..j]
         0 <= i < num_cortes, 0 <= j < num_cortes

   Podemos definir tablon(i,j) recursivamente de la siguiente manera:

      tablon(i, i) = esfuerzo(longitud, i, i)
      tablon(i, j) = esfuerzo(longitud, i, j) + 
         min([tablon(i, j-1),                                      -> cortamos por el último punto de corte disponible
              tablon(i+1, j),                                      -> cortamos por el primer punto de corte disponible
              min(tablon(i, k-1) + tablon(k+1, j) where i < k < j) -> cortamos por otro punto de corte k
            ])
*/

/*
   # COSTE DEL ALGORITMO

   ## COSTE EN ESPACIO ADICIONAL

   Creamos una matriz de num_cortes x num_cortes, por lo que el coste en espacio está en el orden de
   O(num_cortes^2).


   ## COSTE EN TIEMPO

   Para cada casilla que recorremos en la diagonal d, consultamos el valor de 2*d casillas. Por tanto,
   el coste en tiempo del algoritmo es el siguiente:

   coste
   = suma(0 <= d < num_cortes, suma(0 <= i < num_cortes - d, 2*d))
   = suma(0 <= d < num_cortes, 2 * d * (num_cortes - d)))
   = 2 * suma(0 <= d < num_cortes, d * (num_cortes - d)))
   = 2 * (num_cortes * (num_cortes - 1) * (num_cortes + 1)) / 6

   Como coste es un polinomio de grado 3 en función de num_cortes, el coste en tiempo del algoritmo es
   del orden O(num_cortes^3).
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <climits>

#include "Matriz.h"

using namespace std;

const int INFINITO = INT_MAX;

// Esfuerzo para cortar un tablón teniendo todavía disponibles los cortes [corte_ini..corte_fin]
// 0 <= corte_ini < num_cortes
// 0 <= corte_fin < num_cortes
int calc_esfuerzo(int longitud, const vector<int> &puntos_corte, int corte_ini, int corte_fin)
{
   int num_cortes = puntos_corte.size();
   int ini = (corte_ini == 0) ? 0 : puntos_corte[corte_ini - 1];
   int fin = (corte_fin == num_cortes - 1) ? longitud : puntos_corte[corte_fin + 1];
   return 2 * (fin - ini);
}

// Rellenamos por diagonales
void rellenarMatrices(Matriz<int> &m_esfuerzos,
                      int longitud,
                      const vector<int> &puntos_corte)
{
   int num_cortes = puntos_corte.size();

   // Rellenamos la diagonal principal
   for (int i = 0; i < num_cortes; i++)
      m_esfuerzos[i][i] = calc_esfuerzo(longitud, puntos_corte, i, i);

   // Rellenamos el resto de diagonales
   for (int d = 1; d < num_cortes; d++)
      // Recorremos los elementos de la diagonal d (el elemento i-ésimo está en la fila i)
      for (int i = 0; i < num_cortes - d; i++)
      {
         int j = i + d; // j es la columna del elemento i-ésimo de la diagonal
         int esfuerzo = calc_esfuerzo(longitud, puntos_corte, i, j);
         m_esfuerzos[i][j] = INFINITO;

         // k es el índice del punto de corte
         for (int k = i + 1; k < j; k++)
         {
            int temp = m_esfuerzos[i][k - 1] + m_esfuerzos[k + 1][j] + esfuerzo;
            if (temp < m_esfuerzos[i][j]) // Es mejor cortar primero por el corte de índice k
               m_esfuerzos[i][j] = temp;
         }
         // Cortamos por el último punto de corte disponible
         m_esfuerzos[i][j] = min(m_esfuerzos[i][j], m_esfuerzos[i][j - 1] + esfuerzo);
         // Cortamos por el primer punto de corte disponible
         m_esfuerzos[i][j] = min(m_esfuerzos[i][j], m_esfuerzos[i + 1][j] + esfuerzo);
      }
}

void solve(const vector<int> &puntos_corte, int num_cortes, int longitud)
{
   Matriz<int> m_esfuerzos(num_cortes, num_cortes, 0);
   rellenarMatrices(m_esfuerzos, longitud, puntos_corte);
   
   cout << m_esfuerzos;
   
   cout << m_esfuerzos[0][num_cortes - 1] << endl;
}

bool resuelveCaso()
{
   // Longitud del tablón
   int longitud;
   // Numero de cortes
   int num_cortes;
   // Puntos donde cortar. Orden creciente.
   vector<int> puntos_corte;

   cin >> longitud >> num_cortes;
   if (longitud == 0 && num_cortes == 0)
      return false;

   puntos_corte = vector<int>(num_cortes);
   for (int i = 0; i < num_cortes; i++)
      cin >> puntos_corte[i];

   solve(puntos_corte, num_cortes, longitud);

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
