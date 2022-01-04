
// Nombre y Apellidos
// Erik Karlgren Domercq

// Comentario general sobre la solución,
// explicando cómo se resuelve el problema
/*
 * Primero se comprueba si es posible crear una cuerda de la longitud deseada, y solo si se puede
 * se calculan los siguientes 3 datos: el numero de formas posibles de crear la cuerda deseada,
 * el minimo numero de cuerdas necesario y el coste minimo para formar la cuerda.
 *
 * Para calcular los 4 datos se emplean algoritmos de programacion dinamica ascendente que usan
 * un vector unidimensional para ir construyendo la solucion. Todos funcionan de forma similar, asi
 * que vamos a explicar primero como se deduce que existe solucion.
 *
 * En primer lugar, creamos un vector v de L+1 booleanos, siendo L la longitud de la cuerda que queremos crear.
 * El valor de una posicion 0 <= l <= L indica si podemos crear una cuerda de longitud l con las cuerdas
 * que hemos considerado hasta el momento, es decir, las cuerdas desde 0 a c (0 <= c < N). Por tanto, cuando no
 * hemos considerado ninguna cuerda todavia es trivial ver que solo la cuerda de longitud 0 se puede formar y el
 * resto no.
 *
 * En segundo lugar, para cada cuerda c recorremos el vector de derecha a izquierda desde la ultima posicion,
 * y acabamos al llegar a la posicion l' tal que longitud(c) = l'. Para cada posicion l del vector consultamos
 * la posicion l - l', y actualizamos la posicion l (v[l]) con el valor de v[l] OR v[l-l']. La solucion al
 * problema estara en la ultima posicion del vector tras haber recorrido dicho vector tantas veces como cuerdas
 * tengamos.
 *
 * Este esquema es similar al que siguen los otros 3 algoritmos, pero con otros valores iniciales y operaciones
 * para actualizar los valores del vector.
 */

#include <iostream>
#include <fstream>
#include <vector>

#include "p0.h" // propios o los de las estructuras de datos de clase


bool cuerdaSePuedeFormar(const Datos &datos) {
    std::vector<bool> cuerda_viable(datos.L + 1, false);
    int c = 0;

    cuerda_viable[0] = true;

    while (c < datos.N && !cuerda_viable[datos.L]) {
        for (int l = datos.L; l >= datos.longitudes[c]; l--) {
            int ind_anterior = l - datos.longitudes[c];
            cuerda_viable[l] = cuerda_viable[l] || cuerda_viable[ind_anterior];
        }
        c++;
    }

    /*for (auto n : cuerda_viable) {
        std::cerr << n << ' ';
    }
    std::cerr << std::endl;*/

    return cuerda_viable[datos.L];
}

uint64_t numeroDeManerasDeConseguirCuerda(const Datos &datos) {
    std::vector<uint64_t> formas_posibles(datos.L + 1, 0);
    formas_posibles[0] = 1; // se puede formar una cuerda de longitud de una sola manera: con 0 cuerdas

    for (int c = 0; c < datos.N; c++) {
        for (int l = datos.L; l >= datos.longitudes[c]; l--) {
            int ind_anterior = l - datos.longitudes[c];
            formas_posibles[l] += formas_posibles[ind_anterior];
        }
    }

    /*for (auto n : formas_posibles) {
        std::cerr << n << ' ';
    }
    std::cerr << std::endl;*/

    return formas_posibles[datos.L];
}

uint64_t minimoNumeroPosibleDeCuerdas(const Datos &datos) {
    // El "-1" es para evitar overflow a la hora de sumar 1 en el bucle interno
    std::vector<uint64_t> minimo_numero_cuerdas(datos.L + 1, UINT64_MAX - 1);
    minimo_numero_cuerdas[0] = 0;

    for (int c = 0; c < datos.N; c++) {
        for (int l = datos.L; l >= datos.longitudes[c]; l--) {
            int ind_anterior = l - datos.longitudes[c];
            minimo_numero_cuerdas[l] = std::min(minimo_numero_cuerdas[l], minimo_numero_cuerdas[ind_anterior] + 1);
        }

    }

    /*for (auto n : minimo_numero_cuerdas) {
        std::cerr << n << ' ';
    }
    std::cerr << std::endl;*/

    return minimo_numero_cuerdas[datos.L];
}

uint64_t minimoCosteNecesario(const Datos &datos) {
    // MAX_VALUE es el coste maximo que puede tener una cuerda, con lo que evitamos overflow
    // siempre que vayamos a sumar en el bucle interno
    std::vector<uint64_t> minimo_coste(datos.L + 1, UINT64_MAX - MAX_VALUE);
    minimo_coste[0] = 0;

    for (int c = 0; c < datos.N; c++) {
        for (int l = datos.L; l >= datos.longitudes[c]; l--) {
            int ind_anterior = l - datos.longitudes[c];
            minimo_coste[l] = std::min(minimo_coste[l], minimo_coste[ind_anterior] + datos.costes[c]);
        }
    }

    /*for (auto n : minimo_coste) {
        std::cerr << n << ' ';
    }
    std::cerr << std::endl;*/

    return minimo_coste[datos.L];
}

/*
 * Siendo N el numero de cuerdas y L la longitud de la cuerda deseada, tenemos que los costes en
 * tiempo de los 4 algoritmos usados pertenecen a O(N*L) debido a los 2 bucles anidados usados,
 * mientras que el coste en memoria pertenece a O(L) por usar un vector de L+1 elementos.
 */
Solucion resolver(const Datos &datos) {
    Solucion sol;

    if (cuerdaSePuedeFormar(datos)) {
        sol.hay_solucion = true;
        sol.maneras_de_conseguir_cuerda = numeroDeManerasDeConseguirCuerda(datos);
        sol.minimo_numero_de_cuerdas = minimoNumeroPosibleDeCuerdas(datos);
        sol.minimo_coste_posible = minimoCosteNecesario(datos);
    } else {
        sol.hay_solucion = false;
    }

    return sol;
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    Datos datos;

    // leer los datos de la entrada
    std::cin >> datos.N >> datos.L;

    if (!std::cin) // fin de la entrada
        return false;

    datos.longitudes = std::vector<int>((std::size_t) datos.N);
    datos.costes = std::vector<int>((std::size_t) datos.N);

    for (int i = 0; i < datos.N; i++) {
        int longitud;
        int coste;
        std::cin >> longitud >> coste;
        datos.longitudes[i] = longitud;
        datos.costes[i] = coste;
    }

    Solucion sol = resolver(datos);

    // escribir sol
    if (sol.hay_solucion) {
        std::cout << "SI "
                  << sol.maneras_de_conseguir_cuerda << " "
                  << sol.minimo_numero_de_cuerdas << " "
                  << sol.minimo_coste_posible << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }

    return true;
}

int main() {
    // ajustes para que cin extraiga directamente de un fichero
#ifndef DOMJUDGE
    std::ifstream in("/home/erik/Documents/University/2020-2021/2nd semester/MAR2/Problemas/Problema 0/1.in");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

    while (resuelveCaso());

    // para dejar todo como estaba al principio
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    // system("PAUSE");
#endif
    return 0;
}
