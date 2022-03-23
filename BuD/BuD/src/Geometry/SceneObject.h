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
		~SceneObject();

		static SceneObject* Get(uint32_t id);
		static const std::unordered_map<uint32_t, SceneObject*>& GetAll();
		static void DeleteObject(uint32_t id);
		inline static const ObjectsCollection& GetSelected() { return s_selected; }

		inline std::shared_ptr<Mesh> GetMesh() { return m_mesh; }
		inline uint32_t Id() const { return m_id; }
		inline std::string* Name() { return &m_tag; }
		inline virtual bool IsSelected() { return m_selected; }

		virtual void DrawGui() = 0;

		inline virtual void MoveTo(const Vector3& position)		{ m_mesh->m_position = position; }
		inline virtual void MoveBy(const Vector3& difference)	{ m_mesh->m_position += difference; }
		inline virtual void RotateTo(const Vector3& rotation)	{ m_mesh->m_rotation = rotation; }
		inline virtual void RotateBy(const Vector3& difference) { m_mesh->m_rotation += difference; }
		inline virtual void ScaleTo(const Vector3& scale)		{ m_mesh->m_scale = scale; }
		inline virtual void ScaleBy(const Vector3& difference)	{ m_mesh->m_scale += difference; }

		virtual void Select();
		virtual void Unselect();

	protected:
		void InsertObject();

		bool m_selected = false;

		uint32_t m_id;
		std::string m_tag = "Unnamed object";
		std::shared_ptr<Mesh> m_mesh;

		static uint32_t s_nextId;
		static std::stack<uint32_t> s_availableIds;
		static std::unordered_map<uint32_t, SceneObject*> s_objectMap;
		static ObjectsCollection s_selected;
	};
}
