#pragma once

namespace ECS
{

	class ComponentPool final
	{
	public:
		ComponentPool(size_t _ComponentSize);

		//Copy Contructors are deleted
		ComponentPool(const ComponentPool&) = delete;
		ComponentPool& operator=(const ComponentPool&) = delete;

		//Move Constructor
		ComponentPool(ComponentPool&& _ComponentPool) noexcept;

		ComponentPool& operator=(ComponentPool&& _ComponentPool) noexcept;

		~ComponentPool();

		void* Get(size_t _Index);

		void* Add();

	private:
		char* m_Data;
		size_t m_ComponentSize;
		size_t m_PoolSize;

		static constexpr size_t MAX_COMPONENTS = 1024;
	};

}