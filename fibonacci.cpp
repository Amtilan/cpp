#include <bits/stdc++.h>

using namespace std;

int main(int argc, char const *argv[]){
    float long n;
    cin >> n;
    string s = to_string(powf((1 - sqrt(5)) / 2, n) * ((5 - sqrt(5)) / 10) + powf((1 + sqrt(5)) / 2, n) * ((sqrt(5) / 10) + 0.5));
    s.erase(s.find_last_not_of('0'), string::npos);
    cout << s << endl;
    return 0;
}