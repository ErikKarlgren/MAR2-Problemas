// Grupo 15 - Erik Karlgren Domercq

/*

*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

#include "mar_utils.h"
#include "Matriz.h"

using namespace std;

const int INITIAL_VALUE = -1;

int vacas(const vector<int> &comida, Matriz<int> &matriz, int i, int j)
{
    if (i > j)
        return 0;

    int &res = matriz[i][j];
    if (res == INITIAL_VALUE)
    {
        if (i == j)
            res = comida[i];

        else if ((comida[i + 1] >= comida[j]) && (comida[i] >= comida[j - 1]))
            res = max(vacas(comida, matriz, i + 2, j) + comida[i],
                      vacas(comida, matriz, i + 1, j - 1) + comida[j]);

        else if ((comida[i + 1] >= comida[j]) && (comida[i] < comida[j - 1]))
            res = max(vacas(comida, matriz, i + 2, j) + comida[i],
                      vacas(comida, matriz, i, j - 2) + comida[j]);

        else if ((comida[i + 1] < comida[j]) && (comida[i] >= comida[j - 1]))
            res = max(vacas(comida, matriz, i + 1, j - 1) + comida[i],
                      vacas(comida, matriz, i + 1, j - 1) + comida[j]);

        else // ((comida[i + 1] < comida[j]) && (comida[i] < comida[j - 1]))
            res = max(vacas(comida, matriz, i + 1, j - 1) + comida[i],
                      vacas(comida, matriz, i, j - 2) + comida[j]);
    }
    return res;
}

int resolver(int num_cubos, const vector<int> &cantidades_comida)
{
    Matriz<int> matriz(num_cubos, num_cubos, INITIAL_VALUE);
    return vacas(cantidades_comida, matriz, 0, num_cubos - 1);
}

bool resuelveCaso()
{
    int num_cubos;
    vector<int> cantidades_comida;

    cin >> num_cubos;
    if (num_cubos == 0)
        return false;
    leerVector(cantidades_comida, num_cubos);

    cout << resolver(num_cubos, cantidades_comida) << endl;

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
