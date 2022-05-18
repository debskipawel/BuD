#pragma once

#include "SceneObject.h"

namespace BuD
{
	class Point;

	class PointBasedObject : public SceneObject
	{
	public:
		virtual std::vector<Vector3> GetVirtualPoints() { return std::vector<Vector3>(); }

		virtual ObjectFlags GetFlags() override { return ObjectFlags::POINT_BASED; }

		virtual void MoveTo(const Vector3& position, bool propagateUpdate = false) override {}
		virtual void MoveBy(const Vector3& difference, bool propagateUpdate = false) override {}
		virtual void ScaleTo(const Vector3& scale, bool propagateUpdate = false) override {}
		virtual void RotateTo(const Quaternion& quaternion, bool propagateUpdate = false) override {}
		virtual void RotateTo(const Vector3& rotation, bool propagateUpdate = false) override {}

		virtual void AddControlPoint(Point* obj);
		virtual void AddControlPoints(std::initializer_list<Point*> obj);
		virtual void RemoveControlPoint(int index);

		virtual void OnDelete() override;

	protected:
		PointBasedObject(Scene& scene, const std::vector<Point*>& controlPoints);

		friend class Point;

		std::vector<Point*> m_controlPoints;
	};
}
