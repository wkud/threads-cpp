#include "Integrator.h"
#include <chrono>

void Integrator::Count(const std::vector<double>& data, const double step)
{
	//std::lock_guard<std::mutex> lock(dataMutex);
	dataMutex.lock();
	this->data_ = data;
	this->step = step;
	dataMutex.unlock();
}

void Integrator::SetStatus(Status status)
{
	std::lock_guard<std::mutex> lock(statusMutex);
	this->status = status;
}
Integrator::Status Integrator::GetStatus()
{
	std::lock_guard<std::mutex> lock(statusMutex);
	return status;
}
double Integrator::GetResult()
{
	std::lock_guard<std::mutex> lock(resultMutex);
	return result;
}

Integrator::Integrator() : status(Integrator::Status::IDLE), data_(), step(0), result(0)
{
}

void Integrator::ThreadRoutine()
{
	while (isRunning()) {
		if (status == Integrator::Status::IDLE)
		{
			//std::this_thread::yield();
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
		else 
		{
			dataMutex.lock();

			double integralResult = 0;
			for(auto iterator = data_.begin(); iterator != data_.end(); iterator++) //integral result = sum(every f(x) in data)
				integralResult += *iterator; 
			integralResult *= step; // integral result = dx * sum(f(x))

			dataMutex.unlock();

			resultMutex.lock();
			result = integralResult;
			resultMutex.unlock();

			statusMutex.lock();
			status = Integrator::Status::IDLE;
			statusMutex.unlock();
		}
	}
}

