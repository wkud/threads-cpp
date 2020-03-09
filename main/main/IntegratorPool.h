#pragma once
#include "Integrator.h"
#include <vector>
#include <mutex>
class IntegratorPool
{
	std::vector<Integrator *> pool;
public:
	IntegratorPool(const int size);
	virtual ~IntegratorPool();
	Integrator * GetInstance();
	size_t GetLoad();
};

