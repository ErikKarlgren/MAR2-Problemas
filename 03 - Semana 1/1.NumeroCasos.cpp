
// Nombre y Apellidos
// Erik Karlgren Domercq

// Comentario general sobre la solución,
// explicando cómo se resuelve el problema

#include <iostream>
#include <fstream>
//#include <...>

#include "Matriz.h" // propios o los de las estructuras de datos de clase
using namespace std;

struct Solucion
{
   int suma;
   int col_inicial;
};

struct Datos
{
   int n;
   Matriz<int> tablero;
};

int maximo(int a, int b, int c)
{
   return max(a, max(b, c));
}

// función que resuelve el problema
// comentario sobre el coste, O(f(N)), donde N es ...
Solucion resolver(Datos datos)
{
   Matriz<int> M(datos.n, datos.n, 0);
    Solucion sol = {.suma = 0, .col_inicial = datos.n};

    // Inicializa la primera fila de la matriz con los valores de la primera fila del tablero
    for (int j = 0; j < datos.n; j++)
        M[0][j] = datos.tablero[0][j];

    // Rellena la matriz usando los valores de la anterior fila
    for (int i = 1; i < datos.n; i++)
    {
        M[i][0] = datos.tablero[i][0] + max(M[i - 1][0], M[i - 1][1]);
        M[i][datos.n - 1] = datos.tablero[i][datos.n - 1] + max(M[i - 1][datos.n - 1], M[i - 1][datos.n - 2]);

        for (int j = 1; j < datos.n - 1; j++)
            M[i][j] = datos.tablero[i][j] + maximo(M[i - 1][j - 1], M[i - 1][j], M[i - 1][j + 1]);
    }

    // Busca el valor maximo de la ultima fila de la matriz
    for (int j = 0; j < datos.n; j++)
    {
        if (M[datos.n - 1][j] > sol.suma)
        {
            sol.suma = M[datos.n - 1][j];
            sol.col_inicial = j;
        }
    }

    return sol;
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
void resuelveCaso()
{
   Datos datos;

   // leer los datos de la entrada
   cin >> datos.n;

   if(!cin)
      return false;

   

   Solucion sol = resolver(datos);

   // escribir solución
}

int main()
{
   // ajustes para que cin extraiga directamente de un fichero
#ifndef DOMJUDGE
   std::ifstream in("casos.txt");
   auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

   int numCasos;
   std::cin >> numCasos;
   for (int i = 0; i < numCasos; ++i)
      resuelveCaso();

      // para dejar todo como estaba al principio
#ifndef DOMJUDGE
   std::cin.rdbuf(cinbuf);
   system("PAUSE");
#endif
   return 0;
}
