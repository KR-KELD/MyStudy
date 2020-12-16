#pragma once
#include <string>
//시스템 메모리 -> 연속
//가상 메모리 -> 연속(동적 할당)
//메모리가 연속적으로 할당 되어야 한다[정렬]

//기본적으로 할당되어서 사용이 됐던 메모리를
//할당 해제하지 않고 풀에 담아뒀다가 재사용하는 기법
//미리 할당 할 경우 비사용 객체를 꺼내서 사용하고
//다 쓰면 다시 풀에 넣는 형태

template<class T>
class myObjectPool
{

public:
	enum
	{
		POOL_MAX_SIZE = 1024,	//2의 n승
		POOL_SIZE_MASK = POOL_MAX_SIZE - 1,
	};

	static void PrepareAllocation()
	{
		for (int i = 0; i < POOL_MAX_SIZE; i++)
		{
			m_mPool[i] = _aligned_malloc(sizeof(T), MEMORY_ALLOCATION_ALIGNMENT);
		}
		InterlockedAdd(&m_TailPos, POOL_MAX_SIZE);
	}
	static void AllFree()
	{
		for (int i = 0; i < POOL_MAX_SIZE; i++)
		{
			//첫 번째 인자에 두 번째 인자를 대입하고 첫 번째 인자의 주소를 반환한다
			void* val = InterlockedExchangePointer(&m_mPool[i], nullptr);
			if (val != nullptr)
			{
				_aligned_free(m_mPool[i]);
			}
		}
	}

	static void* operator new(size_t size)
	{
		//보호받으면서 값을 1 증가시키는 함수
		long long pos = InterlockedIncrement64(&m_HeadPos) - 1; //m_HeadPos++;
		//비트연산으로 최대값 설정
		//최대값을 넘어가면 0부터 다시 시작
		long long valpos = pos & POOL_SIZE_MASK;
		//배열에 널을 넣고 들어있는걸 반환받아라
		void* val = InterlockedExchangePointer(&m_mPool[valpos], nullptr);
		if (val != nullptr)
		{
			return val;
		}
		return _aligned_malloc(size, MEMORY_ALLOCATION_ALIGNMENT);
	}
	static void operator delete(void* obj)
	{
		//보호받으면서 값을 1 증가시키는 함수
		long long pos = InterlockedIncrement64(&m_TailPos) - 1; //m_HeadPos++;
		//비트연산으로 최대값 설정
		long long valpos = pos & POOL_SIZE_MASK;
		void* val = InterlockedExchangePointer(&m_mPool[valpos], obj);
		if (val != nullptr)
		{
			_aligned_free(val);
		}
	}
private:
	
	static void* volatile m_mPool[POOL_MAX_SIZE];
	// volatile => 변동이 있을 수 있다 컴파일러에게 명시해줌
	static long long volatile m_HeadPos;
	static long long volatile m_TailPos;
};

template<typename T>
void* volatile myObjectPool<T>::m_mPool[POOL_MAX_SIZE] = {};
template<typename T>
long long volatile myObjectPool<T>::m_HeadPos(0);
template<typename T>
long long volatile myObjectPool<T>::m_TailPos(0);