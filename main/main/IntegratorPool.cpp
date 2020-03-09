#include "IntegratorPool.h"
#include <algorithm>
#include <vector>

IntegratorPool::IntegratorPool(const int poolSize)
{
	for (int i = 0; i < poolSize; i++)
	{
		pool.push_back(new Integrator());
		pool.back()->Start();
	}
}


IntegratorPool::~IntegratorPool()
{
	while (pool.size() > 0)
	{
		pool.back()->Join();
		delete pool.back();
		pool.pop_back();
	}
}


bool IsIdle(Integrator * integrator) {
	return integrator->GetStatus() == Integrator::Status::IDLE;
}
Integrator * IntegratorPool::GetInstance()
{
	/*std::vector<Integrator*>::iterator found;
	for (auto it = pool.begin(); true; )
	{
		if (IsIdle(*it))
			break;
	}*/
	
	std::vector<Integrator*>::iterator found;
	do
	{
		found = std::find_if(pool.begin(), pool.end(), IsIdle);
	} while (found == pool.end());
	

	Integrator* instance = *found;
	return instance;
}



size_t IntegratorPool::GetLoad()
{
	size_t load = 0;
	for (size_t i = 0; i < pool.size(); i++)
	{
		if (pool[i]->GetStatus() == Integrator::Status::WORKING) ++load;
	}
	return load;
}
