#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

const int SIZE=2000;

const char ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char NUMBERS[] = "0123456789";
struct Segment {
    char keys[6];
    bool isEmpty;
    bool isDeleted;
};

bool validate_Alphabet(char keys) {
    for (int i = 0; i < 26; i++)
    {
        if (keys == ALPHABET[i]){
            return false;
        }
    }
    return true;
}

bool validate_Numbers(char keys) {
    for (int i = 0; i < 10; i++)
    {
        if (keys == NUMBERS[i]){
            return false;
        }
    }
    return true;
}

bool input_keys(char* keys) {
    cout << "Enter 6 keys (characters): ";
    for (int i = 0; i < 6; i++) {
        cin >> keys[i];
        if (i==0 or i==4 or i==5){
            if (validate_Alphabet(keys[i])){
                cout << "Invalid input. Please enter only uppercase letters for positions 0, 4, and 5." << endl;
                return 0;
            }
        }
        else{
            if (validate_Numbers(keys[i])){
                cout << "Invalid input. Please enter only numbers for positions 1, 2, and 3." << endl;
                return 0;
            }   
        }
    }
    return 1;
}

int hash_function(const char* keys) {
    int hash = 0;
    for (int i = 0; i < 6; i++)
    {
        hash = (hash * 31 + keys[i]) % SIZE;    
    }
    return hash; 
}

int step_function( int hash) {
    int step = 1;
    return (hash + step) % SIZE;
}

void add_segment(Segment* hash_table, const char* keys) {
    int hash = hash_function(keys);
    int start = hash;
    while (!hash_table[hash].isEmpty and !hash_table[hash].isDeleted and is_this_key(keys, hash_table[hash].keys)) {
        hash = step_function(hash);
        if (hash == start) {
            cout << "Hash table is full. Cannot add segment." << endl;
            return;
        }
    }
    for (int i = 0; i < 6; i++) {
        hash_table[hash].keys[i] = keys[i];
    }
    hash_table[hash].isEmpty = false;
    hash_table[hash].isDeleted = false;
}

bool is_this_key(const char* keys, const char* keys_table) {
    for (int i = 0; i < 6; i++) {
        if (keys[i] != keys_table[i]) {
            return false;
        }
    }
    return true;
}

bool delete_segment(Segment* hash_table, const char* keys) {
    int hash = hash_function(keys);
    int start = hash;
    while (!hash_table[hash].isEmpty) {
        if (is_this_key(keys, hash_table[hash].keys)) {
            hash_table[hash].isDeleted = true;
            return 1;
        }
        hash = step_function(hash);
        if (hash == start) {
            cout << "Not found segment for deletion." << endl;
            break;
        }
    }
    return 0;
}

void show_segment(Segment* hash_table, int hash) {
    cout << "Segment " << hash << ": " <<endl;
    for (int i = 0; i < 6; i++) {
        cout << hash_table[hash].keys[i];
    }
    cout << endl;
    cout << "Deleted: " << hash_table[hash].isDeleted<< endl;
    cout << "Empty: " << hash_table[hash].isEmpty << endl;
}

void show_segments(Segment* hash_table,const char* keys) {
    int hash = hash_function(keys);
    int start = hash;
    while (!hash_table[hash].isEmpty){
        show_segment(hash_table, hash);
        hash = step_function(hash);
        if (hash == start) {
            cout << "Segment not found." << endl;
            break;
        }
    }
}

void add_all_segments(Segment* hash_table) {
    for (int i = 0; i < SIZE; i++) {
        if (!hash_table[i].isEmpty && !hash_table[i].isDeleted) {
            add_segment(hash_table, hash_table[i].keys);
        }
    }
    cout << "Hash is full" << endl;
}


void generate_random_key(char* key) {
    key[0] = ALPHABET[rand() % 26];
    key[1] = NUMBERS[rand() % 10];
    key[2] = NUMBERS[rand() % 10];
    key[3] = NUMBERS[rand() % 10];
    key[4] = ALPHABET[rand() % 26];
    key[5] = ALPHABET[rand() % 26];
}

void test_hash_function() {
    srand(time(0));
    int hash_counts[SIZE] = {0};
    ofstream file("hash_test_sample.csv");
    file << "Key,HashIndex,CollisionsSoFar\n";
    for (int i = 0; i < 4000; i++) {
        char key[7];
        generate_random_key(key);

        int hash = hash_function(key);
        int collisions = hash_counts[hash];
        hash_counts[hash]++;
        file << key << "," << hash << "," << collisions << "\n";
    }
    file.close();
}

int main() {
    Segment hash_table[SIZE];
    for (int i = 0; i < SIZE; i++) {
        hash_table[i].isEmpty = true;
        hash_table[i].isDeleted = false;
    }
    int menu;
    char keys[6];
    while (true) {
        cout << "\n===== MENU =====\n";
        cout << "1 - Exit\n";
        cout << "2 - Add key\n";
        cout << "3 - Delete key\n";
        cout << "4 - Show key info\n";
        cout << "5 - Show all table\n";
        cout << "6 - Export CSV\n";
        cout << "================\n";
        cout << "Enter choice: ";
        cin >> menu;
        switch (menu) {
            case 1:
                cout << "Exiting program." << endl;
                return 0;

            case 2:
                if (input_keys(keys)) {
                    add_segment(hash_table, keys);
                }
                break;

            case 3:
                if (input_keys(keys)) {
                    if (delete_segment(hash_table, keys))
                        cout << "Key deleted successfully." << endl;
                    else
                        cout << "Key not found." << endl;
                }
                break;

            case 4:
                if (input_keys(keys)) {
                    show_segments(hash_table, keys);
                }   
                break;

            case 5:
                for (int i = 0; i < SIZE; i++) {
                    if (!hash_table[i].isEmpty) {
                        show_segment(hash_table, i);
                    }
                }
                break;

            case 6:
                test_hash_function();
                break;

            default:
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                cout << "Error: Invalid menu option!" << endl;
                break;
        }
    }
}

// Q754EP E557DR T065SX J953DW