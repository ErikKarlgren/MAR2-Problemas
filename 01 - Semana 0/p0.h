#include <vector>
#include <cstdint>

const int MIN_VALUE = 1;
const int MAX_VALUE = 1000;

struct Datos {
    // Longitud buscada
    int L;
    // Numero de cuerdas
    int N;
    // Longitud de cada cuerda
    std::vector<int> longitudes;
    // Costes de cada cuerda
    std::vector<int> costes;
};

struct Solucion {
    bool hay_solucion;
    uint64_t maneras_de_conseguir_cuerda;
    uint64_t minimo_numero_de_cuerdas;
    uint64_t minimo_coste_posible;
};