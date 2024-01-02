#include <iostream>
#include <windows.h>
#include <time.h>
#include <string>
#include <conio.h>
#include <wchar.h>
HANDLE semafor, C, D, c_1;
CRITICAL_SECTION A_;
using namespace std;

int main(int argc, char* argv[])
{
	int count = std::stoi(argv[1]);
	//std::cout << "count: " << count << std::endl;

	semafor = CreateSemaphore(NULL, 0, count, (LPSTR)"semafor");
	C = CreateEvent(NULL, TRUE, FALSE, (LPSTR)"C");
	D = CreateEvent(NULL, TRUE, FALSE, (LPSTR)"D");
	c_1 = CreateEvent(NULL, FALSE, FALSE, (LPSTR)"c_1");
	//cout << "fafs " << endl;
	if (C == NULL)
		return GetLastError();
	if (D == NULL)
		return GetLastError();
	if (c_1 == NULL)
		return GetLastError();
	if (semafor == NULL)
	{
		cout << "Create semafor failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}



	int i = 0;
	//cout << count << endl;
	while (i < count) {
		//cout << i << endl;
		/*cout << "	WaitForSingleObject(semafor, INFINITE);" << endl;*/
		WaitForSingleObject(semafor, INFINITE);
		//cout << "done" << endl;
		if (WaitForSingleObject(C, 1) == WAIT_OBJECT_0) {
			cout << "Administrator process shows message C" << endl;
		}
		if (WaitForSingleObject(D, 1) == WAIT_OBJECT_0)
		{
			cout << "Administrator process shows message D" << endl;
		}
		i++;
		ResetEvent(C);
		ResetEvent(D);
	}
	CloseHandle(semafor);
	SetEvent(c_1);
	CloseHandle(c_1);
	CloseHandle(C);
	CloseHandle(D);
	Sleep(300);

	std::cout << "Press Enter to continue...";
	std::string userInput;
	std::getline(std::cin, userInput);
	return 0;
}
