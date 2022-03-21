#pragma once

#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

namespace BuD
{
	class SceneObject;

	class ObjectsCollection
	{
	public:
		ObjectsCollection() = default;

		inline size_t Count() const { return m_objects.size(); }
		void Add(SceneObject* object);
		void Remove(SceneObject* object);

		inline const std::vector<SceneObject*>& Objects() const { return m_objects; }

		Vector3 Centroid() const;

	protected:
		std::vector<SceneObject*> m_objects;
	};
}
