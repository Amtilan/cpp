#include <bits/stdc++.h>

using namespace std;

int main(){
    int n;
    cin >> n;
    int first, second, final, crdts;
    double gpa = 0;
    int cnt_crdts = 0;
    double a[101];
    for (int i = 0; i < 50; i++) a[i] = 0;
    double cnt = 1.0;
    for (int i = 50; i < 100; i += 5){
        a[i] = cnt;
        a[i + 1] = cnt;
        a[i + 2] = cnt;
        a[i + 3] = cnt;
        a[i + 4] = cnt;
        cnt += 1.0 / 3.0;
    }
    a[100] = 4.0;
    while (n--){
        cin >> first >> second >> final >> crdts;
        cnt_crdts += crdts;
        if (first + second < 30 || final < 20) gpa += 0;
        else gpa += (a[first + second + final] * crdts);
    }
    cout << gpa / cnt_crdts << endl;
    return 0;
}