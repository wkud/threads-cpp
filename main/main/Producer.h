#pragma once
#include "IThread.h"
#include <vector>
#include <mutex>

class Producer : public IThread
{
	std::mutex locker;
	std::vector<double> collection;

	void ThreadRoutine() override;

public:
	Producer();
	std::vector<double> GetCollection() ;
};

