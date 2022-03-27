#pragma once

#include <SimpleMath.h>

#include <set>

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

		inline const std::set<SceneObject*>& Objects() const { return m_objects; }

		Vector3 Centroid() const;

		void RotateAroundCentroid(Vector3 rotation) const;
		void ScaleAroundCentroid(Vector3 scale) const;
		void MoveAll(Vector3 translation) const;

	protected:
		std::set<SceneObject*> m_objects;
	};
}
