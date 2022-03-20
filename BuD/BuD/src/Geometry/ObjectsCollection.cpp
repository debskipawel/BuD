#include "ObjectsCollection.h"

#include "SceneObject.h"

namespace BuD
{
	void ObjectsCollection::Add(SceneObject* object)
	{
		if (m_objects.size() == 0)
		{
			m_centroid = { 0.0f, 0.0f, 0.0f };
		}

		m_centroid = (static_cast<float>(m_objects.size()) * m_centroid + object->GetModel()->m_position) / static_cast<float>(m_objects.size() + 1);
		m_objects.push_back(object);
	}
	
	void ObjectsCollection::Remove(SceneObject* object)
	{
		auto res = std::find(m_objects.begin(), m_objects.end(), object);
		
		if (res == m_objects.end())
		{
			return;
		}

		m_centroid = (static_cast<float>(m_objects.size()) * m_centroid - object->GetModel()->m_position) / static_cast<float>(m_objects.size() - 1);
		m_objects.erase(res);
	}
	
	Vector3 ObjectsCollection::Centroid() const
	{
		return m_centroid;
	}
}
