// Daniela Alejandra Cordova Porta

// Resolvemos el problema usando programacion dinamica.
//Primero lo resolvemos haciendo uso de una matriz que es recorrida de arriba a abajo
//y de izquierda a derecha. Luego realizamos lo mismo en un vector
//pero este es recorrido de derecha a izquierda

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

#include "Matriz.h" // propios o los d  e las estructuras de datos de clase
#include "EnterosInf.h"

struct Datos {
    long long int longitud, costo;
};

struct Solucion {
    bool sol;
    long long int cant, min, costo;
};
// función que resuelve el problema
// comentario sobre el coste, O(f(N)), donde N es ...

// O(n*L)
//Recorremos la matriz M de booleanos de arriba a abajo y de izquierda a derecha.
//La solucion final estaria en M[n][L]
//El coste espacial esta en O(n*L). Podria optimizarce para que sea lineal O(L) utilizando un vector
//ya que siempre se accede a la fila i-1. Esta optimizacion se hace en la funcion resolverRecursion1Optimizado
bool resolverRecursion1(vector<Datos> const &vector_datos, int n, int L) {
    Matriz<bool> matriz(n + 1, L + 1, false);

    matriz[0][0] = true;
    for (int c = 1; c <= n; c++) {

        matriz[c][0] = true;
        for (int l = 1; l <= L; ++l) {
            if (l < vector_datos[c - 1].longitud)
                matriz[c][l] = matriz[c - 1][l];
            else
                matriz[c][l] = matriz[c - 1][l] || matriz[c - 1][l - vector_datos[c - 1].longitud];
        }
    }
    return matriz[n][L];
}

//En este caso recorremos el vector de derecha a izquierda ya que necesitamos los valores
//de la fila anterior
bool resolverRecursion1Optimizado(vector<Datos> const &v, int n, int L) {
    vector<bool> M(L + 1, false);
    M[0] = true;

    for (int i = 1; i <= n; i++)
        for (int j = L; j >= v[i - 1].longitud; --j)
            M[j] = M[j] || M[j - v[i - 1].longitud];

    return M[L];
}

//-----------------------------------------------------------------------------------------------------------------------------------------

// O(n*L)
//Recorremos la matriz M de enteros de arriba a abajo y de izquierda a derecha.
//La solucion final estaria en M[n][L]
//El coste espacial esta en O(n*L). Podria optimizarce para que sea lineal O(L) utilizando un vector
//ya que siempre se accede a la fila i-1. Esta optimizacion se hace en la funcion resolverRecursion1Optimizado
long long int resolverRecursion2(vector<Datos> const &vector_datos, int n, int L) {
    Matriz<long long int> matriz(n + 1, L + 1, 0);

    matriz[0][0] = 1;

    for (int c = 1; c <= n; c++) {
        matriz[c][0] = 1;
        for (int l = 1; l <= L; ++l) {
            if (l < vector_datos[c - 1].longitud)
                matriz[c][l] = matriz[c - 1][l];
            else
                matriz[c][l] = matriz[c - 1][l] + matriz[c - 1][l - vector_datos[c - 1].longitud];
        }
    }
    return matriz[n][L];
}

//En este caso recorremos el vector de derecha a izquierda ya que necesitamos los valores
//de la fila anterior
long long int resolverRecursion2Optimizado(vector<Datos> const &v, int n, int L) {
    vector<long long int> M(L + 1, 0);
    M[0] = 1;

    for (int i = 1; i <= n; i++)
        for (int j = L; j >= v[i - 1].longitud; --j)
            M[j] = M[j] + M[j - v[i - 1].longitud];

    return M[L];
}


//-----------------------------------------------------------------------------------------------------------------------------------------

// O(n*L)
//Recorremos la matriz M de enteros de arriba a abajo y de izquierda a derecha.
//La solucion final estaria en M[n][L]
//El coste espacial esta en O(n*L). Podria optimizarce para que sea lineal O(L) utilizando un vector
//ya que siempre se accede a la fila i-1. Esta optimizacion se hace en la funcion resolverRecursion1Optimizado
long long int resolverRecursion3(vector<Datos> const &v, int n, int L) {
    Matriz<long long int> M(n + 1, L + 1, Infinito._intInf);
    M[0][0] = 0;
    for (int i = 1; i <= n; i++) {

        M[i][0] = 0;
        for (int j = 1; j <= L; ++j) {
            if (j < v[i - 1].longitud)
                M[i][j] = M[i - 1][j];
            else
                M[i][j] = min(M[i - 1][j], 1 + M[i - 1][j - v[i - 1].longitud]);
        }
    }
    return M[n][L];
}

//En este caso recorremos el vector de derecha a izquierda ya que necesitamos los valores
//de la fila anterior
long long int resolverRecursion3Optimizado(vector<Datos> const &v, int n, int L) {
    vector<long long int> M(L + 1, Infinito._intInf);
    M[0] = 0;

    for (int i = 1; i <= n; i++)
        for (int j = L; j >= v[i - 1].longitud; --j)
            M[j] = min(M[j], 1 + M[j - v[i - 1].longitud]);

    return M[L];
}


