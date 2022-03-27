#pragma once

#include "GeometryType.h"

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

		inline GeometryType GetType() const { return m_objectsType; }
		inline const std::vector<SceneObject*>& Objects() const { return m_objects; }

		Vector3 Centroid() const;

		void RotateAroundCentroid(Vector3 rotation) const;
		void ScaleAroundCentroid(Vector3 scale) const;
		void MoveAll(Vector3 translation) const;

	protected:
		GeometryType m_objectsType = GeometryType::EMPTY;
		std::vector<SceneObject*> m_objects;
	};
}
