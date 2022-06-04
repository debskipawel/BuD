#include "PointBasedObject.h"

#include <Objects/Scene.h>
#include <Objects/Independent/Point.h>

namespace BuD
{
	PointBasedObject::PointBasedObject(Scene& scene, const std::vector<Point*>& controlPoints)
		: SceneObject(scene), m_controlPoints(controlPoints)
	{
		for (auto& point : m_controlPoints)
		{
			point->AddDependentObject(this);
		}
	}

	void PointBasedObject::AddControlPoint(Point* obj)
	{
		m_controlPoints.push_back(obj);
		obj->AddDependentObject(this);

		OnUpdate();
	}

	void PointBasedObject::AddControlPoints(std::initializer_list<Point*> obj)
	{
		for (auto& point : obj)
		{
			m_controlPoints.push_back(point);
			point->AddDependentObject(this);
		}

		OnUpdate();
	}

	void PointBasedObject::RemoveControlPoint(int index)
	{
		if (index < 0 || index >= m_controlPoints.size())
		{
			return;
		}

		m_controlPoints.erase(m_controlPoints.begin() + index);

		OnUpdate();
	}

	void PointBasedObject::ReplaceControlPoint(Point* oldPoint, Point* newPoint)
	{
		while (true)
		{
			auto res = std::find(m_controlPoints.begin(), m_controlPoints.end(), oldPoint);

			if (res == m_controlPoints.end())
			{
				break;
			}

			*res = newPoint;
		}

		OnUpdate();
	}

	void PointBasedObject::OnDelete()
	{
		for (auto& point : m_controlPoints)
		{
			point->RemoveDependentObject(this);
		}
	}
}