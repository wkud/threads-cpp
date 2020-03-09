#include <iostream>
#include <random>
#include <conio.h>
#include "Integrator.h"
#include "IntegratorPool.h"
#include "Producer.h"
#include "Consumer.h"
#include "main.h"
using namespace std;

/*
Wykonanie programu dla licznosci proby: 100 oraz liczby watkow calkujacych: 100
time without pool: 278
time with pool: 65

Wykonanie programu dla licznosci proby: 1000 oraz liczby watkow calkujacych: 100
time without pool: 376
time with pool: 203

Wykonanie programu dla licznosci proby: 10000 oraz liczby watkow calkujacych: 100
time without pool: 1783
time with pool: 1859

Wykonanie programu dla licznosci proby: 100 oraz liczby watkow calkujacych: 10
time without pool: 38
time with pool: 13

Wykonanie programu dla licznosci proby: 1000 oraz liczby watkow calkujacych: 10
time without pool: 46
time with pool: 30

Wykonanie programu dla licznosci proby: 10000 oraz liczby watkow calkujacych: 10
time without pool: 174
time with pool: 156

Wykonanie programu dla licznosci proby: 100 oraz liczby watkow calkujacych: 1
time without pool: 4
time with pool: 3

Wykonanie programu dla licznosci proby: 1000 oraz liczby watkow calkujacych: 1
time without pool: 6
time with pool: 5

Wykonanie programu dla licznosci proby: 10000 oraz liczby watkow calkujacych: 1
time without pool: 24
time with pool: 22
*/


int main()
{
	//zad1
	test_zad1(5);

	//zad2 - measurements for several dataCounts and threadCounts
	/*test_zad2(100, 100);
	test_zad2(1000, 100);
	test_zad2(10000, 100);
	test_zad2(100, 10);
	test_zad2(1000, 10);
	test_zad2(10000, 10);
	test_zad2(100, 1);
	test_zad2(1000, 1);
	test_zad2(10000, 1);*/

	system("pause");
	return 0;
}

void test_zad1(int threadCount)
{
	//creating and starting threads
	vector<Producer*> producers;
	vector<Consumer*> consumers;
	for (int i = 0; i < threadCount; i++)
	{
		Producer* prod = new Producer();
		prod->Start();
		producers.push_back(prod);

		Consumer* cons = new Consumer(*prod);
		cons->Start();
		consumers.push_back(cons);
	}

	//waiting for press enter or escape
	const int esc_keyCode = 27;
	const int enter_keyCode = 13;
	while (true)
	{
		int c = _getch(); 

		if (c == esc_keyCode) //escape has been pressed
		{
			cout << "Zatrzymanie wszystkich watkow \n";
			for (Consumer* cust : consumers)
				cust->Join(); //Stop();
			for (Producer* prod : producers)
				prod->Join(); //Stop();
		}
		if (c == enter_keyCode) //enter has been pressed
		{
			cout << "Wyswietlenie sredniej wszystkich customerow \n";
			for (Consumer* cust : consumers)
				cout << cust->GetAvg() << endl;
		}
	}
}
void test_zad2(int dataSize, int threadCount)
{
	cout << "Wykonanie programu dla licznosci proby: " << dataSize << " oraz liczby watkow calkujacych: " << threadCount << endl;

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0.0, 100.0);

	//create data
	vector<double> data;
	for (int i = 0; i < dataSize; i++)
	{
		data.push_back(dist(mt));
	}


	//WITHOUT POOL METHOD
	auto startWithoutPool = std::chrono::system_clock::now(); //start time of measuring counting integral WITHOUT pool

	//create and start threads
	vector<Integrator*> integrators;
	while (integrators.size() < threadCount)
	{
		integrators.push_back(new Integrator());
		integrators.back()->Start();
		integrators.back()->Count(data, 1);
		integrators.back()->SetStatus(Integrator::Status::WORKING);
	}

	//wait until every thread stops working
	int workingCount = 0;
	do
	{
		workingCount = std::count_if(integrators.begin(), integrators.end(), isIntegratorWorking);
	} while (workingCount > 0);

	//disable and delete threads
	while (integrators.size() > 0)
	{
		integrators.back()->Join();
		delete integrators.back();
		integrators.pop_back();
	}

	auto stopWithoutPool = std::chrono::system_clock::now(); //stop time of measuring counting integral WITHOUT pool



	//WITH POOL METHOD
	auto startWithPool = std::chrono::system_clock::now(); //start time of measuring counting integral WITH pool

	//create and start threads
	IntegratorPool* pool = new IntegratorPool(threadCount);
	for (int i = 0; i < threadCount; i++)
	{
		Integrator* instance = pool->GetInstance();
		instance->Count(data, 1);
		instance->SetStatus(Integrator::Status::WORKING);
	}

	//wait until every thread stops working
	while (pool->GetLoad() > 0);

	auto stopWithPool = std::chrono::system_clock::now(); //stop time of measuring counting integral WITH pool


	//SHOW TIME MEASUREMENTS
	double timeWithoutPool = chrono::duration_cast<chrono::milliseconds>(stopWithoutPool - startWithoutPool).count(); //ms
	cout << "time without pool: " << timeWithoutPool << endl;

	double poolTime = chrono::duration_cast<chrono::milliseconds>(stopWithPool - startWithPool).count(); //ms
	cout << "time with pool: " << poolTime << endl;
}
bool isIntegratorWorking(Integrator* integrator)
{
	return integrator->GetStatus() == Integrator::Status::WORKING;
}

