#pragma once

namespace BuD
{
	class SceneObject;

	class Point;
	class Torus;
	class BezierCurveC0;
	class BezierCurveC2;
	class InterpolatedCurveC2;
	class BezierPatchC0;
	class BezierSurfaceC0;
	class BezierPatchC2;
	class BezierPatchC2GPU;
	class BezierSurfaceC2;
	class GregoryPatch;

	class AbstractVisitor
	{
	public:
		void Visit(SceneObject& obj);

		virtual void Action(Point& point) = 0;
		virtual void Action(Torus& torus) = 0;
		virtual void Action(BezierCurveC0& curve) = 0;
		virtual void Action(BezierCurveC2& curve) = 0;
		virtual void Action(InterpolatedCurveC2& curve) = 0;
		virtual void Action(BezierPatchC0& patch) = 0;
		virtual void Action(BezierSurfaceC0& surface) = 0;
		virtual void Action(BezierPatchC2& patch) = 0;
		//virtual void Action(BezierPatchC2GPU& patch) = 0;
		virtual void Action(BezierSurfaceC2& surface) = 0;
		virtual void Action(GregoryPatch& surface) = 0;
	};
}
