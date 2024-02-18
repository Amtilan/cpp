#include <iostream>
#include <vector>
 
using namespace std;
 
void BubbleSort ( vector<int> &arr, int n)
{
   for (int i = 0; i < n - 1; ++i)
   { 
      bool swapped = false;
      for (int j = 0; j < n - i - 1; ++j)
      {
         cout << "i = " << i << " j = " << j << endl;
         if ( arr[j] > arr [j + 1]){
            swap( arr[j], arr[j + 1]);
            swapped = true;
         }
      }
      cout << '\n';

      if(!swapped)
         break;
   } 
} 
 
int main()
{   
    vector<int> arr;

    int a, n = 0;
    while ( cin >> a ){
    for ( int i = 0; i < a; i++ ){
      n++;
      int x;
      cin>> x;
      arr.push_back(x);
    }}

 
   BubbleSort( arr, n );
 
   for ( int i = 0; i < n; i++ ){
        cout << arr[i] << " ";
}
}