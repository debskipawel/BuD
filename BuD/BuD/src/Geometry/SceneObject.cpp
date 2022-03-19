#include "SceneObject.h"

namespace BuD
{
	SceneObject::SceneObject()
	{
		InsertObject();
	}

	SceneObject* SceneObject::Get(uint32_t id)
	{
		return s_objectMap[id];
	}

	const std::unordered_map<uint32_t, SceneObject*>& SceneObject::GetAll()
	{
		return s_objectMap;
	}

	void SceneObject::InsertObject()
	{
		uint32_t id = {};

		if (s_availableIds.empty())
		{
			id = s_nextId++;
		}
		else
		{
			id = s_availableIds.top();
			s_availableIds.pop();
		}

		m_id = id;

		s_objectMap[id] = this;
	}

	uint32_t SceneObject::s_nextId = 1;
	std::stack<uint32_t> SceneObject::s_availableIds = std::stack<uint32_t>();
	std::unordered_map<uint32_t, SceneObject*> SceneObject::s_objectMap = std::unordered_map<uint32_t, SceneObject*>();
}
