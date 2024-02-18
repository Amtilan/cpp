#include <bits/stdc++.h>

using namespace std;

int main(){
	int t, n, i, k, p = 0, l; 
	cin >> n; 
	
    bool S[2 * n];

    for(k = 0; k <= n * 2; ++k)
        S[k]=true;
    
    for(k = 2; k * k <= n * 2; ++k)
        if(S[k]) 
            for(l = k * k; l <= 2 * n; l += k) 
                S[l]=false; 
    
    for(k = n + 1; k <= n * 2; ++k)
        if(S[k])  
            p++;
    
    cout << p;
    
    return 0;
} 