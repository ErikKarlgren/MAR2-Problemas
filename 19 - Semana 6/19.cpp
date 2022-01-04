// Grupo 15 -

/*

*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

/* Representa los datos de una soluci�n parcial para el problema. */
struct Nodo {
    int duracion_restante[2];
    int ult_cancion;        // �ltimo trabajador que hemos considerado hasta el momento
    int beneficio_real;        // beneficio de la soluci�n parcial
    double coste_optimista;// beneficio en el mejor de los casos
    bool operator<(Nodo const &otro) const { return otro.coste_optimista > coste_optimista; }
};

struct DatosCancion {
    int duracion;
    int puntos;

    bool operator<(DatosCancion const &otro) const {
        return (puntos / (double) duracion) > (otro.puntos / (double) otro.duracion);
    }
};

/* Calcula el coste optimista estimado */
double costeOptimistaEstimado(const vector<DatosCancion> &canciones, int duracion_cara, const Nodo &nodo) {
    int hueco = nodo.duracion_restante[0] + nodo.duracion_restante[1];
    double optimo = nodo.beneficio_real;
    int i = nodo.ult_cancion + 1;
    int num_canciones = canciones.size();

    while (i < num_canciones && canciones[i].duracion < hueco) {
        hueco -= canciones[i].duracion;
        optimo += (double) canciones[i].puntos;
        i++;
    }

    if (i < num_canciones)
        optimo += (hueco / (double) canciones[i].duracion) * canciones[i].puntos;

    return optimo;
}

/* Calcula el coste pesimista estimado */
int costePesimistaEstimado(const vector<DatosCancion> &canciones, int duracion_cara, const Nodo &nodo) {
    int tiempo_ocupado[2] = {0, 0};
    tiempo_ocupado[0] = duracion_cara - nodo.duracion_restante[0];
    tiempo_ocupado[1] = duracion_cara - nodo.duracion_restante[1];
    int pesimista = nodo.beneficio_real;
    int i = nodo.ult_cancion + 1;
    int num_canciones = canciones.size();

    while (i < num_canciones && (tiempo_ocupado[0] < duracion_cara || tiempo_ocupado[1] < duracion_cara)) {
        if (tiempo_ocupado[0] + canciones[i].duracion <= duracion_cara) {
            tiempo_ocupado[0] += canciones[i].duracion;
            pesimista += canciones[i].puntos;
        } else if (tiempo_ocupado[1] + canciones[i].duracion <= duracion_cara) {
            tiempo_ocupado[1] += canciones[i].duracion;
            pesimista += canciones[i].puntos;
        }
        i++;
    }
    return pesimista;
}

void addNode(priority_queue<Nodo> &queue, const Nodo &nodo, vector<int> &mejor_sol,
             int num_canciones, double &mejor_coste, int pesimista) {
    if (nodo.coste_optimista >= mejor_coste) {
        if (nodo.ult_cancion == num_canciones - 1) {
            mejor_coste = nodo.beneficio_real;
        } else {
            queue.push(nodo);
            if (pesimista > mejor_coste)
                mejor_coste = (double) pesimista;
        }
    }
}

int resolver(int num_canciones, int duracion_cara, const vector<DatosCancion> &canciones) {
    double mejor_coste; // Es double para poder comparar con la cota optimista, que es también real
    int pesimista;
    vector<int> mejor_sol;

    priority_queue<Nodo> queue;

    Nodo primer_nodo;
    primer_nodo.duracion_restante[0] = duracion_cara;
    primer_nodo.duracion_restante[1] = duracion_cara;
    primer_nodo.ult_cancion = -1;
    primer_nodo.beneficio_real = 0;
    primer_nodo.coste_optimista = costeOptimistaEstimado(canciones, duracion_cara, primer_nodo);

    pesimista = costePesimistaEstimado(canciones, duracion_cara, primer_nodo);
    // mejor_coste tendr� el peor coste posible en cada momento, y al terminar el algoritmo tendr� el mejor coste posible
    mejor_coste = pesimista;

    queue.push(primer_nodo);

    while (!queue.empty() && queue.top().coste_optimista >= mejor_coste) {
        const Nodo top = queue.top();
        queue.pop();
        Nodo nodo(top);
        nodo.ult_cancion = top.ult_cancion + 1;

        // meter en cara 1
        if (top.duracion_restante[0] >= canciones[top.ult_cancion + 1].duracion) {
            nodo.beneficio_real = top.beneficio_real + canciones[nodo.ult_cancion].puntos;
            nodo.duracion_restante[0] = top.duracion_restante[0] - canciones[nodo.ult_cancion].duracion;
            nodo.duracion_restante[1] = top.duracion_restante[1];
            nodo.coste_optimista = top.coste_optimista; // misma cota optimista porque ponemos la cancion

            // 'pesimista' no varía porque estamos poniendo la canción en la primera cara
            addNode(queue, nodo, mejor_sol, num_canciones, mejor_coste, pesimista);
        }

        // meter en cara 2
        if (top.duracion_restante[1] >= canciones[top.ult_cancion + 1].duracion &&
            top.duracion_restante[1] != top.duracion_restante[0]) {
            nodo.beneficio_real = top.beneficio_real + canciones[nodo.ult_cancion].puntos;
            nodo.duracion_restante[0] = top.duracion_restante[0];
            nodo.duracion_restante[1] = top.duracion_restante[1] - canciones[nodo.ult_cancion].duracion;
            nodo.coste_optimista = top.coste_optimista; // misma cota optimista porque ponemos la cancion

            pesimista = costePesimistaEstimado(canciones, duracion_cara, nodo);
            addNode(queue, nodo, mejor_sol, num_canciones, mejor_coste, pesimista);
        }

        // no meter
        nodo.beneficio_real = top.beneficio_real;
        nodo.duracion_restante[0] = top.duracion_restante[0];
        nodo.duracion_restante[1] = top.duracion_restante[1];
        nodo.coste_optimista = costeOptimistaEstimado(canciones, duracion_cara, nodo);

        pesimista = costePesimistaEstimado(canciones, duracion_cara, nodo);
        addNode(queue, nodo, mejor_sol, num_canciones, mejor_coste, pesimista);
    }
    return (int) mejor_coste;
}

bool resuelveCaso() {
    int num_canciones;
    int duracion_cara;
    vector<DatosCancion> canciones;

    cin >> num_canciones >> duracion_cara;
    if (num_canciones == 0)
        return false;

    for (int i = 0; i < num_canciones; i++) {
        DatosCancion c;
        cin >> c.duracion >> c.puntos;
        canciones.push_back(c);
    }
    sort(canciones.begin(), canciones.end());

    cout << resolver(num_canciones, duracion_cara, canciones) << endl;

    return true;
}

int main() {
    // ajustes para que cin extraiga directamente de un fichero

#ifndef DOMJUDGE
    std::ifstream in("/home/erik/Documents/University/2020-2021/2nd semester/MAR2/Problemas/19 - Semana 6/1.in");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

    while (resuelveCaso());

// para dejar todo como estaba al principio
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
#endif

    return 0;
}
