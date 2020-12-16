#pragma once
#include <string>
//�ý��� �޸� -> ����
//���� �޸� -> ����(���� �Ҵ�)
//�޸𸮰� ���������� �Ҵ� �Ǿ�� �Ѵ�[����]

//�⺻������ �Ҵ�Ǿ ����� �ƴ� �޸𸮸�
//�Ҵ� �������� �ʰ� Ǯ�� ��Ƶ״ٰ� �����ϴ� ���
//�̸� �Ҵ� �� ��� ���� ��ü�� ������ ����ϰ�
//�� ���� �ٽ� Ǯ�� �ִ� ����

template<class T>
class myObjectPool
{

public:
	enum
	{
		POOL_MAX_SIZE = 1024,	//2�� n��
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
			//ù ��° ���ڿ� �� ��° ���ڸ� �����ϰ� ù ��° ������ �ּҸ� ��ȯ�Ѵ�
			void* val = InterlockedExchangePointer(&m_mPool[i], nullptr);
			if (val != nullptr)
			{
				_aligned_free(m_mPool[i]);
			}
		}
	}

	static void* operator new(size_t size)
	{
		//��ȣ�����鼭 ���� 1 ������Ű�� �Լ�
		long long pos = InterlockedIncrement64(&m_HeadPos) - 1; //m_HeadPos++;
		//��Ʈ�������� �ִ밪 ����
		//�ִ밪�� �Ѿ�� 0���� �ٽ� ����
		long long valpos = pos & POOL_SIZE_MASK;
		//�迭�� ���� �ְ� ����ִ°� ��ȯ�޾ƶ�
		void* val = InterlockedExchangePointer(&m_mPool[valpos], nullptr);
		if (val != nullptr)
		{
			return val;
		}
		return _aligned_malloc(size, MEMORY_ALLOCATION_ALIGNMENT);
	}
	static void operator delete(void* obj)
	{
		//��ȣ�����鼭 ���� 1 ������Ű�� �Լ�
		long long pos = InterlockedIncrement64(&m_TailPos) - 1; //m_HeadPos++;
		//��Ʈ�������� �ִ밪 ����
		long long valpos = pos & POOL_SIZE_MASK;
		void* val = InterlockedExchangePointer(&m_mPool[valpos], obj);
		if (val != nullptr)
		{
			_aligned_free(val);
		}
	}
private:
	
	static void* volatile m_mPool[POOL_MAX_SIZE];
	// volatile => ������ ���� �� �ִ� �����Ϸ����� �������
	static long long volatile m_HeadPos;
	static long long volatile m_TailPos;
};

template<typename T>
void* volatile myObjectPool<T>::m_mPool[POOL_MAX_SIZE] = {};
template<typename T>
long long volatile myObjectPool<T>::m_HeadPos(0);
template<typename T>
long long volatile myObjectPool<T>::m_TailPos(0);