//-----------------------------------------------------------------------------------------------------------------------------------------

// O(n*L)
//Recorremos la matriz M de enteros de arriba a abajo y de izquierda a derecha.
//La solucion final estaria en M[n][L]
//El coste espacial esta en O(n*L). Podria optimizarce para que sea lineal O(L) utilizando un vector
//ya que siempre se accede a la fila i-1. Esta optimizacion se hace en la funcion resolverRecursion1Optimizado
long long int resolverRecursion4(vector<Datos> const &v, int n, int L) {
    Matriz<long long int> M(n + 1, L + 1, Infinito._intInf);
    M[0][0] = 0;
    for (int i = 1; i <= n; i++) {

        M[i][0] = 0;
        for (int j = 1; j <= L; ++j) {
            if (j < v[i - 1].longitud)
                M[i][j] = M[i - 1][j];
            else
                M[i][j] = min(M[i - 1][j], v[i - 1].costo + M[i - 1][j - v[i - 1].longitud]);
        }
    }
    return M[n][L];
}

//En este caso recorremos el vector de derecha a izquierda ya que necesitamos los valores
//de la fila anterior
long long int resolverRecursion4Optimizado(vector<Datos> const &v, int n, int L) {
    vector<long long int> M(L + 1, Infinito._intInf);
    M[0] = 0;

    for (int i = 1; i <= n; i++)
        for (int j = L; j >= v[i - 1].longitud; --j)
            M[j] = min(M[j], v[i - 1].costo + M[j - v[i - 1].longitud]);

    return M[L];
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//TODO UNIDO: En un struct

// O(n*L)
//Recorremos la matriz M de enteros de arriba a abajo y de izquierda a derecha.
//La solucion final estaria en M[n][L]
//El coste espacial esta en O(n*L). Podria optimizarce para que sea lineal O(L) utilizando un vector
//ya que siempre se accede a la fila i-1. Esta optimizacion se hace en la funcion resolverRecursion1Optimizado


Solucion resolver2(vector<Datos> const &v, int n, int L) {
    Matriz<Solucion> M(n + 1, L + 1, {false, 0, Infinito._intInf, Infinito._intInf});
    M[0][0].cant = 1;
    M[0][0].sol = true;
    M[0][0].min = 0;
    M[0][0].costo = 0;
    for (int i = 1; i <= n; i++) {
        M[i][0].cant = 1;
        M[i][0].sol = true;
        M[i][0].min = 0;
        M[i][0].costo = 0;
        for (int j = 1; j <= L; ++j) {
            if (j < v[i - 1].longitud) {
                M[i][j] = M[i - 1][j];

            } else {
                M[i][j].sol = (M[i - 1][j].sol || M[i - 1][j - v[i - 1].longitud].sol);
                M[i][j].cant = M[i - 1][j].cant + M[i - 1][j - v[i - 1].longitud].cant;
                M[i][j].min = min(M[i - 1][j].min, M[i - 1][j - v[i - 1].longitud].min + 1);
                M[i][j].costo = min(M[i - 1][j].costo, M[i - 1][j - v[i - 1].longitud].costo + v[i - 1].costo);

            }
        }
    }


    return M[n][L];
}

//En este caso recorremos el vector de derecha a izquierda ya que necesitamos los valores
//de la fila anterior
Solucion resolver3(vector<Datos> const &v, int n, int L) {
    vector<Solucion> M(L + 1, {false, 0, Infinito._intInf, Infinito._intInf});
    M[0].cant = 1;
    M[0].sol = true;
    M[0].min = 0;
    M[0].costo = 0;
    for (int i = 1; i <= n; i++) {
        M[0].cant = 1;
        M[0].sol = true;
        M[0].min = 0;
        M[0].costo = 0;
        for (int j = L; j >= v[i - 1].longitud; --j) {

            M[j].cant = M[j].cant + M[j - v[i - 1].longitud].cant;
            M[j].sol = M[j].sol || M[j - v[i - 1].longitud].sol;
            M[j].min = min(M[j].min, 1 + M[j - v[i - 1].longitud].min);
            M[j].costo = min(M[j].costo, v[i - 1].costo + M[j - v[i - 1].longitud].costo);
        }

    }

    return M[L];
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {

    // leer los datos de la entrada
    int n, L;
    cin >> n;

    if (!std::cin)  // fin de la entrada
        return false;


    cin >> L;

    vector<Datos> v;
    int len, c;
    for (int i = 0; i < n; i++) {
        cin >> len >> c;
        v.push_back({len, c});
    }

    Solucion sol = resolver2(v, n, L);

    if (sol.sol)
        cout << "SI " << sol.cant << " " << sol.min << " " << sol.costo << endl;
    else
        cout << "NO" << endl;
    // escribir sol

    return true;

}

int main() {
    // ajustes para que cin extraiga directamente de un fichero

#ifndef DOMJUDGE
    std::ifstream in("casos.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

    while (resuelveCaso());

    // para dejar todo como estaba al principio
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif


    return 0;
}


