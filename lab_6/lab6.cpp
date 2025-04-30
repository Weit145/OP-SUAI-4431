#include <iostream>
#include <cmath>

using namespace std;

unsigned int input_size(char* a) {
    int n;
    cout << "Input "<<a<< endl;
    cin >> n;
    while (!cin.good() or n<0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Problem with parametr repyt" << endl;
        cin >> n;
    }
    return n;
}

void input(double** arr, int row, int col) {
    double l;
    for (int i = 0; i < row; i++) {
        for (int k = 0; k < col; k++) {
            cout << "Input " << i + 1 << " " << k + 1 << ":" << endl;
            cin >> l;
            while (!cin.good() ) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Problem with " << i + 1 << " " << k + 1 << " repeat" << endl;
                cin >> l;
            }
            arr[i][k] = l;
        }
    }
    cout << endl;
}

void output(double** arr, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int k = 0; k < col; k++) {
            cout << arr[i][k] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool zad1(double**& arr, int& row, int& col) {
    int str = 0;
    bool* mstr = new bool[row];
    for (int i = 0; i < row; i++) {
        bool zero = 0;
        for (int k = 0; k < col; k++) {
            if (arr[i][k] != 0) {
                zero = 1;
                break;
            }
        }
        mstr[i] = zero;
        if (mstr[i]) {
            str++;
        }
    }
    int stolb = 0;
    bool* mstolb = new bool[col];
    for (int k = 0; k < col; k++) {
        bool zero = 0;
        for (int i = 0; i < row; i++) {
            if (arr[i][k] != 0) {
                zero = 1;
                break;
            }
        }
        mstolb[k] = zero;
        if (mstolb[k]) {
            stolb++;
        }
    }
    if (str == 0 or stolb == 0) {
        for (int i = 0; i < row; i++) {
            delete[] arr[i];
        }
        delete[] arr;
        row = 0;
        col = 0;
        delete[] mstr;
        delete[] mstolb;
        return 1;
    }
    double** mas = new double*[str];
    for (int i = 0; i < str; i++) {
        mas[i] = new double[stolb];
    }
    int ni = 0;
    for (int i = 0; i < row; i++) {
        if (!mstr[i]) {
            continue;
        }
        int nk = 0;
        for (int k = 0; k < col; k++) {
            if (!mstolb[k]) {
                continue;
            }
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
    return 0;
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
    char sr[]="rows";
    char sc[]="cols";
    int row = input_size(sr);
    int col = input_size(sc);
    double** mass = new double*[row];
    for (int i = 0; i < row; i++) {
        mass[i] = new double[col];
    }
    input(mass, row, col);
    output(mass, row, col);
    if (zad1(mass, row, col)) {
        cout << "Матрица пуста" << endl;
    } 
    else {
        output(mass, row, col);
        int sum=zad2(mass, row, col);
        if (sum!=0){
            cout << "First positive number in row: " << sum << endl;
        }
        else{
            cout << "No positive number" <<endl;
        }
        for (int i = 0; i < row; i++) {
            delete[] mass[i];
        }
        delete[] mass;
    }
}