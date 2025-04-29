#include <iostream>
#include <limits>
#include <cmath>

using namespace std;

unsigned int input_size() {
    unsigned int n;
    cout << "Input Size " << endl;
    cin >> n;
    while (!cin.good()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Problem with parametr repyt" << endl;
        cin >> n;
    }
    return n;
}

void input(double** arr, int n) {
    double l;
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            cout << "Input " << i + 1 << " " << k + 1 << ":" << endl;
            cin >> l;
            while (!cin.good()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Problem with " << i + 1 << " " << k + 1 << " repeat" << endl;
                cin >> l;
            }
            arr[i][k] = l;
        }
    }
}

void output(double** arr, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int k = 0; k < col; k++) {
            cout << arr[i][k] << " ";
        }
        cout << endl;
    }
}

bool zad1(double**& arr, int& row, int& col) {
    int str = 0;
    bool* mstr = new bool[row];
    for (int i = 0; i < row; i++) {
        bool zero = true;
        for (int k = 0; k < col; k++) {
            if (arr[i][k] != 0) {
                zero = false;
                break;
            }
        }
        mstr[i] = !zero;
        if (mstr[i]) {
            str++;
        }
    }

    int stolb = 0;
    bool* mstolb = new bool[col];
    for (int k = 0; k < col; k++) {
        bool zero = true;
        for (int i = 0; i < row; i++) {
            if (arr[i][k] != 0) {
                zero = false;
                break;
            }
        }
        mstolb[k] = !zero;
        if (mstolb[k]) {
            stolb++;
        }
    }

    if (str == 0 || stolb == 0) {
        for (int i = 0; i < row; i++) {
            delete[] arr[i];
        }
        delete[] arr;
        arr = nullptr;
        row = 0;
        col = 0;
        delete[] mstr;
        delete[] mstolb;
        return true;
    }

    double** mas = new double*[str];
    for (int i = 0; i < str; i++) {
        mas[i] = new double[stolb];
    }

    int ni = 0;
    for (int i = 0; i < row; i++) {
        if (!mstr[i]) continue;
        
        int nk = 0;
        for (int k = 0; k < col; k++) {
            if (!mstolb[k]) continue;
            
            mas[ni][nk] = arr[i][k];
            nk++;
        }
        ni++;
    }

    for (int i = 0; i < row; i++) {
        delete[] arr[i];
    }
    delete[] arr;

    arr = mas;
    row = str;
    col = stolb;

    delete[] mstr;
    delete[] mstolb;
    return false;
}

int zad2(double** arr, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int k = 0; k < col; k++) {
            if (arr[i][k] > 0) {
                return i + 1;
            }
        }
    }
    return 0;
}

int main() {
    int n = input_size();
    int row = n, col = n;
    
    double** mass = new double*[n];
    for (int i = 0; i < n; i++) {
        mass[i] = new double[n];
    }
    
    input(mass, n);
    output(mass, row, col);
    
    bool deleted = zad1(mass, row, col);
    
    if (row == 0 && col == 0) {
        cout << "Матрица пуста" << endl;
    } else {
        output(mass, row, col);
        cout << "Unsigned row" << zad2(mass, row, col) << endl;
    }
    
    if (!deleted) {
        for (int i = 0; i < row; i++) {
            delete[] mass[i];
        }
        delete[] mass;
    }
    
    return 0;
}