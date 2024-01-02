#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <vector>
using namespace std;

int main() {
    HANDLE hWriteChannelReady = CreateEvent(NULL, FALSE, FALSE, (LPCSTR)"WRITE_CHANNEL_READY");
    HANDLE hReadChannelReady = CreateEvent(NULL, FALSE, FALSE, (LPCSTR)"READ_CHANNEL_READY");
    HANDLE hPipeRead, hPipeWrite, hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = FALSE;
    int n;
    cout << "Enter array size: " << endl;
    cin >> n;
    if (!CreatePipe(&hPipeRead, &hPipeWrite, &sa, 0))
    {
        _cputs("Create pipe failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }
    if (!DuplicateHandle(
        GetCurrentProcess(), 
        hPipeWrite, 
        GetCurrentProcess(), 
        &hWritePipe, 
        0,
        TRUE, 
        DUPLICATE_SAME_ACCESS))
    {
        _cputs("Duplicate handle failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }
    CloseHandle(hPipeWrite);
    if (!DuplicateHandle(
        GetCurrentProcess(), 
        hPipeRead, 
        GetCurrentProcess(), 
        &hReadPipe, 
        0, 
        TRUE,
        DUPLICATE_SAME_ACCESS))
    {
        _cputs("Duplicate handle failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }
    CloseHandle(hPipeRead);
    STARTUPINFO si;
    PROCESS_INFORMATION piApp;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    char lpszAppName[] = "C:\\Users\\ro\\source\\repos\\server\\x64\\Debug\\hignt.exe";
    char lpszParameter[20];
    sprintf_s(lpszParameter, "%d %d", (int)hReadPipe, (int)hWritePipe);
    ZeroMemory(&si, sizeof(STARTUPINFO));
    if (!CreateProcessA((LPCSTR)lpszAppName, lpszParameter, nullptr, nullptr, TRUE,
        CREATE_NEW_CONSOLE, nullptr, nullptr, (LPSTARTUPINFOA)&si, &piApp))
    {
        _cputs("The new process is not created.\n");
        _cputs("Check a name of the process.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return 0;
    }
    int dwBytesWritten = 0;
    if (!WriteFile(hWritePipe, (char*)&n, sizeof(int), (LPDWORD)&dwBytesWritten, NULL))
    {
        _cputs("Write to file failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }
    SetEvent(hWriteChannelReady);
    WaitForSingleObject(hReadChannelReady, INFINITE);
    int n1;
    if (!ReadFile(hReadPipe, &(n1), sizeof(int), (LPDWORD)&dwBytesWritten, NULL))
    {
        _cputs("Read from file failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }
    SetEvent(hWriteChannelReady);
    vector<_int16> a(n1);
    for (int i = 0; i < n1; ++i)
    {
        WaitForSingleObject(hReadChannelReady, INFINITE);
        if (!ReadFile(hReadPipe, &(a[i]), sizeof(a[i]), (LPDWORD)&dwBytesWritten, NULL))
        {
            _cputs("Read from file failed.\n");
            _cputs("Press any key to finish.\n");
            _getch();
            return GetLastError();
        }
        SetEvent(hWriteChannelReady);
    }
    for (int i = 0; i < n1; i++) {
        cout << a[i] << ' ';
    }
    cout << endl;
    _cputs("Press any key to finish.\n");
    _getch();
    CloseHandle(hWriteChannelReady);
    CloseHandle(hReadChannelReady);
    CloseHandle(piApp.hThread);
    CloseHandle(piApp.hProcess);
    CloseHandle(hReadPipe);
    CloseHandle(hWritePipe);

    return 0;
}
