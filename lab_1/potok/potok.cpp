#include <windows.h> 
#include <iostream> 
using namespace std;

struct Massive {
	int* a;
	int n;
	int count;
	Massive(int i) {
		this->n = i;
		a = new int[n];
		count = 0;
	}
	~Massive() {
		delete[] a;
	}
};

DWORD WINAPI worker(void * A) {
	Massive* massive = static_cast<Massive*>(A);
	for (int i = 0; i < massive->n; i++) {
		if (massive->a[i] % 3 == 0) {
			massive->count++;
		}
		Sleep(1000);
	}
	cout << massive->count;
	delete massive;
	return 0;
}

int main()
{
	HANDLE hThread;
	DWORD IDThread;
	int n;
	cout << "Enter the number of array elements:" << endl;
	cin >> n;
	auto A = new Massive(n);
	cout << "Enter elements" << endl;
	for (int i = 0; i < n; i++) {
		cin >> A->a[i];
	}
	int t;
	cout << "Enter sleep time:" << endl;
	cin >> t;
	hThread = CreateThread(NULL, 0, &worker, A, 0, &IDThread);
	if (hThread == NULL)
		return GetLastError();
	SuspendThread(hThread);
	cout << "Thread is stopped" << endl;
	Sleep(t * 1000);
	ResumeThread(hThread);
	cout << "Thread is resumed" << endl;
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 0;
}