#pragma once

#include <string>
#include <set>
#include <stack>

namespace MG1
{
	class SceneObject
	{
	public:
		inline uint32_t GetId() const { return m_id; }
		virtual bool SetId(uint32_t id);

		std::string name;

	protected:
		uint32_t m_id;

		explicit SceneObject();
		SceneObject(const SceneObject& obj) = default;

		static uint32_t s_firstFreeId;
		static std::set<uint32_t> s_takenIds;
		static std::stack<uint32_t> s_freedIds;
	};
}
