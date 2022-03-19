#pragma once

#include "Scene/Mesh.h"

#include <memory>
#include <stack>
#include <unordered_map>

namespace BuD
{
	class SceneObject
	{
	public:
		SceneObject();

		static SceneObject* Get(uint32_t id);
		static const std::unordered_map<uint32_t, SceneObject*>& GetAll();
		inline std::shared_ptr<Mesh> GetModel() { return m_model; }

		virtual void DrawGui() = 0;

	protected:
		void InsertObject();

		uint32_t m_id;
		std::string m_tag = "Unnamed object";
		std::shared_ptr<Mesh> m_model;

		static uint32_t s_nextId;
		static std::stack<uint32_t> s_availableIds;
		static std::unordered_map<uint32_t, SceneObject*> s_objectMap;
	};
}
