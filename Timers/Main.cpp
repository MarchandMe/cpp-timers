#include <iostream>
#include <vector>
#include "Timer.h"

void a(int a) {
	std::cout << a << std::endl;
}
void b(int a) {
	std::cout << a << "\n";
}
void c(int a) {
	printf("%i \n", a);
}

int main() { //drive & test
	Timers::Comparator<void, int> cm;
	cm.addFunc(a, 1);
	cm.addFunc(b, 1);
	cm.addFunc(c, 1);
	//cm.run(); // for one-by-one testing
	cm.runParallel(); // for parallel testing
	std::vector<double> t = cm.getTimes();
	for (double i : t) printf("%lf\n", i);
	std::cin.get();
}