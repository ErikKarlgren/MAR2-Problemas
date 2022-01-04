#include <vector>
#include <iostream>
#include <fstream>

/*
    Crea un vector de tamaño size y lee de la entrada estándar
    dicha cantidad de valores.
*/
template <typename T>
void leerVector(std::vector<T> &v, int size)
{
    v = std::vector<T>(size);
    for (int i = 0; i < size; i++)
        std::cin >> v[i];
}

template <typename T>
void mostrarVector(const std::vector<T> &v, const std::string &encabezado)
{
    std::cout << encabezado;
    for (auto x : v)
        std::cout << x << ' ';
    std::cout << std::endl;
}

#ifdef MATRIZ_H
/*
    Crea una matriz de un número de filas rows y un número de
    columnas cols a partir de la entrada estándar.
*/
template <typename T>
void leerMatriz(Matriz<T> &m, int rows, int cols)
{
    m = Matriz<T>(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            std::cin >> m[i][j];
}
#endif