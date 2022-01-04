// Grupo 15 - Erik Karlgren Domercq

/*
   
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <deque>

#include "Matriz.h"

using namespace std;

const int NOT_INITIALIZED = -1;

struct Solucion
{
   int num_of_new_letters;
   string new_word;
};

int aibofobia(const string &word, Matriz<int> &matrix, int i, int j)
{
   int &res = matrix[i][j];

   if (res == NOT_INITIALIZED)
   {
      if (i == j || i > j)
         res = 0;
      else if (word[i] == word[j])
         res = aibofobia(word, matrix, i + 1, j - 1);
      else
         res = min(aibofobia(word, matrix, i + 1, j), aibofobia(word, matrix, i, j - 1)) + 1;
   }
   return res;
}

void rebuildSolution(const string &word, const Matriz<int> &matrix, string &new_word, int i, int j)
{
   if (i <= j)
   {
      if (i == j)
      {
         new_word.push_back(word[i]);
      }
      else if (word[i] == word[j])
      {
         new_word.push_back(word[i]);
         rebuildSolution(word, matrix, new_word, i + 1, j - 1);
         new_word.push_back(word[j]);
      }
      else if (matrix[i][j] == matrix[i + 1][j] + 1)
      {
         new_word.push_back(word[i]);
         rebuildSolution(word, matrix, new_word, i + 1, j);
         new_word.push_back(word[i]);
      }
      else // matrix[i][j] == matrix[i][j-1] + 1
      {
         new_word.push_back(word[j]);
         rebuildSolution(word, matrix, new_word, i, j - 1);
         new_word.push_back(word[j]);
      }
   }
}

void solve(const string &word)
{
   Solucion sol = {.num_of_new_letters = 0, .new_word = ""};
   Matriz<int> matrix(word.size(), word.size(), NOT_INITIALIZED);

   sol.num_of_new_letters = aibofobia(word, matrix, 0, word.size() - 1);
   //cout << matrix;
   rebuildSolution(word, matrix, sol.new_word, 0, word.size() - 1);

   cout << sol.num_of_new_letters << ' ' << sol.new_word << endl;
   //cout << endl;

   if (sol.num_of_new_letters != sol.new_word.size() - word.size())
      cerr << ">>> Error: the new word (" << sol.new_word << ") has an unexpected size" << endl;
}

bool resuelveCaso()
{
   string word;
   // leer los datos de la entrada
   cin >> word;

   if (!std::cin) // fin de la entrada
      return false;

   solve(word);

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
