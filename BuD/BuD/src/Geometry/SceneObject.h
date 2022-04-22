#pragma once

#include "Scene/Mesh.h"

#include "ObjectsCollection.h"

#include <memory>
#include <stack>
#include <unordered_map>

namespace BuD
{
	class SceneObject
	{
	public:
		SceneObject();

		virtual bool DrawGui() = 0;

		virtual void MoveTo(const Vector3& position);
		virtual void MoveBy(const Vector3& difference);
		virtual void ScaleTo(const Vector3& scale);
		virtual void RotateTo(const Quaternion& quaternion);
		virtual void RotateTo(const Vector3& rotation);

		virtual GeometryType GetType() = 0;

		virtual Vector3 Position() { return m_meshes[0]->m_position; }

		virtual void Select();
		virtual void Unselect();

		virtual std::vector<Vector3> VirtualControlPoints() { return std::vector<Vector3>(); }

		inline std::shared_ptr<Mesh> GetMesh(uint32_t index) { return m_meshes[index]; }
		
		inline uint32_t MeshesCount() const	{ return m_meshes.size(); }
		inline uint32_t Id() const { return m_id; }
		
		inline std::string* Name() { return &m_tag; }
		inline virtual bool IsSelected() const { return m_selected; }
		inline virtual bool ShouldBeDeleted() const { return m_shouldBeDeleted; }

		static SceneObject* Get(uint32_t id);
		static const std::unordered_map<uint32_t, SceneObject*>& GetAll();
		static void DeleteObject(uint32_t id);
		inline static const ObjectsCollection& GetSelected() { return s_selected; }

	protected:
		void InsertObject();

		bool m_selected = false;
		bool m_shouldBeDeleted = false;

		uint32_t m_id;
		std::string m_tag = "Unnamed object";
		std::vector<std::shared_ptr<Mesh>> m_meshes;

		static uint32_t s_nextId;
		static std::stack<uint32_t> s_availableIds;
		static std::unordered_map<uint32_t, SceneObject*> s_objectMap;
		static ObjectsCollection s_selected;
	};
}
