#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>
#include <functional>

int Compute()
{
	int iSum = 0;
	for (int i = 0; i < 10000000; i++)
	{
		iSum += 1;
	}
	return iSum;
}
std::once_flag g_Flag;
void FuncThread(int id)
{
	auto start = std::chrono::system_clock::now();
	std::call_once(g_Flag, Compute);
	auto end = std::chrono::system_clock::now();
	double fTime = std::chrono::duration<double>(end - start).count();
	std::cout << "Thread -> " << id << " : Finished  -" << fTime << "sec" << std::endl;
}

class sss
{
public:
	sss(int d)
	{

	}
public:
	void Init()
	{

	}
};

void main()
{

	std::cout.sync_with_stdio(true);
	sss asd(3);
	std::thread t{ &sss::Init, &asd };
	std::vector<std::thread> thList;
	for (int i = 0; i < 4; i++)
	{
		thList.emplace_back(FuncThread, i);
		thList.back().detach();
	}
	std::this_thread::sleep_for(std::chrono::seconds(3));
}