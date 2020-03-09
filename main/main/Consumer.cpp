#include "Consumer.h"
#include <iostream>
#include <vector>

void Consumer::ThreadRoutine()
{
	using namespace std::chrono;
	while (isRunning())
	{
		auto collection = producer.GetCollection();
		
		double sum = 0; //oblicz sumę
		for (std::vector<double>::iterator iterator = collection.begin(); iterator != collection.end(); iterator++)
			sum += *iterator;
		
		avg = sum / collection.size(); //oblicz średnią na podstawie sumy
		
		std::this_thread::sleep_for(1500ms);
	}
}
Consumer::Consumer(Producer& producer) : producer(producer)
{
}

double Consumer::GetAvg() {
	locker.lock();
	auto copy = avg;
	locker.unlock();
	return copy;
}
