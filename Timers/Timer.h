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
		/*
		struct process{
			reType(*)(Ts...) p_func;
			Ts... func_Args;
			double Time;
			std::thread rthread
		};
		std::vector<process> processes; */

		std::vector<reType(*)(Ts...)> funcs; //vector of all functions
		std::vector<Ts...> funcArgs;
		std::vector<double> Times;
		std::vector<std::thread> threads;

		void const parallelFunc(int address) { //address of the func pointer
			manualTimer ptimer;
			ptimer.startTimer();
			funcs[funcs.size() - address - 1](funcArgs[funcs.size() - address - 1]); //in the correct order
			Times.emplace_back(ptimer.endTimerNoOutput());
		}
	public:
		void addFunc(reType(*func)(Ts...), Ts... arg) { 
			funcs.emplace_back(func);
			funcArgs.emplace_back(arg...);
		}
		void const runParallel() {
			for (int i = 0; i < funcs.size(); i++)
				threads.push_back(std::thread(&Comparator::parallelFunc, this, i));
			for (auto& t : threads) t.join();
			for (int i = 1; i <= Times.size(); i++) {
				printf("process number %i took %lf milliseconds\n", i, Times[i-1]);
			}
		}
		void const run() {
			for (int i = 0; i < funcs.size(); i++) {
				timer.startTimer();
				funcs[i](funcArgs[i]);
				Times.emplace_back(timer.endTimerNoOutput());
			}
		}
		std::vector<double> const getTimes() {
			return Times;
		}
	};
}