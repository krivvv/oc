#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <cstdlib>
#include <vector>
using namespace std;

struct Array
{
	int n;
	char arr[51];
};

int main(int argc, char* argv[]) {
	HANDLE hReadChannelReady = OpenEvent(EVENT_ALL_ACCESS, FALSE, (LPCSTR)"WRITE_CHANNEL_READY");
	HANDLE hWriteChannelReady = OpenEvent(EVENT_ALL_ACCESS, FALSE, (LPCSTR)"READ_CHANNEL_READY");
	int dwBytesWritten = 0;
	int n;
	HANDLE hWritePipe = (HANDLE)atoi(argv[1]),	hReadPipe = (HANDLE)atoi(argv[0]);
	WaitForSingleObject(hReadChannelReady, INFINITE);
	if (!ReadFile(hReadPipe, (char*)&n, sizeof(int), (LPDWORD)&dwBytesWritten, NULL))
	{
		_cputs("Read from file failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}
	vector <_int16> b(n);
	srand(static_cast<unsigned int>(time(0)));
	cout << "Elements of array: ";
	for (int i = 0; i < n; ++i) {
		b[i] = rand() % 100; 
		cout << b[i] << ' ';
	}
	cout << endl;
	int n1;
	cout << " Enter N: ";
	cin >> n1;
	vector<_int16> a;
	cout << "Elements that are greater than N: " << endl;
	for (_int16 i = 0; i < b.size(); i++) {
		if (b[i] > n1) {
			a.push_back(b[i]);
			cout << b[i] << ' ';
		}
	}
	int size = a.size();
	if (!WriteFile(hWritePipe, &size, sizeof(int), (LPDWORD)&dwBytesWritten, NULL))
	{
		_cputs("Write to file failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}
	SetEvent(hWriteChannelReady);
	for (_int16 i = 0; i < size; ++i)
	{
		WaitForSingleObject(hReadChannelReady, INFINITE);
		if (!WriteFile(hWritePipe, &(a[i]), sizeof(a[i]), (LPDWORD)&dwBytesWritten, NULL))
		{
			_cputs("Write to file failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
		SetEvent(hWriteChannelReady);

	}
	cout << endl;
	_cputs("Press any key to finish.\n");
	_getch();
	return 0;
}
