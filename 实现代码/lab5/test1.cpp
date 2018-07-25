#include <iostream>
#include <thread>
#include <mutex>
#include <ctime>
#include <random>
#include <vector>

using namespace std;

mutex loc;
int count;

void test()
{	
	while(1)
	{
		loc.lock();
		if(count>=100){
			loc.unlock();
			break;	
		}
		for(int i=0;i<5;i++)			
			cout << "thread " << this_thread::get_id() << " :   " << ++count << endl;
		loc.unlock();
	}
}

int main()
{
	cout<<"Main thread begin"<<endl;
	vector<thread> threadset;
	count = 0;

	for (int i = 0; i < 4; ++i)
	{
		threadset.push_back(thread(test));
	}
	for (auto& subthread : threadset)
		subthread.join();

	cout<<"Main thread finished"<<endl;
}