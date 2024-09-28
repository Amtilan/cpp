#include <bits/stdc++.h>

using namespace std;

int main(){
    int n; 
    cin >> n;
    while(n != 1){
        if(n % 2 != 0){
            cout << "NO" << endl;
            break;
        }
        n /= 2;
    }
    if(n == 1) cout= << "YES" << endl;
    return 0;
}