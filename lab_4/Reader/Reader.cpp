#include <iostream>
#include <windows.h>
#include <time.h>
#include <string>
HANDLE A, B,X, c_2;
using namespace std;

int main(int argc, char* argv[])
{
	HANDLE mutex;
	A = CreateEvent(NULL, FALSE, FALSE, (LPSTR)"A");
	X = CreateEvent(NULL, FALSE, FALSE, (LPSTR)"X");
	B = CreateEvent(NULL, FALSE, FALSE, (LPSTR)"B");
	c_2 = CreateEvent(NULL, FALSE, FALSE, (LPSTR)"c_2");
	mutex = OpenMutex(SYNCHRONIZE, FALSE, (LPSTR)"mutex");
	int count_ = std::stoi(argv[1]);
	//std::cout << "count: " << count_ << std::endl;
	if (A == NULL) {
		return GetLastError();
	}
	if (B == NULL)
		return GetLastError();
	if (c_2 == NULL)
		return GetLastError();
	if (mutex == NULL)
	{
		cout << "Create mutex failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}

	for (int i = 0; i < count_; i++) {
		/*cout << "WaitForSingleObject(mutex, INFINITE);" << endl;*/
		WaitForSingleObject(X, INFINITE);
		ResetEvent(X);
		WaitForSingleObject(mutex, INFINITE);
		//cout << "done" << endl;
		cout << "Enter the message: ";
		char d;
		cin >> d;
		if (d == 'A')
		{
			SetEvent(A);
		}
		if (d == 'B')
		{
			SetEvent(B);
		}
		//ResetEvent(A);
		//ResetEvent(B);

		ReleaseMutex(mutex);
		Sleep(100);
	}
	SetEvent(c_2);

	CloseHandle(c_2);
	CloseHandle(B);
	CloseHandle(A);
	return 0;
}
