
// Nombre y Apellidos: Erik Karlgren Domercq

// Comentario general sobre la solución,
// explicando cómo se resuelve el problema

/*

*/

#include <iostream>
#include <fstream>

#include "EnterosInf.h"

//#define ALG_CON_MATRICES

#ifndef ALG_CON_MATRICES
#define ALG_CON_VECTORES
#include <vector>
#else
#include "Matriz.h"
#endif

//#define DEBUG

using Solucion = struct
{
   bool hay_solucion;
   EntInf coste_min;
   EntInf potencia;
};

using Datos = struct
{
   int num_tipos_bombillas;
   int pot_min;
   int pot_max;
   std::vector<int> pot_bombillas;
   std::vector<int> coste_bombillas;
};

// Más abajo está la solución usando vectores unidimensionales, que tiene un coste menor en espacio.
#ifdef ALG_CON_MATRICES

#ifdef DEBUG
void mostrarMatriz(const Matriz<EntInf> &m_costes, const Datos &datos)
{
   std::cerr << "\nPmin: " << datos.pot_min << "\tPmax: " << datos.pot_max << std::endl;
   for (int i = 0; i <= datos.num_tipos_bombillas; i++)
   {
      for (int p = 0; p <= datos.pot_min; p++)
      {
         std::cerr << m_costes[i][p] << " \t";
      }
      std::cerr << std::endl;
   }
}
#endif

/*
   Sean N el número de tipos de bombillas y Pmax la potencia máxima necesaria.

   # Coste en tiempo: O(N * Pmax)
   Para cada tipo de bombilla i (1 <= i <= N) calculamos el coste mínimo necesario para alcanzar una potencia p
   (1 <= p <= Pmax), y para hacerlo para cada pareja (i,p) invertimos un tiempo constante. Por tanto, el coste 
   final en tiempo es de O(N * Pmax);

   # Coste en espacio: O(N * Pmax)
   Creamos 2 matrices con N+1 filas y Pmax+1 columnas cada una, así que el coste en espacio es claramente O(N * Pmax).
   El coste en espacio se podría mejorar usando 2 vectores de Pmax+1 posiciones, lo que reduciría el coste a O(Pmax),
   ya que solo llegamos a consultar un valor de la fila anterior cada para cada tipo de bombilla i y potencia p
   (1 <= i <= N, 1 <= p <= Pmax).
*/
Solucion resolver(const Datos &datos)
{
   Solucion sol = {.hay_solucion = false, .coste_min = Infinito, .potencia = Infinito};
   // Matriz con el coste mínimo necesario para cada potencia p usando los tipos de bombilla de la 1 a la i
   Matriz<EntInf> mat(datos.num_tipos_bombillas + 1, datos.pot_max + 1, Infinito);

   // casos base: potencia mínima es 0
   for (int i = 0; i <= datos.num_tipos_bombillas; i++)
      mat[i][0] = 0;

   // Rellenamos la matriz con las soluciones
   for (int i = 1; i <= datos.num_tipos_bombillas; i++)
      for (int p = 1; p <= datos.pot_max; p++)
      {
         int ind_anterior = p - datos.pot_bombillas[i];
         if (p < datos.pot_bombillas[i])
            mat[i][p] = mat[i - 1][p];
         else
            mat[i][p] = std::min(mat[i - 1][p],
                                 mat[i][ind_anterior] + datos.coste_bombillas[i]);
      }

   // Reconstruimos la solución
   for (int p = datos.pot_min; p <= datos.pot_max; p++)
      if (mat[datos.num_tipos_bombillas][p] < sol.coste_min)
      {
         sol.coste_min = mat[datos.num_tipos_bombillas][p];
         sol.potencia = p;
      }

   sol.hay_solucion = sol.coste_min != Infinito;

#ifdef DEBUG
   mostrarMatriz(m_costes, datos);
#endif

   return sol;
}
#endif

#ifdef ALG_CON_VECTORES
/*
   Sean N el número de tipos de bombillas y Pmax la potencia máxima necesaria.

   # Coste en tiempo: O(N * Pmax)
   Para cada tipo de bombilla i (1 <= i <= N) calculamos el coste mínimo necesario para alcanzar una potencia p
   (1 <= p <= Pmax), y para hacerlo para cada pareja (i,p) invertimos un tiempo constante. Por tanto, el coste 
   final en tiempo es de O(N * Pmax);


   # Coste en espacio: O(Pmax)
   Creamos un vector de Pmax+1 elementos, así que el coste en espacio es claramente O(Pmax).
*/
Solucion resolver(const Datos &datos)
{
   Solucion sol = {.hay_solucion = false, .coste_min = Infinito, .potencia = Infinito};
   std::vector<EntInf> v_costes(datos.pot_max + 1, Infinito);

   // casos base: potencia mínima es 0, así que coste 0
   v_costes[0] = 0;

   // Rellenamos el vector con las soluciones
   for (int i = 1; i <= datos.num_tipos_bombillas; i++)
      for (int p = 1; p <= datos.pot_max; p++)
      {
         int ind_anterior = p - datos.pot_bombillas[i];
         if (p >= datos.pot_bombillas[i])
            v_costes[p] = std::min(v_costes[p],
                                   v_costes[ind_anterior] + datos.coste_bombillas[i]);
      }

   // Reconstruimos la solución
   for (int p = datos.pot_min; p <= datos.pot_max; p++)
      if (v_costes[p] < sol.coste_min)
      {
         sol.coste_min = v_costes[p];
         sol.potencia = p;
      }

   sol.hay_solucion = sol.coste_min != Infinito;

   return sol;
}

#endif

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso()
{
   Datos datos;
   Solucion sol;

   // leer los datos de la entrada
   std::cin >> datos.num_tipos_bombillas;

   if (!std::cin) // fin de la entrada
      return false;

   std::cin >> datos.pot_max >> datos.pot_min;

   datos.coste_bombillas = std::vector<int>(datos.num_tipos_bombillas + 1);
   datos.pot_bombillas = std::vector<int>(datos.num_tipos_bombillas + 1);

   datos.pot_bombillas[0] = 0;
   for (int i = 1; i <= datos.num_tipos_bombillas; i++)
      std::cin >> datos.pot_bombillas[i];

   datos.coste_bombillas[0] = 0;
   for (int i = 1; i <= datos.num_tipos_bombillas; i++)
      std::cin >> datos.coste_bombillas[i];

   sol = resolver(datos);

   // escribir sol
   if (sol.hay_solucion)
      std::cout << sol.coste_min << ' ' << sol.potencia << std::endl;
   else
      std::cout << "IMPOSIBLE" << std::endl;

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
   // system("PAUSE");
#endif
   return 0;
}
