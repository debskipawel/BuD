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
		inline static const ObjectsCollection& GetSelected() { return s_selected; }
		static void DeleteObject(uint32_t id);

		inline std::shared_ptr<Mesh> GetModel() { return m_model; }

		inline std::string* Name() { return &m_tag; }

		virtual void DrawGui() = 0;

		virtual void Select();
		virtual void Unselect();

	protected:
		void InsertObject();
		void RemoveObject();

		bool m_selected = false;

		uint32_t m_id;
		std::string m_tag = "Unnamed object";
		std::shared_ptr<Mesh> m_model;

		static uint32_t s_nextId;
		static std::stack<uint32_t> s_availableIds;
		static std::unordered_map<uint32_t, SceneObject*> s_objectMap;
		static ObjectsCollection s_selected;
	};
}
