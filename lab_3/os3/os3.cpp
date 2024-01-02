#include <iostream>
#include <windows.h>
#include <vector>
#include <process.h>
#include <cctype>
#include <string>
using namespace std;

CRITICAL_SECTION cs;
long long result = 0;

HANDLE Event_1 = CreateEvent(NULL, TRUE, FALSE, NULL);

DWORD WINAPI Thread2Function(LPVOID lpParam) {
    vector<char>& a = *static_cast<vector<char>*>(lpParam);
    string result = "";
    cout << " Введите интервал : ";
    int i_;
    cin >> i_;
    for (int i = 0; i < a.size(); i++) {
        int count = 0;
        if (isdigit(a[i])) {
            result.push_back(a[i]);
            for (int j = i + 1; j < a.size(); j++) {
                if (a[j] != ' ' && a[j] != '-') {
                    result.push_back(a[j]);
                    count++;
                }
                else {
                    break;
                }
            }
        }
        else {
            result.push_back('_');
        }
        i += count;
        Sleep(i_);
    }
    cout << endl << endl;
    string str = "";
    for (int i = 0; i < result.size(); i++) {
        if (result[i] != '_') {
            str.push_back(result[i]);
        }
    }
    for (int i = 0; i < result.size(); i++) {
        if (result[i] == '_') {
            str.push_back(result[i]);
        }
    }
    for (int i = 0; i < str.size(); i++) {
        a[i] = str[i];
    }
    SetEvent(Event_1);
    return 0;
}

DWORD WINAPI Thread3Function(LPVOID lpParam) {
   
    EnterCriticalSection(&cs);
    WaitForSingleObject(Event_1, INFINITE);
    vector<char>& a = *static_cast<vector<char>*>(lpParam);

 
    cout << "вход в секцию потоком CountElmenets" << endl;
    for (int i = 0; i < a.size(); ++i) {
        if (a[i] != '_') continue;
        else {
            result = i;
            break;
        }
    }
    cout << "Выход из крит секции 3 потока" << endl;
    LeaveCriticalSection(&cs);
    return 0;
}

int main() {
    setlocale(LC_ALL, "RUSSIAN");

    cout << " Введите размер массива: ";
    int n;
    cin >> n;
    vector<char> a(n);
    string input;
    while (std::cin.get() != '\n');
    cout << " Введите элементы : ";
    getline(cin, input);
    for (int i = 0; i < n; ++i) {
        a[i] = input[i];
    }

    cout << " Размер массива : " << n << endl;
    cout << " Массив : ";
    for (int i = 0; i < n; ++i) {
        cout << a[i] << " ";
    }
    cout << endl;
    InitializeCriticalSection(&cs);

    HANDLE hThread2 = CreateThread(NULL, 0, Thread2Function, &a, 0, NULL);
   
    HANDLE hThread3 = CreateThread(NULL, 0, Thread3Function, &a, 0, NULL);
    WaitForSingleObject(Event_1, INFINITE);
    cout << " Массив итог : ";
    for (int i = 0; i < n; ++i) {
        cout << a[i] << " ";
    }
    cout << endl;   
    EnterCriticalSection(&cs);
    cout << "вход в секцию потоком main" << endl;
    cout << "Итоговый подсчёт: " << result << std::endl;
    cout << "выходи из крит секц 1 потока" << endl;
    LeaveCriticalSection(&cs);
   
    WaitForSingleObject(hThread3, INFINITE);

    CloseHandle(Event_1);
    CloseHandle(hThread2);
    CloseHandle(hThread3);
    DeleteCriticalSection(&cs);

    return 0;
}
