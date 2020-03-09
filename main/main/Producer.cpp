#include "Producer.h"
#include <random>
#include <iostream>

using namespace std::chrono;

void Producer::ThreadRoutine() {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(-100.0, 100.0);  // generowanie liczb losowych//

	while (isRunning()) 
	{
		locker.lock();
		auto random = dist(mt);
		collection.push_back(random);
		locker.unlock();
		std::this_thread::sleep_for(100ms);
	}
}
Producer::Producer() :IThread()
{
}
std::vector<double> Producer::GetCollection() {
	locker.lock();
	auto copy = collection;
	locker.unlock();
	return copy;
}