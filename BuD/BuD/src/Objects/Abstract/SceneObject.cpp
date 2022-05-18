#include "SceneObject.h"

#include <Objects/Scene.h>

namespace BuD
{
	uint32_t SceneObject::s_nextId = 1;
	std::stack<uint32_t> SceneObject::s_availableIds;

	SceneObject::SceneObject(Scene& scene)
		: m_selected(false), m_scene(scene)
	{
		uint32_t id;

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
	}

	Matrix SceneObject::ModelMatrix() const
	{
		return m_meshes.size() > 0 ? m_meshes[0]->GetModelMatrix() : Matrix::Identity;
	}

	Vector3 SceneObject::Position() const
	{
		return m_meshes.size() > 0 ? m_meshes[0]->m_position : Vector3{ 0.0f, 0.0f, 0.0f };
	}

	void SceneObject::MoveTo(const Vector3& position, bool propagateUpdate)
	{
		for (auto& mesh : m_meshes)
		{
			mesh->m_position = position;
		}

		if (propagateUpdate)
		{
			OnUpdate();
		}
	}

	void SceneObject::MoveBy(const Vector3& difference, bool propagateUpdate)
	{
		for (auto& mesh : m_meshes)
		{
			mesh->m_position += difference;
		}

		if (propagateUpdate)
		{
			OnUpdate();
		}
	}

	void SceneObject::ScaleTo(const Vector3& scale, bool propagateUpdate)
	{
		for (auto& mesh : m_meshes)
		{
			mesh->m_scale = scale;
		}

		if (propagateUpdate)
		{
			OnUpdate();
		}
	}

	void SceneObject::RotateTo(const Quaternion& quaternion, bool propagateUpdate)
	{
		auto eulerRadian = quaternion.ToEuler();
		Vector3 rotation =
		{
			DirectX::XMConvertToDegrees(eulerRadian.x),
			DirectX::XMConvertToDegrees(eulerRadian.y),
			DirectX::XMConvertToDegrees(eulerRadian.z),
		};

		for (auto& mesh : m_meshes)
		{
			mesh->m_quatRotation = quaternion;
			mesh->m_rotation = rotation;
		}

		if (propagateUpdate)
		{
			OnUpdate();
		}
	}
	
	void SceneObject::RotateTo(const Vector3& rotation, bool propagateUpdate)
	{
		for (auto& mesh : m_meshes)
		{
			mesh->m_rotation = rotation;
			mesh->UpdateRotation();
		}

		if (propagateUpdate)
		{
			OnUpdate();
		}
	}
}