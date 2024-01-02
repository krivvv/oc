#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;

int main()
{
    int n;
    cout << "Enter amount of elements" << endl;
    cin >> n;
    cout << "Enter elements" << endl;
    int* a = new int[n];
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    cout << "Enter:\n\"B\" to set blue text color\n\"A\" to set red text color\n\"X\" to set green text color\n\"K\" to set brown text color\n";
    char color;
    cin >> color;
	char args[1000], element[10], color1[2];
	
	wsprintfA(args, "%d", n);
	wsprintfA(color1, "%c", color);
	strcat_s(args, color1);

	wsprintfA(element, "%d ",0);
	strcat_s(args, element);

    for (int i = 0; i < n; ++i)
    {
        wsprintfA(element, "%d ", a[i]);
        strcat_s(args, element);
    }
	STARTUPINFO si;
	PROCESS_INFORMATION piApp;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USEFILLATTRIBUTE;
	switch (color)								
	{
	case 'B':
		si.dwFillAttribute = FOREGROUND_BLUE;
		break;
	case 'A':
		si.dwFillAttribute = FOREGROUND_RED;
		break;
	case 'X':
		si.dwFillAttribute = FOREGROUND_GREEN;
		break;
	case 'K':
		si.dwFillAttribute =  FOREGROUND_RED | FOREGROUND_GREEN;
		break;
	}
	char lpszAppName[] = "Child.exe";
	if (!CreateProcessA((LPCSTR)lpszAppName, args, nullptr, nullptr, FALSE,	
		CREATE_NEW_CONSOLE, nullptr, nullptr, (LPSTARTUPINFOA)&si, &piApp))
	{
		_cputs("The new process is not created.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}
	WaitForSingleObject(piApp.hProcess, INFINITE);
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);
	delete[] a;
	return 0;
}