#include <bits/stdc++.h>

using namespace std;

int joke (int a){
    int  sum = 0;
    
    while (a!=0){
        sum += a % 10;
        a /= 10;
    }
    return sum;


}
int main(){
    int n;
    cin >> n;
    cout << joke (n);
    return 0;
}