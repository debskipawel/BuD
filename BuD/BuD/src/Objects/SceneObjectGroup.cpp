#include "SceneObjectGroup.h"

#include <algorithm>
#include <execution>
#include <numeric>

namespace BuD
{
	SceneObjectsGroup::SceneObjectsGroup(const std::map<uint32_t, std::shared_ptr<SceneObject>>& objects)
		: m_objects(objects), m_objectsType(static_cast<ObjectFlags>(0))
	{
		auto val = static_cast<ObjectFlags>(0);

		m_objectsType = std::accumulate(m_objects.begin(), m_objects.end(), val,
			[](ObjectFlags type, std::pair<uint32_t, std::shared_ptr<SceneObject>> record) { return type | record.second->GetFlags(); }
		);
	}
	
	void SceneObjectsGroup::Add(std::shared_ptr<SceneObject> object)
	{
		m_objects.insert(std::make_pair(object->Id(), object));
		m_objectsType = m_objectsType | object->GetFlags();
	}
	
	void SceneObjectsGroup::Remove(std::shared_ptr<SceneObject> object)
	{
		m_objects.erase(object->Id());

		auto val = static_cast<ObjectFlags>(0);

		m_objectsType = std::accumulate(m_objects.begin(), m_objects.end(), val,
			[](ObjectFlags type, std::pair<uint32_t, std::shared_ptr<SceneObject>> record) { return type | record.second->GetFlags(); }
		);
	}

	Vector3 SceneObjectsGroup::Centroid() const
	{
		Vector3 result = { 0.0f, 0.0f, 0.0f };

		auto sum = std::accumulate(m_objects.begin(), m_objects.end(), result,
			[](Vector3 pos, std::pair<uint32_t, std::shared_ptr<SceneObject>> record) { return pos + record.second->Position(); }
		);

		return sum / m_objects.size();
	}

	void SceneObjectsGroup::RotateAroundCentroid(Vector3 rotation) const
	{
		auto centroid = Centroid();
		auto transform = Matrix::CreateRotationX(DirectX::XMConvertToRadians(rotation.x))
			* Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotation.y))
			* Matrix::CreateRotationZ(DirectX::XMConvertToRadians(rotation.z));

		auto model = Matrix::CreateTranslation(-centroid) * transform * Matrix::CreateTranslation(centroid);

		std::for_each(std::execution::par, m_objects.begin(), m_objects.end(),
			[&model](std::pair<uint32_t, std::shared_ptr<SceneObject>> record)
			{
				auto& object = record.second;
				auto matrix = object->ModelMatrix() * model;

				Vector3 position, scale;
				Quaternion rotQuat;
				matrix.Decompose(scale, rotQuat, position);

				object->MoveTo(position);
				object->RotateTo(rotQuat);
			}
		);

		std::for_each(m_objects.begin(), m_objects.end(),
			[](std::pair<uint32_t, std::shared_ptr<SceneObject>> r) { r.second->OnUpdate(); }
		);
	}

	void SceneObjectsGroup::ScaleAroundCentroid(Vector3 scale) const
	{
		auto centroid = Centroid();
		auto transform = Matrix::CreateScale(scale);

		auto model = Matrix::CreateTranslation(-centroid) * transform * Matrix::CreateTranslation(centroid);

		std::for_each(std::execution::par, m_objects.begin(), m_objects.end(),
			[&model](std::pair<uint32_t, std::shared_ptr<SceneObject>> record)
			{
				auto& object = record.second;
				auto matrix = object->ModelMatrix() * model;

				Vector3 position, scale;
				Quaternion rotQuat;
				matrix.Decompose(scale, rotQuat, position);

				object->MoveTo(position);
				object->ScaleTo(scale.LengthSquared() ? scale : Vector3{ 0.01f, 0.01f, 0.01f });
			}
		);

		std::for_each(m_objects.begin(), m_objects.end(),
			[](std::pair<uint32_t, std::shared_ptr<SceneObject>> r) { r.second->OnUpdate(); }
		);
	}
	
	void SceneObjectsGroup::MoveAll(Vector3 translation) const
	{
		for (auto& [id, obj] : m_objects)
		{
			obj->MoveBy(translation, false);
		}

		std::for_each(m_objects.begin(), m_objects.end(),
			[](std::pair<uint32_t, std::shared_ptr<SceneObject>> r) { r.second->OnUpdate(); }
		);
	}
}
