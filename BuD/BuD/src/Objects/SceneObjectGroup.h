#pragma once

#include "Abstract/SceneObject.h"

#include <map>
#include <memory>

namespace BuD
{
	class SceneObjectsGroup
	{
	public:
		SceneObjectsGroup(const std::map<uint32_t, std::shared_ptr<SceneObject>>& objects);

		inline size_t Count() const { return m_objects.size(); }
		inline ObjectFlags GetType() const { return m_objectsType; }
		
		void Add(std::shared_ptr<SceneObject> object);
		void Remove(std::shared_ptr<SceneObject> object);

		inline const std::map<uint32_t, std::shared_ptr<SceneObject>>& Objects() const { return m_objects; }

		Vector3 Centroid() const;

		void RotateAroundCentroid(Vector3 rotation) const;
		void ScaleAroundCentroid(Vector3 scale) const;
		void MoveAll(Vector3 translation) const;

	protected:
		ObjectFlags m_objectsType;

	protected:
		std::map<uint32_t, std::shared_ptr<SceneObject>> m_objects;
	};
}
