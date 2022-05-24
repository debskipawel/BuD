#include <pch.h>

#include "SceneObject.h"

namespace MG1
{
	bool SceneObject::SetId(uint32_t id)
	{
		auto searchResult = s_takenIds.find(id);

		if (searchResult != s_takenIds.end())
		{
			return false;
		}

		s_takenIds.insert(id);
		s_freedIds.push(m_id);

		m_id = id;

		return true;
	}

	SceneObject::SceneObject()
	{
		if (s_freedIds.empty())
		{
			m_id = s_firstFreeId++;
		}
		else
		{
			m_id = s_freedIds.top();
			s_freedIds.pop();
		}

		s_takenIds.insert(m_id);
	}

	uint32_t SceneObject::s_firstFreeId = 0;
	std::set<uint32_t> SceneObject::s_takenIds = {};
	std::stack<uint32_t> SceneObject::s_freedIds = {};
}
