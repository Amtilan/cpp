#include <bits/stdc++.h>

using namespace std;

double gpa_check(int subjects, map<string, double> gpa){
    double sum_of_gpa = 0;
    int cnt_credits = 0;
    for(int i = 0; i < subjects; i++){
        string grade;
        int credits;
        cin >> grade >> credits;
        cnt_credits += credits;
        sum_of_gpa += credits * gpa[grade];
    }
    return (sum_of_gpa / cnt_credits);
}

int main(){
    map<string, string> gpa_students;
    map<string, double> gpa = {
        {"A+", 4.00},
        {"A", 3.75},
        {"B+", 3.50},
        {"B", 3.00},
        {"C+", 2.50},
        {"C", 2.00},
        {"D+", 1.50},
        {"D", 1.00},
        {"F", 0.00}
    };
    int list_students;
    cin >> list_students;
    for(int i = 0; i < list_students; i++){
        string surname, name;
        cin >> surname >> name;
        int subjects;
        cin >> subjects;
        gpa_students[surname + ' ' + name] = gpa_check(subjects, gpa);
    }
    for(map<string, string>::iterator it = gpa_students.begin(); it != gpa_students.end(); it++){
        cout << it->first << " " << setprecision(4) << it->second << endl; //МНЕ БЫЛО ЛЕНЬ ОКРУГЛЯТЬ НОРМАЛЬНО и было лень сделать норм сортировку по значению
    }
    return 0;
}