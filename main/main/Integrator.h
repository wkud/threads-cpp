#pragma once
#include "IThread.h"
#include <vector>
#include <mutex>

class Integrator : public IThread
{
public:
	enum Status { IDLE, WORKING };
	void Count(const std::vector<double>& data, const double step); //set data for counting integral
	void SetStatus(Status status); //synchronize by statusMutex
	Status GetStatus(); //synchronize by statusMutex
	double GetResult(); //synchronize by resultMutex

	Integrator(); //safe

private:
	Status status;
	double result;
	std::vector<double> data_; //itegral input (function values)
	double step; //x step
	std::mutex dataMutex; //data, step
	std::mutex statusMutex;
	std::mutex resultMutex;
	void ThreadRoutine() override;

};

