// Multithreading.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <mutex>
#include "Windows.h"
#include <queue>
#include <condition_variable>


using namespace std;

mutex mu;
queue<int> kolejka;
condition_variable con;

void Dodaj(int& i)
{
	lock_guard<mutex> lock(mu);
	kolejka.push(i);
	con.notify_one();
	
}



void Producent()
{
	for(int i=0;i<1000;i++)
	{
		Dodaj(i);
	}
	
	
}

void Zdejmij()
{
	unique_lock<mutex> lock(mu);
	con.wait(lock,[](){return !kolejka.empty();});
	cout<<kolejka.front()<<endl;
	kolejka.pop();
}

void Konsument()
{
	while(true)
		Zdejmij();
}


int _tmain(int argc, _TCHAR* argv[])
{
	thread t1(Producent);
	thread t2(Konsument);

	t1.join();
	t2.join();
	
	

	system("PAUSE");
	return 0;
}

