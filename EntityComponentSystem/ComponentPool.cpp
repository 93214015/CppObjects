#include "ComponentPool.h"

namespace ECS
{

	ComponentPool::ComponentPool(size_t _ComponentSize)
		: m_ComponentSize(_ComponentSize), m_PoolSize(0)
	{
		m_Data = new char[_ComponentSize * MAX_COMPONENTS];
	}

	ComponentPool::ComponentPool(ComponentPool&& _ComponentPool) noexcept
		: m_Data(_ComponentPool.m_Data), m_ComponentSize(_ComponentPool.m_ComponentSize), m_PoolSize(_ComponentPool.m_PoolSize)
	{
		_ComponentPool.m_Data = nullptr;
		_ComponentPool.m_PoolSize = 0;
	}

	ComponentPool& ComponentPool::operator=(ComponentPool&& _ComponentPool) noexcept
	{
		m_Data = _ComponentPool.m_Data;
		m_ComponentSize = _ComponentPool.m_ComponentSize;
		m_PoolSize = _ComponentPool.m_PoolSize;

		_ComponentPool.m_Data = nullptr;
		_ComponentPool.m_PoolSize = 0;

		return *this;
	}

	ComponentPool::~ComponentPool()
	{
		delete[] m_Data;
	}

	void* ComponentPool::Get(size_t _Index)
	{
		return m_Data + (_Index * m_ComponentSize);
	}

	void* ComponentPool::Add()
	{
		void* _NewPtr = m_Data + (m_PoolSize * m_ComponentSize);
		m_PoolSize++;
		return _NewPtr;
	}

}
