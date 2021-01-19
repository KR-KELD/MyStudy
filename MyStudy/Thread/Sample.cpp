#include <iostream>
#include <thread>

int g_iSum = 0;

void counter(int id, int size)
{
	for (int i = 0; i < size; i++)
	{
		g_iSum += id;
		std::cout << id << std::endl;
	}
}

class Counter
{
public:
	int m_id;
	int m_size;
	Counter(int id, int size)
	{
		m_id = id;
		m_size = size;
	}
	void loop()
	{
		for (int i = 0; i < m_size; i++)
		{
			std::cout << m_id << std::endl;
		}
	}
};

void main()
{
	std::thread t1( counter, 99, 3 );
	std::thread t2{ counter, 88, 3 };
	//대기함수 쓰레드가 종료 할 때 까지 대기한다
	t1.join();
	t2.join();
	std::cout << g_iSum << std::endl;
	auto Lamda = 
	[](int id, int size) -> void
	{
		for (int i = 0; i < size; i++)
		{
			std::cout << id << std::endl;
		}
	};
	std::thread t3(Lamda, 77,3);
	t3.join();

	Counter c1(66, 3);
	std::thread t4(&Counter::loop,&c1);
	t4.join();
}
