#include<bits/stdc++.h>

using namespace std;

int sum_reverse(int sum){
    string sum_string = to_string(sum);
    reverse(sum_string.begin(), sum_string.end());
    return stoi(sum_string);
}

int reverse_number(string first_number, string second_number){
    reverse(first_number.begin(), first_number.end());
    reverse(second_number.begin(), second_number.end());
    int real_first_number = stoi(first_number), real_second_number = stoi(second_number);
    return sum_reverse(real_first_number + real_second_number);
}

int main(){
    int n;
    cin >> n;
    vector<int> numbers;
    for(int i = 0; i < n; i++){
        string first_number, second_number;
        cin >> first_number >> second_number;
        numbers.push_back(reverse_number(first_number, second_number));
    }
    for(int i = 0; i < numbers.size(); i++){
        cout << numbers[i] << endl;
    }
    return 0;
}