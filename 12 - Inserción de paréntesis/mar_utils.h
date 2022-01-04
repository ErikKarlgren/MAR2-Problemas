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