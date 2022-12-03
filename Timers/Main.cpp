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
	cm.addFunc(b, 2);
	cm.addFunc(c, 3);
	cm.runParallel(); // for parallel testing
	cm.run(); // for one-by-one testing
	std::vector<double> t = cm.getTimes();
	for (double i : t) printf("%lf\n", i);
	std::cin.get();
}