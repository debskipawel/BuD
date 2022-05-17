#include "BezierCurve.h"

#include <algorithm>

#include <Objects/Scene.h>
#include <Objects/Independent/Point.h>

namespace BuD
{
	BezierCurve::BezierCurve(Scene& scene, const std::vector<Point*>& controlPoints)
		: PointBasedObject(scene, controlPoints)
	{
	}

	void BezierCurve::OnSelect()
	{
		SceneObject::OnSelect();
		m_color = { 0.7f, 0.5f, 0.0f };
	}

	void BezierCurve::OnUnselect()
	{
		SceneObject::OnUnselect();
		m_color = { 1.0f, 1.0f, 1.0f };
	}

	RECT BezierCurve::GetSurroundingRectangle(const Matrix& view, const Matrix& projection, UINT width, UINT height)
	{
		long minX = width, maxX = 0;
		long minY = height, maxY = 0;

		auto matrix = view * projection;

		for (auto& cp : m_controlPoints)
		{
			auto position = cp->Position();
			auto trPosition = Vector4{ position.x, position.y, position.z, 1.0f };
			trPosition = Vector4::Transform(trPosition, matrix);
			trPosition /= trPosition.w;

			long xPos = width * (std::clamp(trPosition.x, -1.0f, 1.0f) + 1.0f) / 2.0f;
			long yPos = height * (std::clamp(trPosition.y, -1.0f, 1.0f) + 1.0f) / 2.0f;

			maxX = max(xPos, maxX);
			minX = min(xPos, minX);
			maxY = max(yPos, maxY);
			minY = min(yPos, minY);
		}

		return { minX, minY, maxX, maxY };
	}
}