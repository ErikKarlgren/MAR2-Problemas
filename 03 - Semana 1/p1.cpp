#include "Matriz.h"
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

Solucion tablero(const Datos &datos)
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
            sol.col_inicial = j + 1;
        }
    }

    return sol;
}