#include <clocale> 
#include <iostream> 
#include <climits> 
#include <math.h>
using namespace std;

int testN()
{
  int i;
  while (true)
  {
    cout << "Введите номер канала [0-15]: ";
    cin >> i;
    if ((cin.fail()) or (i < 0) or (i > 15))
    {
      cout << "Некорректный ввод";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << endl;
    }
    else break;
  }
  return i;
}

int testE()
{
  int i;
  while (true)
  {
    cout << "Введите признак ошибки [0-1]: ";
    cin >> i;
    if ((cin.fail()) or (i < 0) or (i > 1))
    {
      cout << "Некорректный ввод";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << endl;
    }
    else break;
  }
  return i;
}

int testC()
{
  int i;
  while (true)
  {
    cout << "Ведите код причины прерывания [0-511]: ";
    cin >> i;
    if ((cin.fail()) or (i < 0) or (i > 511))
    {
      cout << "Некорректный ввод";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << endl;
    }
    else break;
  }
  return i;
} 

int testP()
{
  int i;
  while (true)
  {
    cout << "Введите признак завершения программы в канале [0-1]: ";
    cin >> i;
    if ((cin.fail()) or (i < 0) or (i > 1))
    {
      cout << "Некорректный ввод";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << endl;
    }
    else break;
  }
  return i;
}

unsigned short to_hex(int N,int E, int C, int P)
{
  return P | C << 2 | E << 11 | N << 13;
}

int main()
{
  setlocale(LC_ALL, "rus");
  int N, E, C, P;
  N = testN();
  E = testE();
  C = testC();
  P = testP();
  unsigned short X= to_hex(N, E, C, P);
  cout << hex << X;
}