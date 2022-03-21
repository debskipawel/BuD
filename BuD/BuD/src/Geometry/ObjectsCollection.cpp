#include "ObjectsCollection.h"

#include "SceneObject.h"

#include <numeric>

namespace BuD
{
	void ObjectsCollection::Add(SceneObject* object)
	{
		m_objects.push_back(object);
	}
	
	void ObjectsCollection::Remove(SceneObject* object)
	{
		auto res = std::find(m_objects.begin(), m_objects.end(), object);
		
		if (res == m_objects.end())
		{
			return;
		}

		m_objects.erase(res);
	}
	
	Vector3 ObjectsCollection::Centroid() const
	{
		auto centroid = std::accumulate(m_objects.begin(), m_objects.end(), Vector3{ 0.0f, 0.0f, 0.0f }, 
			[](Vector3 a, SceneObject* obj) { return a + obj->GetModel()->m_position; });
		
		return centroid / static_cast<float>(m_objects.size());
	}
}
