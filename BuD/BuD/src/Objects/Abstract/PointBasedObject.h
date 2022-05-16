#pragma once

#include "SceneObject.h"

namespace BuD
{
	class Point;

	class PointBasedObject : public SceneObject
	{
	public:
		virtual std::vector<Vector3> GetVirtualPoints() { return std::vector<Vector3>(); }

		virtual ObjectType GetType() override { return ObjectType::POINT_BASED; }

		virtual void AddControlPoint(Point* obj);
		virtual void AddControlPoints(std::initializer_list<Point*> obj);
		virtual void RemoveControlPoint(int index);

		virtual void OnDelete() override;

	protected:
		PointBasedObject(const std::vector<Point*>& controlPoints);

		friend class Point;

		std::vector<Point*> m_controlPoints;
	};
}
