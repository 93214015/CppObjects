#pragma once

#include <bitset>
#include <vector>
#include <memory>
#include <assert.h>



namespace ECS
{

	using EntityIndex = uint32_t;
	using EntityVersion = uint32_t;

	using ComponentMask = std::bitset<32>;

	extern size_t gComponentIDCounter;

	struct EntityID
	{
		EntityIndex Index;
		EntityVersion Version;

		explicit operator uint64_t() const
		{
			return (static_cast<uint64_t>(Index) << 32) | Version;
		}

		EntityID& operator++()
		{
			++Index;
			return *this;
		}

		EntityID operator++(int)
		{
			EntityID _Old = *this;
			operator++();
			return _Old;
		}

		bool operator==(const EntityID& _ID)
		{
			return static_cast<uint64_t>(*this) == static_cast<uint64_t>(_ID);
		}

		bool operator!=(const EntityID& _ID)
		{
			return static_cast<uint64_t>(*this) != static_cast<uint64_t>(_ID);
		}
	};


	class ComponentPool final
	{
	public:
		ComponentPool(size_t _ComponentSize)
			: m_ComponentSize(_ComponentSize), m_PoolSize(0)
		{
			m_Data = new char[_ComponentSize * MAX_COMPONENTS];
		}

		//Copy Contructors are deleted
		ComponentPool(const ComponentPool&) = delete;
		ComponentPool& operator=(const ComponentPool&) = delete;

		//Move Constructor
		ComponentPool(ComponentPool&& _ComponentPool) noexcept
			: m_Data(_ComponentPool.m_Data), m_ComponentSize(_ComponentPool.m_ComponentSize), m_PoolSize(_ComponentPool.m_PoolSize)
		{
			_ComponentPool.m_Data = nullptr;
			_ComponentPool.m_PoolSize = 0;
		}

		ComponentPool& operator=(ComponentPool&& _ComponentPool) noexcept
		{
			m_Data = _ComponentPool.m_Data;
			m_ComponentSize = _ComponentPool.m_ComponentSize;
			m_PoolSize = _ComponentPool.m_PoolSize;

			_ComponentPool.m_Data = nullptr;
			_ComponentPool.m_PoolSize = 0;

		}


		~ComponentPool()
		{
			delete[] m_Data;
		}

		void* Get(size_t _Index)
		{
			return m_Data + (_Index * m_ComponentSize);
		}

		void* Add()
		{
			void* _NewPtr = m_Data + (m_PoolSize * m_ComponentSize);
			m_PoolSize++;
			return _NewPtr;
		}

	private:
		char* m_Data;
		size_t m_ComponentSize;
		size_t m_PoolSize;

		static constexpr size_t MAX_COMPONENTS = 1024;
	};


	class Scene
	{
	public:

		Scene() = default;
		~Scene() = default;

		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;

		Scene(Scene&&) noexcept = default;
		Scene& operator=(Scene&&) noexcept = default;

		struct EntityDesc
		{
			EntityID ID;
			ComponentMask Masks;
		};

		EntityID CreateEntity();

		template<class T>
		size_t GetID()
		{
			static size_t static_ComponentID = gComponentIDCounter++;
			return static_ComponentID;
		}


		bool IsEntityValid(EntityID _EntityID)
		{
			return m_Entities[_EntityID.Index].ID == _EntityID;
		}


		template<class T, typename... Args>
		void Assign(EntityID _EntityID, Args... _Args)
		{

			size_t _ComponentID = GetID<T>();
			if (_ComponentID >= m_ComponentPools.size())
			{
				m_ComponentPools.resize(_ComponentID + 1);
			}
			if (m_ComponentPools[_ComponentID] == nullptr)
			{
				m_ComponentPools[_ComponentID].reset(new ComponentPool(sizeof(T)));
			}

			ComponentPool* _ComponentPool = m_ComponentPools[_ComponentID].get();

			T* _NewComponent = new (_ComponentPool->Get(_EntityID.Index)) T(std::forward<Args>(_Args)...);

			m_Entities[_EntityID.Index].Masks.set(_ComponentID, true);
		}

		template<class T>
		void Remove(EntityID _EntityID)
		{

			if (!IsEntityValid(_EntityID))
			{
				assert(false);
				return; // The EntityID is invalid. It's been probably deleted
			}

			m_Entities[_EntityID.Index].Masks.reset(GetID<T>());
		}

		void DestroyEntity(EntityID _ID)
		{
			//m_Entities[_ID.Index].ID.Index = -1; // I doubt it's necessary to set the Index -1. the version modifying probably is just enough.
			m_Entities[_ID.Index].ID.Version += 1;
			m_Entities[_ID.Index].Masks.reset();
			m_FreeEntityIndecies.emplace_back(_ID.Index);
		}

	private:
		EntityID m_CurrentEntityID{ 0, 0 };

		std::vector<EntityDesc> m_Entities;

		std::vector<EntityIndex> m_FreeEntityIndecies;

		std::vector<std::unique_ptr<ComponentPool>> m_ComponentPools;
	};


}
