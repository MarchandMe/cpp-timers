#pragma once
#include <chrono>
#include <iostream>
#include <vector>
#include <thread>
//everything is in milliseconds
namespace Timers {
	class autoTimer //basic timer, outputs to console, ends when destructed
	{
	private:
		std::chrono::steady_clock::time_point timeStart, timeEnd;
		std::chrono::duration<double> dur;
	public:
		autoTimer()	{
			timeStart = std::chrono::high_resolution_clock::now();
		}
		~autoTimer() 
		{
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
		double interval() { //does not reset
			if (!start) printf("the timer was not started");
			timeEnd = std::chrono::high_resolution_clock::now();
			dur = timeEnd - timeStart;
			printf("proccess used %lf ms \n", (dur.count() * 1000));
			return dur.count() * 1000;
		}
		double intervalNoOutput() {
			if (!start) printf("the timer was not started");
			timeEnd = std::chrono::high_resolution_clock::now();
			dur = timeEnd - timeStart;
			return dur.count() * 1000;
		}
		double endTimer() {
			if (!start) printf("the timer was not started");
			timeEnd = std::chrono::high_resolution_clock::now();
			dur = timeEnd - timeStart;
			printf("proccess used %lf ms \n", (dur.count() * 1000));
			start = false; //can be reused, resets
			return dur.count() * 1000;
		}
		double endTimerNoOutput() {
			if (!start) printf("the timer was not started");
			timeEnd = std::chrono::high_resolution_clock::now();
			dur = timeEnd - timeStart;
			start = false; //can be reused, resets
			return dur.count() * 1000;
		}
	};

	//TODO: write parallel async, automatic thread pool
	template<typename reType, typename... Ts> //note that functions will need same signatures, add dummy inputs as needed :)
	class Comparator { //takes function pointers
	private:
		manualTimer timer;
		
		struct process{
			reType(*p_func)(Ts...);
			std::vector<Ts...> funcArgs; //too lazy to write this :)
			double time;
		};
		std::vector<process> processes; 
		std::vector<std::thread> threads;

		void const parallelFunc(int address) { //address of the func pointer
			manualTimer ptimer;
			ptimer.startTimer();
			processes[processes.size() - address - 1].p_func(processes[processes.size() - address - 1].funcArgs[0]); //in the correct order
			processes[processes.size() - address - 1].time = (ptimer.endTimerNoOutput());
		}
	public:
		void addFunc(reType(*func)(Ts...), Ts... arg) {
			processes.resize(processes.size() + 1);
			processes[processes.size() - 1].p_func = (func);
			processes[processes.size() - 1].funcArgs.emplace_back(arg...);
		}
		void const runParallel() {
			for (int i = 0; i < processes.size(); i++)
				threads.emplace_back(std::thread(&Comparator::parallelFunc, this, i));
			for (int i = 0; i < processes.size(); i++) threads[i].join();
			for (int i = 1; i <= processes.size(); i++) {
				printf("process number %i took %lf milliseconds\n", i, processes[i-1].time);
			}
		}
		void const run() {
			for (int i = 0; i < processes.size(); i++) {
				timer.startTimer();
				processes[i].p_func(processes[i].funcArgs[0]); //in the correct order
				processes[i].time = (timer.endTimerNoOutput());
			}
			for (int i = 1; i <= processes.size(); i++) {
				printf("process number %i took %lf milliseconds\n", i, processes[i - 1].time);
			}
		}
		std::vector<double> getTimes() {
			std::vector<double> tmp;
			for (int i = 0; i < processes.size(); i++) tmp.emplace_back(processes[i].time);
			return tmp;
		}
		
	};
}