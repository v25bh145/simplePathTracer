#include <Windows.h>
#include <iostream>
#include <sstream>
using namespace std;

#define N 3

DWORD WINAPI ThreadFun(LPVOID lpParameter) {
	pair<int, int> *widthRange = (pair<int, int>*) lpParameter;
	string info = "";
	ostringstream buffer(info);
	buffer << "first=" << widthRange->first << ", second=" << widthRange->second << endl;
	cout << buffer.str();
	widthRange->first++;
	return 0;
}
int main() {
	cout << "create thread" << endl;
	HANDLE* handle = new HANDLE[N];
	int width = 7;
	pair<int, int>* widthRange = new pair<int, int>[20];
	for (int i = 0; i < N; ++i) {
		widthRange[i] = { width * i / N, width * (i + 1) / N };
		// [ first(), second() )
		handle[i] = CreateThread(NULL, 0, ThreadFun, widthRange + i, 0, NULL);
	}
	for (int i = 0; i < N; ++i) {
		WaitForSingleObject(handle[i], INFINITE);
	}
	for (int i = 0; i < N; ++i) {
		cout << widthRange[i].first << endl;
	}
	return 0;
}