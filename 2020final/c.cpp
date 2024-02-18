#include <bits/stdc++.h>

using namespace std;

int main(){
    int number_of_colors;
    cin >> number_of_colors;
    map<int, int> coloring;
    for(int i = 0; i < number_of_colors; i++){
        int metres;
        cin >> metres;
        for(int j = 0; j < metres; j++){
            coloring[j] = i;
        }
    }
    int changes = 1;
    int check_changes = coloring[0];
    for(map<int, int>::iterator it = coloring.begin(); it != coloring.end(); it++){
        if(it->second != check_changes){
            check_changes = it->second;
            changes++;
        }
    }
    cout << changes << endl;
    return 0;
}