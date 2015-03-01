// SaveMultithreadingClass.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>

using namespace std;



class SaveQueue
{
	mutex mu;
	queue<int> kolejka;
	condition_variable con;
public:
	void Try_Push(int &a )
	{
		lock_guard<mutex> locker(mu);
		cout<<"Pushing "<<a<<" by T1"<<endl;
		kolejka.push(a);
		con.notify_one();

	}
	int Try_Pop()
	{
		unique_lock<mutex> locker(mu);
		con.wait(locker,[this](){return !this->kolejka.empty(); });
		int val= kolejka.front();
		kolejka.pop();
		locker.unlock();
		cout<<"Poping "<<val<<" by T2"<<endl;
		return val;
	}



};


void Customer(SaveQueue & obj)
{
	int i;
	while(cin>>i)
	{
		obj.Try_Push(i);
		
	}

}

void Producer(SaveQueue & obj)
{

	while(true)
	{
		obj.Try_Pop();

	}

}


int _tmain(int argc, _TCHAR* argv[])
{
	SaveQueue obj;
	thread t1(Customer,ref(obj));
	thread t2(Producer,ref(obj));
	

	t1.join();
	t2.join();

	system("PAUSE");

	return 0;
}

