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

		void RotateAroundCentroid(Vector3 rotation) const;
		void ScaleAroundCentroid(Vector3 scale) const;
		void MoveAll(Vector3 translation) const;

	protected:
		std::vector<SceneObject*> m_objects;
	};
}
