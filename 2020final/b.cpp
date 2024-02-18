#include <bits/stdc++.h>

using namespace std;

void printFirstTriForce(int height){
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < height; j++) {
            cout << " ";
        }
        for (int j = 0; j < height - i - 1; j++) {
            cout << " ";
        }

        for (int j = 0; j <= i; j++) {
            cout << "* ";
        }
        cout << endl;
    }
    return;
}

void printTriforce(int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < height - i - 1; j++) {
            cout << " ";
        }

        for (int j = 0; j <= i; j++) {
            cout << "* ";
        }
        for (int j = 0; j < 2 * (height - i - 1); j++) {
            cout << " ";
        }
        for (int j = 0; j <= i; j++) {
            cout << "* ";
        }
        cout << endl;
    }
    return;
}

int main() {
    int height;
    cin >> height;
    printFirstTriForce(height);
    printTriforce(height);
    return 0;
}
