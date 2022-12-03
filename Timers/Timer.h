#pragma once
#include <chrono>
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
namespace Timers {
	class autoTimer //basic timer, outputs to console, ends when destructed
	{
	private:
		std::chrono::steady_clock::time_point timeStart, timeEnd;
		std::chrono::duration<double> dur;
	public:
		autoTimer() {
			timeStart = std::chrono::high_resolution_clock::now();
		}
		~autoTimer() {
				timeEnd = std::chrono::high_resolution_clock::now();
				dur = timeStart - timeEnd;
				printf("proccess used %lf ms \n", (dur.count() * 1000));
		}
	};

	class manualTimer //timer with manual controls
	{
	private:
		std::chrono::steady_clock::time_point timeStart, timeEnd;
		std::chrono::duration<double> dur;
		bool start = false;
	public:
		void startTimer() {
			start = true;
			timeStart = std::chrono::high_resolution_clock::now();
		}
		int endTimer() {
			if (!start) printf("the timer was not started");
			timeEnd = std::chrono::high_resolution_clock::now();
			dur = timeEnd - timeStart;
			printf("proccess used %lf ms \n", (dur.count() * 1000));
			return dur.count() * 1000;
		}
	};

	//remember to use mutexs!
	class CompareTimer //for use between threads
	{
	private:
		manualTimer t1, t2;
	public:

	};

	template<typename reType, typename... Ts> //note that functions will need same signitures, add dummy inputs as needed :)
	class Comparator { //takes function pointers
	private:
		manualTimer timer;

		std::vector<reType(*)(Ts...)> funcs; //vector of all functions
		std::vector<Ts...> funcArgs;
		std::vector<int> Times;

		void const parallelFunc(int address) { //address of the func pointer
			manualTimer ptimer;
			ptimer.startTimer();
			funcs[address](funcArgs[address]);
			Times.emplace_back(ptimer.endTimer());
		}
	public:
		void addFunc(reType(*func)(Ts...), Ts... arg) { 
			funcs.emplace_back(func);
			funcArgs.emplace_back(arg...);
		}
		void const run() {
			for (int i = 0; i < funcs.size(); i++) {
				timer.startTimer();
				funcs[i](funcArgs[i]);
				Times.emplace_back(timer.endTimer());
			}
		}
		int* const getTimes() {
			return Times.data();
		}
	};
}