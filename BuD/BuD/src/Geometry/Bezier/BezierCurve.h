#pragma once

#include "../SceneObject.h"

namespace BuD
{
	class BezierCurve : public SceneObject
	{
	public:
		virtual bool DrawGui() override;

		virtual void MoveTo(const Vector3& position) override {}
		virtual void MoveBy(const Vector3& difference) override {}
		virtual void ScaleTo(const Vector3& scale) override {}
		virtual void RotateTo(const Quaternion& quaternion) override {}
		virtual void RotateTo(const Vector3& rotation) override {}
		
		virtual void AddControlPoint(SceneObject* obj);
		virtual void RemoveControlPoint(int index);

		virtual void Select() override
		{
			SceneObject::Select();

			m_color = { 0.7f, 0.5f, 0.0f };
		}

		virtual void Unselect()
		{
			SceneObject::Unselect();

			m_color = { 1.0f, 1.0f, 1.0f };
		}

		inline virtual void DrawPolygon(bool draw) { m_drawPolygon = draw; }
		inline virtual bool ShouldDrawPolygon() { return m_drawPolygon; }

	protected:
		BezierCurve(std::vector<SceneObject*> controlPoints);

		virtual void FilterControlPoints();
		virtual void UpdateCentroid();
		virtual RECT GetSurroundingRectangle(std::shared_ptr<AbstractCamera> camera, UINT width, UINT height);

		virtual bool DrawGuiForAddingControlPoints();
		virtual bool DrawGuiForEditingControlPoints();

		Vector3 m_color;

		std::vector<SceneObject*> m_controlPoints;
		bool m_drawPolygon = false;
	};
}
