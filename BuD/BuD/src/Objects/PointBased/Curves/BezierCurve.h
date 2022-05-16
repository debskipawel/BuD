#pragma once

#include <Objects/Abstract/PointBasedObject.h>

namespace BuD
{
	class BezierCurve : public PointBasedObject
	{
	public:
		virtual void OnUpdate() override {}

		virtual void OnSelect() override;
		virtual void OnUnselect() override;

		virtual void MoveTo(const Vector3& position, bool propagateUpdate = false) override {}
		virtual void MoveBy(const Vector3& difference, bool propagateUpdate = false) override {}
		virtual void ScaleTo(const Vector3& scale, bool propagateUpdate = false) override {}
		virtual void RotateTo(const Quaternion& quaternion, bool propagateUpdate = false) override {}
		virtual void RotateTo(const Vector3& rotation, bool propagateUpdate = false) override {}

		inline virtual void DrawPolygon(bool draw) { m_drawPolygon = draw; }

	protected:
		BezierCurve(const std::vector<Point*>& controlPoints);

		virtual RECT GetSurroundingRectangle(const Matrix& view, const Matrix& projection, UINT width, UINT height);

		Vector3 m_color = { 1.0f, 1.0f, 1.0f };
		bool m_drawPolygon = false;

		friend class GuiDrawer;
	};
}
