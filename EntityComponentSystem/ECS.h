#pragma once

#include <bitset>
#include <vector>
#include <memory>
#include <assert.h>

#include "ComponentPool.h"


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

		template<class... Ts>
		friend struct SceneView;
	};


	template<class... Ts>
	struct SceneView
	{
		struct Iterator
		{
			Iterator(Scene* _Scene, size_t _Index, ComponentMask _ComponentMask)
				: m_Scene(_Scene), m_Index(_Index), m_ComponentMask(_ComponentMask)
			{}

			EntityID operator*() const
			{
				return m_Scene->m_Entities[m_Index].ID;
			}

			bool operator==(const Iterator& _OtherIterator)
			{
				return m_Index == _OtherIterator.m_Index;
			}

			bool operator!=(const Iterator& _OtherIterator)
			{
				return m_Index != _OtherIterator.m_Index;
			}

			Iterator& operator++()
			{
				do
				{
					m_Index++;

				} while (m_Index < m_Scene->m_Entities.size() && !IsValidIndex(m_Index));

				return *this;
			}
			
		private:

			inline bool IsValidIndex(size_t _Index) const
			{
				const Scene::EntityDesc& _EntityDesc = m_Scene->m_Entities[_Index];
				return _EntityDesc.ID.Index != -1 && ( (m_ComponentMask == 0) || (m_ComponentMask == (m_ComponentMask & _EntityDesc.Masks)) );
			}

		private:
			Scene* m_Scene;
			size_t m_Index;
			ComponentMask m_ComponentMask;

		};

		SceneView(Scene* _Scene)
			: m_Scene(_Scene)
		{
			for (size_t i = 0; i < sizeof...(Ts); ++i)
			{
				m_ComponentMask.set(i, true);
			}
		}

		Iterator begin() const
		{
			size_t _Index = 0;

			const auto& _Entities = m_Scene->m_Entities;

			while (_Index < _Entities.size() && (_Entities[_Index].ID.Index == -1 || (m_ComponentMask != 0 && _Entities[_Index].Masks != m_ComponentMask)))
			{
				_Index++;
			}


			return Iterator(m_Scene, _Index, m_ComponentMask);
		}

		Iterator end() const
		{
			return Iterator(m_Scene, m_Scene->m_Entities.size(), m_ComponentMask);
		}


	private:
		Scene* m_Scene;
		ComponentMask m_ComponentMask;
	};


}
