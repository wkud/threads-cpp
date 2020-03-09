#pragma once
#include "IThread.h"
#include "Producer.h"

class Consumer : public IThread
{
	Producer& producer;
	double avg;
	std::mutex locker;
	
	void ThreadRoutine() override;

public:
	Consumer(Producer& producer);
	
	double GetAvg();

};

