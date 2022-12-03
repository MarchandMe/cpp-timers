#include <iostream>
#include <thread>
#include <mutex>
#include "Timer.h"

void a(int a) {
	std::cout << a << std::endl;
}
void b(int a) {
	std::cout << a << "\n";
}
void c(int a) {
	printf("%i \n",a);
}

int main() { //drive & test
	Timers::Comparator<void, int> cm;
	cm.addFunc(a, 1);
	cm.addFunc(b, 1);
	cm.addFunc(c, 1);
	cm.run();
	cm.runParallel();
	std::cin.get();
}