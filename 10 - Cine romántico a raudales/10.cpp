// Grupo 15 - Erik Karlgren Domercq

/*
   Sean las siguientes definiciones:
   - N (o num_pelis): número de películas
   - D (o DESCANSO): tiempo en minutos de descanso entre películas (D = 10).
   - T (o MAX_TPO): número de minutos en 1 día más un tiempo de descanso por si alguna peli
   acabara a las 12 de la noche exactamente (T = 24 * 60 + D).
   - h_ini(i) y duracion(i) son, respectivamente, la hora de inicio y la duracion de la
   peli i (0 <= i <= N).

   Sea cines(i, j) el máximo de minutos que se está viendo una película al considerar
   las pelis desde 1 a i que podemos ver antes de la hora j (en minutos) (0 <= i <= N,
   0 <= j < T). Podemos definirlo de forma recursiva de la siguiente forma:

   Casos base:
      cines(i, 0) = 0         0 <= i <= N
      cines(0, j) = 0         0 <= j < T

   Casos recursivos:
      cines(i, j) = cines(i-1, j)                              si h_ini(i) + duracion(i) + D < j
      cines(i, j) = max(cines(i-1, j),
                        cines(i-1, h_ini(i)) + duracion(i))    en caso contrario

   Podemos usar un algoritmo basado en programación dinámica ascendente para conseguir la solución.
   Podríamos usar una matriz de (N+1) x T casillas e ir rellenándola con todas las soluciones a los subproblemas.
   No obstante, como la solución para cada fila i (0 < i <= N) solo depende de la fila anterior (la i-1), nos basta 
   usar solo un vector de T posiciones que iremos rellenando de izquierda a derecha.

   Coste en espacio adicional:
   Como usamos un vector de T posiciones y T es constante, el coste en espacio adicional es de O(T) = O(1), aunque 
   es una constante no demasiado pequeña.

   Coste en tiempo:
   Como tenemos que resolver el subproblema cines(i, j) para todo i y j tal que 0 <= i <= N y 0 <= j < T, tenemos
   una cantidad de subproblemas del orden de O(N * T) = O(N), y todos se resuelven con tiempo constante por lo que
   el coste en tiempo es de O(N).

   No obstante, necesitamos que las películas estén ordenadas crecientemente por hora de inicio. Usamos la función
   std::sort() sobre el vector con los datos de las películas, que tendría un coste de O(N * log N). Por tanto, el
   coste finalmente es de O(N * log N).
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

//#define DEBUG

#ifdef DEBUG
#include <iomanip>
#endif

using namespace std;

// Minutos de descanso entre cada película
const int DESCANSO = 10;
// Hora límite a la que puede acabar una película, incluyendo descanso después de la peli
const int MAX_TIEMPO = 24 * 60 + DESCANSO;
// Minutos en una hora
const int MINUTOS_HORA = 60;
// Estructura con los datos de una película (hora de inicio y duración).
struct DatosPeli
{
   // Hora de inicio en minutos de la película (0 <= hora_inicio < MAX_TIEMPO)
   int hora_inicio;
   // Duración en minutos de la película
   int duracion;
   bool operator<(const DatosPeli &dp) const { return hora_inicio < dp.hora_inicio; }
};

DatosPeli crearPeli(int hora, int minuto, int duracion)
{
   DatosPeli d = {.hora_inicio = hora * MINUTOS_HORA + minuto, .duracion = duracion};
   return d;
}

int resolver(const vector<DatosPeli> &peliculas)
{
   vector<int> cines(MAX_TIEMPO + 1, 0);

#ifdef DEBUG
   for (int i = 0; i < MAX_TIEMPO; i++)
      cout << setw(4) << i << ' ';
   cout << endl;
#endif

   for (auto peli : peliculas)
   {
      for (int j = peli.hora_inicio + peli.duracion + DESCANSO; j < MAX_TIEMPO + 1; j++)
         cines[j] = max(cines[j],
                        cines[peli.hora_inicio] + peli.duracion);

#ifdef DEBUG
      for (auto n : cines)
         cout << setw(4) << n << ' ';
      cout << endl;
#endif
   }
   return cines.back();
}

bool resuelveCaso()
{
   // Peliculas ordenadas por hora a la que acaban incluyendo un descanso
   vector<DatosPeli> peliculas;
   // Número de películas
   int num_pelis;
   // Número máximo de minutos que podemos estar viendo películas
   int mins_viendo_peliculas;

   cin >> num_pelis;
   if (num_pelis == 0)
      return false;

   peliculas = vector<DatosPeli>(num_pelis);
   for (int i = 0; i < num_pelis; i++)
   {
      int hora, minuto, duracion;
      char c;
      cin >> hora >> c >> minuto >> duracion;
      peliculas[i] = crearPeli(hora, minuto, duracion);
   }

   sort(peliculas.begin(), peliculas.end());

#ifdef DEBUG
   cout << "Películas:" << endl;
   for (auto p : peliculas)
      cout << "Hora inicio: " << p.hora_inicio
           << "\t Duracion: " << p.duracion
           << "\tHora final: " << p.hora_inicio + p.duracion << endl;
#endif

   mins_viendo_peliculas = resolver(peliculas);
   cout << mins_viendo_peliculas << endl;

   return true;
}

int main()
{
   // ajustes para que cin extraiga directamente de un fichero

#ifndef DOMJUDGE
   std::ifstream in("2.in");
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
