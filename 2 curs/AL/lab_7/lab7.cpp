#include <iostream>
#include <vector>

using namespace std;

int inputRowsCols(string prompt) {
    int n;
    cout << prompt;
    cin >> n;
    return n;
}

int** createMatrix(int n) {
    int** matrix = new int*[n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[n];
    }
return matrix;
}

void inputMatrix(int** matrix, int n) {
    cout << "Enter matrix bool elements: " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << "Element [" << i << "][" << j << "]: ";
            cin >> matrix[i][j];
        }
        cout<<endl;
    }
}

void deleteMatrix(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}


bool isOddMatrix(int** matrix, int n, int& trip) {
    int oddCount = 0;
    for (int i = 0; i < n; i++) {
        int temp = 0;
        for (int j = 0; j < n; j++) {
            temp += matrix[i][j];
        }
        if (temp % 2 != 0) {
            oddCount++;
        }
    }
    trip = oddCount;
    return (oddCount == 0 || oddCount == 2);
}
void copyMatrix(int** source, int** dest, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dest[i][j] = source[i][j];
        }
    }
}

void foundTrip(int** matrix, int n, int start) {
    cout << "Euler path: ";
    vector<int> stack;
    stack.push_back(start);
    int **copy_matrix = createMatrix(n);
    copyMatrix(matrix, copy_matrix, n);

    while (!stack.empty()) {
        int v = stack.back();
        int i = 0;
        for (i ; i < n; i++) {
            if (copy_matrix[v][i] == 1) {
                stack.push_back(i);
                copy_matrix[v][i]--;
                copy_matrix[i][v]--;
                break;
            }
        }
        if (i == n) {
            cout << v << " ";
            stack.pop_back();
        }
    }
    deleteMatrix(copy_matrix, n);
    cout << endl;
}

void showMatrix(int** matrix, int n) {
    cout << "Adjacency matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}




int main() {
    int** matrix = nullptr;
    int n = 0;
    int trip = 0;
    int menu = 0;

    while (true) {
        cout << "\n===== MENU =====\n";
        cout << "1 - Exit\n";
        cout << "2 - Input graph size and create matrix\n";
        cout << "3 - Input adjacency matrix\n";
        cout << "4 - Check Euler path/cycle\n";
        cout << "5 - Find Euler path\n";
        cout << "6 - Show adjacency matrix\n";
        cout << "Enter: ";

        cin >> menu;

        if (menu == 1) {
            cout << "Exit.\n";
            break;
        }

        switch (menu) {

        case 2: {
            if (matrix != nullptr) {
                deleteMatrix(matrix, n);
            }
            n = inputRowsCols("Enter number of vertices: ");
            matrix = createMatrix(n);
            cout << "Matrix created.\n";
            break;
        }

        case 3: {
            if (matrix == nullptr) {
                cout << "Matrix not created. Create it first.\n";
                break;
            }
            inputMatrix(matrix, n);
            cout << "Matrix input completed.\n";
            break;
        }

        case 4: {
            if (matrix == nullptr) {
                cout << "Matrix not created.\n";
                break;
            }
            if (isOddMatrix(matrix, n, trip)) {
                cout << "Euler path or cycle is possible. Odd vertices: " << trip << endl;
            } else {
                cout << "Euler path or cycle is NOT possible. Odd vertices: " << trip << endl;
            }
            break;
        }

        case 5: {
            if (matrix == nullptr) {
                cout << "Matrix not created.\n";
                break;
            }
            if (!isOddMatrix(matrix, n, trip)) {
                cout << "Euler path or cycle is NOT possible.\n";
                break;
            }
            int start = 0;
            cout << "Enter starting vertex: ";
            cin >> start;
            if (start < 0 || start >= n) {
                cout << "Invalid vertex.\n";
                break;
            }
            foundTrip(matrix, n, start);
            break;
        }
        case 6: {
            if (matrix == nullptr) {
                cout << "Matrix not created.\n";
                break;
            }
            showMatrix(matrix, n);
            break;
        }
        default:
            cout << "Invalid option.\n";
            break;
        }
    }

    if (matrix != nullptr) {
        deleteMatrix(matrix, n);
    }

    return 0;
}

