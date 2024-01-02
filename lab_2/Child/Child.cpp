#include <windows.h>
#include <conio.h>
#include <vector>
#include <iostream>
using namespace std;
void Sortixxx(int* arr, int n) {
    vector<int> a;
    vector<int> b;
    for (int i = 0; i < n; i++) {
        if (arr[i] % 5 == 0) {
            a.push_back(arr[i]);
            b.push_back(i);
        }
    }
    for (int i = 0; i < a.size(); i++) {
        int a1 = arr[i];
        arr[i] = a[i];
        arr[b[i]] = a1;
    }
}
int main(int argc, char* argv[])
{   
    
    int n = atoi(argv[0]);
    for (int i = 0; i < n; i++) {
        cout << i << " - " << argv[i];
    }
    int* a = new int[n];
    for (int i = 1; i < n+1 ; i++) {
        a[i-1] = atoi(argv[i]);
    }   
    cout << "Array before sorting:" << endl;
    for (int i = 0; i < n; i++) {
        cout << a[i] << ' ';
    }
    Sortixxx(a, n);
    cout << endl;
    cout << "Array after sorting:" << endl;
    for (int i = 0; i < n; i++) {   
        cout << a[i] << ' ';
    }
    _getch();
    delete[] a;
    return 0;
}
