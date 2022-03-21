#include "SceneObject.h"

#include "ObjectsCollection.h"

namespace BuD
{
	SceneObject::SceneObject()
	{
		InsertObject();
	}

	SceneObject::~SceneObject()
	{
		RemoveObject();
	}

	SceneObject* SceneObject::Get(uint32_t id)
	{
		auto res = s_objectMap.find(id);

		if (res == s_objectMap.end())
		{
			return nullptr;
		}

		return res->second;
	}

	const std::unordered_map<uint32_t, SceneObject*>& SceneObject::GetAll()
	{
		return s_objectMap;
	}

	void SceneObject::DeleteObject(uint32_t id)
	{
		auto res = s_objectMap.find(id);

		if (res == s_objectMap.end())
		{
			return;
		}

		s_selected.Remove(res->second);
		s_objectMap.erase(id);
	}

	void SceneObject::Select()
	{
		m_selected = true;
		s_selected.Add(this);
	}

	void SceneObject::Unselect()
	{
		m_selected = false;
		s_selected.Remove(this);
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

	void SceneObject::RemoveObject()
	{
		s_objectMap.erase(m_id);
	}

	uint32_t SceneObject::s_nextId = 1;
	std::stack<uint32_t> SceneObject::s_availableIds = std::stack<uint32_t>();
	std::unordered_map<uint32_t, SceneObject*> SceneObject::s_objectMap = std::unordered_map<uint32_t, SceneObject*>();
	ObjectsCollection SceneObject::s_selected = ObjectsCollection();
}
