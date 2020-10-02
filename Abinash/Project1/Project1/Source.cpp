#include <iostream>
#include <thread>
using namespace std;


void loop1(int a) {
	cout << a << " have been printed" << endl;
	for (int i = 0; i < 100000; i++) {
		
	}
}
void loop2(int a) {
	cout << a << " have been printed" << endl;
	for (int i = 0; i < 100000; i++) {

	}
}
int main() {
	thread thread1(loop1, 2);
	thread thread2(loop1, 2);
	thread2.join();
	thread1.join();
	getchar();
	return 0;
}