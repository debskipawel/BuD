#pragma once

#include <Objects/ObjectType.h>
#include <Scene/Mesh.h>
#include <Visitors/AbstractVisitor.h>

#include <memory>
#include <stack>
#include <unordered_map>

#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

namespace BuD
{
	class SceneObject
	{
	public:
		SceneObject();

		virtual void Accept(AbstractVisitor& visitor) = 0;

		virtual void MoveTo(const Vector3& position, bool propagateUpdate = false);
		virtual void MoveBy(const Vector3& difference, bool propagateUpdate = false);
		virtual void ScaleTo(const Vector3& scale, bool propagateUpdate = false);
		virtual void RotateTo(const Quaternion& quaternion, bool propagateUpdate = false);
		virtual void RotateTo(const Vector3& rotation, bool propagateUpdate = false);

		virtual ObjectType GetType() = 0;
		
		virtual void OnUpdate() = 0;
		virtual void OnDelete() = 0;
		virtual void OnSelect() { m_selected = true; }
		virtual void OnUnselect() { m_selected = false; }

		virtual Matrix ModelMatrix() const;
		virtual Vector3 Position() const;

		inline int MeshesCount() const { return m_meshes.size(); }
		inline std::shared_ptr<Mesh> GetMesh(int index) { return m_meshes[index]; }

		inline uint32_t Id() const { return m_id; }
		inline std::string Name() { return m_tag; }
		inline bool Selected() { return m_selected; }

	protected:
		uint32_t m_id;
		std::string m_tag = "Unnamed object";
		std::vector<std::shared_ptr<Mesh>> m_meshes;
		
		bool m_selected;

		static uint32_t s_nextId;
		static std::stack<uint32_t> s_availableIds;
	};
}
