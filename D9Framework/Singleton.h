#pragma once
#include <assert.h>

template<class C>
class Singleton
{

public:
	static C* Create()
	{
		assert(Singleton<C>::m_currentInstance == 0);
		Singleton<C>::m_currentInstance = ::new C;
		return Singleton<C>::m_currentInstance;
	}
	static void Destroy()
	{
		delete Singleton<C>::m_currentInstance;
		m_currentInstance = 0;
	}
	static C* GetInstance()
	{
		return m_currentInstance;
	}

protected:
	static C* m_currentInstance;
};

#define DefineSingleton(C) C* Singleton<C>::m_currentInstance = 0;
