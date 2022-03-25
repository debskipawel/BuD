#include "ObjectsCollection.h"

#include "SceneObject.h"

#include <algorithm>
#include <execution>
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
			[](Vector3 a, SceneObject* obj) { return a + obj->GetMesh()->m_position; });
		
		return centroid / static_cast<float>(m_objects.size());
	}
	
	void ObjectsCollection::RotateAroundCentroid(Vector3 rotation) const
	{
		auto centroid = Centroid();
		auto transform = Matrix::CreateRotationX(DirectX::XMConvertToRadians(rotation.x))
			* Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotation.y))
			* Matrix::CreateRotationZ(DirectX::XMConvertToRadians(rotation.z));

		auto model = Matrix::CreateTranslation(-centroid) * transform * Matrix::CreateTranslation(centroid);

		std::for_each(std::execution::par, m_objects.begin(), m_objects.end(),
			[&model](SceneObject* object) 
			{ 
				auto mesh = object->GetMesh();
				auto matrix = mesh->GetModelMatrix() * model;

				Vector3 position, scale;
				Quaternion rotQuat;
				matrix.Decompose(scale, rotQuat, position);

				object->MoveTo(position);
				object->RotateTo(rotQuat);
				object->ScaleTo(scale);
			});
	}

	void ObjectsCollection::ScaleAroundCentroid(Vector3 scale) const
	{
		auto centroid = Centroid();
		auto transform = Matrix::CreateScale(scale);

		auto model = Matrix::CreateTranslation(-centroid) * transform * Matrix::CreateTranslation(centroid);

		std::for_each(std::execution::par, m_objects.begin(), m_objects.end(),
			[&model](SceneObject* object)
			{
				auto mesh = object->GetMesh();
				auto matrix = mesh->GetModelMatrix() * model;

				Vector3 position, scale;
				Quaternion rotQuat;
				matrix.Decompose(scale, rotQuat, position);

				object->MoveTo(position);
				object->ScaleTo(scale);
			});
	}

	void ObjectsCollection::MoveAll(Vector3 translation) const
	{
		std::for_each(std::execution::par, m_objects.begin(), m_objects.end(),
			[translation](SceneObject* object)
			{
				object->GetMesh()->m_position += translation;
			});
	}
}
