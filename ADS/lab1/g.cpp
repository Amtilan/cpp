#include <bits/stdc++.h>
using namespace std;

bool isPrime(int n){
      if(n == 1 || n == 0) return false;
      for(int i = 2; i < n; i++) if(n % i == 0) return false;
      return true;
}

int main(){
    int n; cin >> n;
    int cnt = 0,cnt2 = 0;
    for(int i=1;i<sqrt(INT_MAX);i++){
        if(isPrime(i)) cnt++;
        if(isPrime(cnt) && isPrime(i)) cnt2++; 
        if(cnt2==n){
            cout << i;
            break;
        }
    }
    return 0;
}