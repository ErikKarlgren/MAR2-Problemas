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
#include "Matriz.h" // propios o los de las estructuras de datos de clase
//#include "EnterosInf.h"


struct Solucion
{
	int suma;
	int col_inicial;
};

struct Datos
{
	int n;
	Matriz<int>  tablero;
};

int maximo(int a, int b, int c)
{
	return max(a, max(b, c));
}

Solucion tablero(const Datos& datos)
{
	Matriz<int> M(datos.n, datos.n, 0);
	Solucion sol = { 0,datos.n };

	for (int j = 0; j < datos.n; j++)
	{
		M[0][j] = datos.tablero[0][j];
	}

	for (int i = 1; i < datos.n; i++)
	{
		M[i][0] = datos.tablero[i][0] + max(M[i - 1][0], M[i - 1][1]);
		M[i][datos.n - 1] = datos.tablero[i][datos.n - 1] + max(M[i - 1][datos.n - 1], M[i - 1][datos.n - 2]);

		for (int j = 1; j < datos.n - 1; j++)
		{
			M[i][j] = datos.tablero[i][j] + maximo(M[i - 1][j - 1], M[i - 1][j], M[i - 1][j + 1]);
		}
	}

	for (int j = 0; j < datos.n; j++)
	{
		if (M[datos.n - 1][j] > sol.suma)
		{
			sol.suma = M[datos.n - 1][j];
			sol.col_inicial = j;
		}
	}
	sol.col_inicial ++;
	return sol;
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {

	// leer los datos de la entrada
	int n;
	cin >> n;

	if (!std::cin)  // fin de la entrada
		return false;
	

		Datos d;
		d.n = n;
		int num;
		d.tablero= Matriz<int> (n, n, 0);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++) {
			cin >> num;
			d.tablero[i][j]=num;
			}
		}

		Solucion sol = tablero(d);

		cout << sol.suma << " " << sol.col_inicial << endl;
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