#include <bits/stdc++.h>

using namespace std;

void giving(map<int, int>& coins, int index, int& cus_coins) {
    coins[index]--; 
    cus_coins -= index;
}

string check_bank(map<int, int>& coins, int cus_coins){
    while(cus_coins != 0){
        if(cus_coins >= 100){
            if(coins[100] == 0) return "Transaction stopped!";//chech if there is no coins available
            giving(coins, 100, cus_coins); 
        }
        else if(cus_coins >= 50){
            if(coins[50] == 0) return "Transaction stopped!";
            giving(coins, 50, cus_coins);
        }
        else if(cus_coins >= 20){
            if(coins[20] == 0) return "Transaction stopped!";
            giving(coins, 20, cus_coins);
        }
        else if(cus_coins >= 10){
            if(coins[10] == 0) return "Transaction stopped!";
            giving(coins, 10, cus_coins);
        }
        else if(cus_coins >= 5){
            if(coins[5] == 0) return "Transaction stopped!";
            giving(coins, 5, cus_coins);
        }
        else if(cus_coins >= 2){
            if(coins[2] == 0) return "Transaction stopped!";
            giving(coins, 2, cus_coins);
        }
        else if(cus_coins >= 1){
            if(coins[1] == 0) return "Transaction stopped!";
            giving(coins, 1, cus_coins);
        }
    }
    return "Transaction accepted!";
}

int main(){
    map<int, int> coins;
    int inital_number; 
    cin >> inital_number; coins[1] = inital_number;//giving inital number of coins for each type
    cin >> inital_number; coins[2] = inital_number;
    cin >> inital_number; coins[5] = inital_number;
    cin >> inital_number; coins[10] = inital_number;
    cin >> inital_number; coins[20] = inital_number;
    cin >> inital_number; coins[50] = inital_number;
    cin >> inital_number; coins[100] = inital_number;
    int customers;
    cin >> customers;
    vector<int> cus_coins(customers);
    for(int i = 0; i < customers; i++) cin >> cus_coins[i];
    vector<string> answer(customers);
    for(int i = 0; i < customers; i++) answer[i] = check_bank(coins, cus_coins[i]); //givin answer
    for(string it : answer) cout << it << endl;
}