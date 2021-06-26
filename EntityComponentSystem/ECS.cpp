#include "ECS.h"

#include <cstdint>

namespace ECS
{
	size_t gComponentIDCounter;

	EntityID Scene::CreateEntity()
	{
		if (!m_FreeEntityIndecies.empty())
		{
			EntityIndex _Index = m_FreeEntityIndecies.back();
			m_FreeEntityIndecies.pop_back();

			EntityID _NewID{ _Index, m_Entities[_Index].ID.Version };
		}


		m_Entities.emplace_back(EntityDesc{ m_CurrentEntityID, ComponentMask{} });
		return m_CurrentEntityID++;
	}
}

