#pragma once

#include "AbstractVisitor.h"

namespace BuD
{
	class ObjectSerializer : public AbstractVisitor
	{
	public:
		virtual void Action(Point& point) override;
		virtual void Action(Torus& torus) override;
		virtual void Action(BezierCurveC0& curve) override;
		virtual void Action(BezierCurveC2& curve) override;
		virtual void Action(InterpolatedCurveC2& curve) override;
		virtual void Action(BezierPatchC0& patch) override;
		virtual void Action(BezierSurfaceC0& surface) override;
		virtual void Action(BezierPatchC2& patch) override;
		virtual void Action(BezierSurfaceC2& surface) override;
		virtual void Action(GregoryPatch& surface) override;
	};
}
