#pragma once

#include <Objects/Abstract/SceneObject.h>
#include <Objects/Abstract/PointBasedObject.h>

#include <set>

namespace BuD
{
	class Point : public SceneObject
	{
	public:
		Point(const DX11Device& device, const Vector3& position);

		virtual void Accept(AbstractVisitor& visitor) override;

		virtual void OnUpdate() override;
		virtual void OnDelete(Scene& scene) override;
		virtual void OnSelect() override;
		virtual void OnUnselect() override;

		virtual void AddDependentObject(PointBasedObject* obj);
		virtual void RemoveDependentObject(PointBasedObject* obj);

		virtual ObjectFlags GetFlags() override { return ObjectFlags::POINT; }

		inline virtual void RotateTo(const Vector3& rotation, bool propagateUpdate = false) override { }
		inline virtual void RotateTo(const Quaternion& rotation, bool propagateUpdate = false) override { }
		inline virtual void ScaleTo(const Vector3& scale, bool propagateUpdate = false) override { }

		static std::shared_ptr<Mesh> GetMesh(const DX11Device& device);

	protected:
		std::set<PointBasedObject*> m_dependentObjects;

	private:
		Vector3 m_color = { 1.0f, 1.0f, 1.0f };

		static std::shared_ptr<DX11VertexBuffer> GetVB(const DX11Device& device);
		static std::shared_ptr<DX11IndexBuffer> GetIB(const DX11Device& device);

		static std::shared_ptr<DX11VertexBuffer> s_vertexBuffer;
		static std::shared_ptr<DX11IndexBuffer> s_indexBuffer;
	};
}
