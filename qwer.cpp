#include <bits/stdc++.h>

using namespace std;

vector <int> getprostoi (int n){
    vector <int> prostoi;
    for(int i = 2; i <= n; i++){
        int count = 0;
        for (int k = 1; k <= i; k++){
            if (i % k == 0){
                count++;
            }
        }
        if(count <= 2){
            prostoi.push_back(i);
        }
    }
    return prostoi;
}

int main(){
    int n;
    cin >> n;
    vector <int> prostoi = getprostoi(n);
    for(auto c : prostoi) cout << c << " ";
    return 0;
}