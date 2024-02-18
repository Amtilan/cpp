#include <bits/stdc++.h>

using namespace std;

void upper(int size) {
    for (int i = size / 2; i <= size; i += 2) {
        for (int j = 1; j < size - i; j += 2) cout << " ";
        for (int j = 1; j <= i; j++) cout << "V";
        for (int j = 1; j <= size - i; j++) cout << " ";
        for (int j = 1; j <= i; j++) cout << "V";
        cout << endl;
    }
    return;
}

void lower(int size) {
    string message = " I Love You! ";
    int message_length = message.length();
    int spaces = ((size * 2) -  message_length) / 2;
    for (int j = 1; j < spaces; j++) {
        cout << "V";
    }
    cout << message;
    for (int j = spaces + message_length; j < ((size * 2)); j++) {
        cout << "V";
    }
    cout << endl;

    for (int i = size - 1; i >= 0; i--) {
        for (int j = i; j < size; j++) cout << " ";
        for (int j = 1; j < ((i * 2)); j++) cout << "V";
        cout << endl;
    }
    return;
}

int main() {
    int size;
    cin >> size;
    upper(size);
    lower(size);
    return 0;
}
