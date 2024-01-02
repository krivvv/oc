#include <iostream>
#include <windows.h>
#include <string>
using namespace std;

HANDLE A, B, C, D, X, c_2, c_1, semafor, mutex;
struct MyStruct
{
    int countof;
    int count;
    char* lpszAppName;
    char* args;
    STARTUPINFO* si;
    PROCESS_INFORMATION* piApp;
    HANDLE* a;
    MyStruct(int countof1, int count1, char* lpszAppName1, char* args1, STARTUPINFO* si1, PROCESS_INFORMATION* piApp1, HANDLE* a1) {
        countof = countof1;
        count = count1;
        lpszAppName = lpszAppName1;
        si = si1;
        piApp = piApp1;
        args = args1;
        a = a1;
    }
    MyStruct& operator=(const MyStruct& other) {
        countof = other.countof;
        count = other.count;
        lpszAppName = other.lpszAppName;
        si = other.si;
        piApp = other.piApp;
        args = other.args;
        a = other.a;
        return *this;
    }
};
int main()
{
    A = CreateEvent(NULL, FALSE, FALSE, (LPSTR)"A");
    X = CreateEvent(NULL, FALSE, FALSE, (LPSTR)"X");
    B = CreateEvent(NULL, FALSE, FALSE, (LPSTR)"B");
    C = CreateEvent(NULL, TRUE, FALSE, (LPSTR)"C");
    D = CreateEvent(NULL, TRUE, FALSE, (LPSTR)"D");
    c_2 = CreateEvent(NULL, FALSE, FALSE, (LPSTR)"c_2");
    c_1 = CreateEvent(NULL, FALSE, FALSE, (LPSTR)"c_1");

    mutex = CreateMutex(NULL, FALSE, (LPSTR)"mutex");

    int countof;
    int countof_;
    std::cout << "Enter the number of all Writer processes ";
    cin >> countof;
    std::cout << "Enter the number of all Reader processes ";
    cin >> countof_;

    std::cout << endl;

    int count;
    int count_;
    std::cout << "Enter the number of each Writer messages ";
    cin >> count;
    std::cout << "Enter the number of each Reader messages ";
    cin >> count_;
    std::cout << endl;
    bool check1 = false;

    if (count_ == count) {
        check1 = true;
    }
    semafor = CreateSemaphore(NULL, 0, count_, (LPSTR)"semafor");
    char lpszAppName[] = "Reader.exe";
    char lpszAppName_[] = "Reader1.exe";

    char countAsString[20];
    sprintf_s(countAsString, "%d", count);
    char args[100];
    sprintf_s(args, "%s %s", lpszAppName, countAsString);

    char countAsString_[20];
    sprintf_s(countAsString_, "%d", count_);
    char args_[100];
    sprintf_s(args_, "%s %s", lpszAppName_, countAsString_);

    STARTUPINFO* si = new STARTUPINFO[countof];
    PROCESS_INFORMATION* piApp = new PROCESS_INFORMATION[countof];
    HANDLE* a = new HANDLE[countof];
    STARTUPINFO* si1 = new STARTUPINFO[countof_];
    PROCESS_INFORMATION* piApp1 = new PROCESS_INFORMATION[countof_];
    HANDLE* a1 = new HANDLE[countof_];
    MyStruct first(countof, count, lpszAppName, args, si, piApp,a);
    MyStruct second(countof_, count_, lpszAppName_, args_, si1, piApp1,a1);
    if (countof_ > countof) {
        MyStruct third = second;
        second = first;
        first = third;
    }
    int cc = 0;
    for (int i = 0; i < first.countof; i++) {
        ZeroMemory(&first.si[i], sizeof(STARTUPINFO));
        first.si[i].cb = sizeof(STARTUPINFO);
        if (!CreateProcess(NULL, first.args, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, (LPSTARTUPINFOA)&first.si[i], &first.piApp[i]))
        {
            std::cout << "The new Writer process is not created.\n";
            return 0;
        }
        first.a[i] = second.piApp[i].hProcess;
        if (i == 0) SetEvent(X);
        Sleep(500);


        int j = i;
        if (cc < second.countof) {
            ZeroMemory(&second.si[j], sizeof(STARTUPINFO));
            second.si[j].cb = sizeof(STARTUPINFO);
            if (!CreateProcess(NULL, second.args, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, (LPSTARTUPINFOA)&second.si[j], &second.piApp[j]))
            {
                std::cout << "The new  Reader process is not created.\n";
                return 0;
            }
            cc++;
            second.a[j] = second.piApp[j].hProcess;
            Sleep(300);
        }
        int k = 0;
        while (k <  first.count) {
            WaitForSingleObject(mutex, INFINITE);

            if (WaitForSingleObject(A, 0) == WAIT_OBJECT_0) {
                std::cout << "Writer process shows message A" << endl;
                SetEvent(C);
            }

            if (WaitForSingleObject(B, 0) == WAIT_OBJECT_0) {
                std::cout << "Writer process shows message B" << endl;
                SetEvent(D);
            }

            ReleaseSemaphore(semafor, 1, NULL);
            ResetEvent(A);
            ResetEvent(B);

            k++;

            SetEvent(X);
            ReleaseMutex(mutex);
            Sleep(300);
        }
    }
    WaitForSingleObject(c_1, 0);
    WaitForSingleObject(c_2, 0);
    std::cout << "Reader and Writer are done." << endl;
    return 0;
}