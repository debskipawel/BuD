#pragma once

#include <MathObjects/Parameterized2DObjectCPU.h>
#include <Objects/Abstract/PointBasedObject.h>

#include <array>

namespace BuD
{
	class BezierSurfaceC0;

	class BezierPatchC0 : public PointBasedObject, public Parameterized2DObjectCPU
	{
	public:
		BezierPatchC0(Scene& scene, const DX11Device& device, const std::vector<Point*>& controlPoints, int samplesU, int samplesV, BezierSurfaceC0* owner = nullptr);

		virtual void OnUpdate() override;
		virtual void OnDelete() override;
		virtual void Accept(AbstractVisitor& visitor) override;

		inline virtual bool ParameterizationWrapped() override { return false; }

		virtual ObjectFlags GetFlags() override { return ObjectFlags::BEZIER_PATCH_C0; }
		virtual Vector2 MinDomain() override { return { 0.0f, 0.0f }; }
		virtual Vector2 MaxDomain() override { return { 1.0f, 1.0f }; }

		virtual void OnSelect() override;
		virtual void OnUnselect() override;

	protected:
		virtual Vector3 GetPoint(Vector2 param) override;

		virtual void ToggleBezierPolygon(bool show);

		static std::array<float, 4> DeCasteljau(float t);
		
		float m_prevU = -1.0f;
		std::array<float, 4> m_uDeCasteljau;

		Vector3 m_color = { 1.0f, 1.0f, 1.0f };

		std::shared_ptr<Mesh> m_bezierPolygonMesh;
		bool m_showBezierPolygon = false;

		BezierSurfaceC0* m_owner;

		friend class BezierSurfaceC0;
		friend class GuiDrawer;
	};
}
