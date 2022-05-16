#pragma once

namespace BuD
{
	class SceneObject;

	class Point;
	class Torus;
	class BezierCurveC0;
	class BezierCurveC2;
	class InterpolatedCurveC2;

	class AbstractVisitor
	{
	public:
		void Visit(SceneObject& obj);

		virtual void Action(Point& point) = 0;
		virtual void Action(Torus& torus) = 0;
		virtual void Action(BezierCurveC0& curve) = 0;
		virtual void Action(BezierCurveC2& curve) = 0;
		virtual void Action(InterpolatedCurveC2& curve) = 0;
	};
}
