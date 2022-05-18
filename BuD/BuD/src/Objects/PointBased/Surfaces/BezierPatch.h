#pragma once

#include <MathObjects/Parameterized2DObjectCPU.h>
#include <Objects/Abstract/PointBasedObject.h>

namespace BuD
{
	class BezierSurface;

	class BezierPatch : public PointBasedObject, public Parameterized2DObjectCPU
	{
	public:
		BezierPatch(Scene& scene, const DX11Device& device, const std::vector<Point*>& controlPoints, int samplesU, int samplesV, BezierSurface* owner = nullptr);

		virtual void OnDelete() override;

		inline virtual bool ParameterizationWrapped() override { return false; }

		virtual ObjectFlags GetFlags() override { return ObjectFlags::PATCH; }
		virtual Vector2 MinDomain() override { return { 0.0f, 0.0f }; }
		virtual Vector2 MaxDomain() override { return { 1.0f, 1.0f }; }

		virtual void OnSelect() override;
		virtual void OnUnselect() override;

	protected:
		virtual void ToggleBezierPolygon(bool show);

		Vector3 m_color = { 1.0f, 1.0f, 1.0f };

		std::shared_ptr<Mesh> m_bezierPolygonMesh;
		bool m_showBezierPolygon = false;

		BezierSurface* m_owner;

		friend class BezierSurface;
		friend class GuiDrawer;
	};
}
