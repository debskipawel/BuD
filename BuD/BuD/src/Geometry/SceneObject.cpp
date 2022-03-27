#include "SceneObject.h"

#include "ObjectsCollection.h"

namespace BuD
{
	SceneObject::SceneObject()
	{
		InsertObject();
	}

	SceneObject::~SceneObject()
	{
		DeleteObject(m_id);
	}

	SceneObject* SceneObject::Get(uint32_t id)
	{
		auto res = s_objectMap.find(id);

		if (res == s_objectMap.end())
		{
			return nullptr;
		}

		return res->second;
	}

	const std::unordered_map<uint32_t, SceneObject*>& SceneObject::GetAll()
	{
		return s_objectMap;
	}

	void SceneObject::DeleteObject(uint32_t id)
	{
		auto res = s_objectMap.find(id);

		if (res == s_objectMap.end())
		{
			return;
		}

		res->second->m_shouldBeDeleted = true;
		s_selected.Remove(res->second);
		s_objectMap.erase(id);
		s_availableIds.push(id);
	}

	void SceneObject::MoveTo(const Vector3& position)
	{
		for (auto& mesh : m_meshes)
		{
			mesh->m_position = position;
		}
	}

	void SceneObject::MoveBy(const Vector3& difference)
	{
		for (auto& mesh : m_meshes)
		{
			mesh->m_position += difference;
		}
	}

	void SceneObject::ScaleTo(const Vector3& scale)
	{
		for (auto& mesh : m_meshes)
		{
			mesh->m_scale = scale;
		}
	}

	void SceneObject::RotateTo(const Quaternion& quaternion)
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
	}

	void SceneObject::RotateTo(const Vector3& rotation)
	{
		Vector3 radianRot =
		{
			DirectX::XMConvertToRadians(rotation.x),
			DirectX::XMConvertToRadians(rotation.y),
			DirectX::XMConvertToRadians(rotation.z),
		};

		auto rotate = Matrix::CreateRotationX(radianRot.x) * dxm::Matrix::CreateRotationY(radianRot.y) * dxm::Matrix::CreateRotationZ(radianRot.z);
		auto quatRotation = Quaternion::CreateFromRotationMatrix(rotate);

		for (auto& mesh : m_meshes)
		{
			mesh->m_rotation = rotation;
			mesh->m_quatRotation = quatRotation;
		}
	}

	void SceneObject::Select()
	{
		m_selected = true;
		s_selected.Add(this);
	}

	void SceneObject::Unselect()
	{
		m_selected = false;
		s_selected.Remove(this);
	}

	void SceneObject::InsertObject()
	{
		uint32_t id = {};

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

		s_objectMap[id] = this;
	}

	uint32_t SceneObject::s_nextId = 1;
	std::stack<uint32_t> SceneObject::s_availableIds = std::stack<uint32_t>();
	std::unordered_map<uint32_t, SceneObject*> SceneObject::s_objectMap = std::unordered_map<uint32_t, SceneObject*>();
	ObjectsCollection SceneObject::s_selected = ObjectsCollection();
}
