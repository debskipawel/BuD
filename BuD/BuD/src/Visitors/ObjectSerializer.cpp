#include "ObjectSerializer.h"

#include <Objects/Independent/Point.h>
#include <Objects/Independent/Torus.h>

#include <Objects/PointBased/Curves/BezierCurve.h>
#include <Objects/PointBased/Curves/BezierCurveC0.h>
#include <Objects/PointBased/Curves/BezierCurveC2.h>
#include <Objects/PointBased/Curves/InterpolatedCurveC2.h>

#include <Objects/PointBased/Surfaces/BezierPatchC0.h>
#include <Objects/PointBased/Surfaces/BezierPatchC2.h>
#include <Objects/PointBased/Surfaces/BezierSurfaceC0.h>
#include <Objects/PointBased/Surfaces/BezierSurfaceC2.h>

#include <Serializer.h>

namespace BuD
{
	void ObjectSerializer::Action(Point& point)
	{
		auto& scene = MG1::Scene::Get();
		auto p = MG1::Point();

		p.name = point.GetName();
		p.SetId(point.Id());

		auto position = point.Position();

		p.position = MG1::Float3{ position.x, position.y, position.z };

		scene.points.push_back(p);
	}

	void ObjectSerializer::Action(Torus& torus)
	{
		auto& scene = MG1::Scene::Get();
		auto t = MG1::Torus();

		auto position = torus.GetMesh(0)->m_position;
		auto rotation = torus.GetMesh(0)->m_rotation;
		auto scale = torus.GetMesh(0)->m_scale;

		t.position = MG1::Float3{ position.x, position.y, position.z };
		t.rotation = MG1::Float3{ rotation.x, rotation.y, rotation.z };
		t.scale = MG1::Float3{ scale.x, scale.y, scale.z };
		
		t.name = torus.GetName();
		t.largeRadius = torus.m_largeRadius;
		t.smallRadius = torus.m_smallRadius;
		t.samples = MG1::Uint2{ static_cast<uint32_t>(torus.SamplesU()), static_cast<uint32_t>(torus.SamplesV()) };

		t.SetId(torus.Id());

		scene.tori.push_back(t);
	}

	void ObjectSerializer::Action(BezierCurveC0& curve)
	{
		auto& scene = MG1::Scene::Get();
		auto c = MG1::BezierC0();

		c.name = curve.GetName();
		c.SetId(curve.Id());

		for (auto cp : curve.m_controlPoints)
		{
			c.controlPoints.push_back(MG1::PointRef(cp->Id()));
		}

		scene.bezierC0.push_back(c);
	}

	void ObjectSerializer::Action(BezierCurveC2& curve)
	{
		auto& scene = MG1::Scene::Get();
		auto c = MG1::BezierC2();

		c.name = curve.GetName();
		c.SetId(curve.Id());

		for (auto cp : curve.m_controlPoints)
		{
			c.controlPoints.push_back(MG1::PointRef(cp->Id()));
		}

		scene.bezierC2.push_back(c);
	}

	void ObjectSerializer::Action(InterpolatedCurveC2& curve)
	{
		auto& scene = MG1::Scene::Get();
		auto c = MG1::InterpolatedC2();

		c.name = curve.GetName();
		c.SetId(curve.Id());

		for (auto cp : curve.m_controlPoints)
		{
			c.controlPoints.push_back(MG1::PointRef(cp->Id()));
		}

		scene.interpolatedC2.push_back(c);
	}

	void ObjectSerializer::Action(BezierPatchC0& patch)
	{
	}

	void ObjectSerializer::Action(BezierSurfaceC0& surface)
	{
		auto& scene = MG1::Scene::Get();
		auto s = MG1::BezierSurfaceC0();

		s.name = surface.GetName();
		s.SetId(surface.Id());

		s.uWrapped = surface.m_cylinder;
		s.vWrapped = false;
		s.size = MG1::Uint2{ static_cast<uint32_t>(surface.m_patchesU), static_cast<uint32_t>(surface.m_patchesV) };

		for (auto& patch : surface.m_patches)
		{
			auto p = MG1::BezierPatchC0();
			p.name = patch->GetName();
			p.SetId(patch->Id());

			p.samples = MG1::Uint2{ static_cast<uint32_t>(patch->SamplesU()), static_cast<uint32_t>(patch->SamplesV()) };

			for (auto& cp : patch->m_controlPoints)
			{
				p.controlPoints.push_back(MG1::PointRef(cp->Id()));
			}

			s.patches.push_back(p);
		}

		scene.surfacesC0.push_back(s);
	}

	void ObjectSerializer::Action(BezierPatchC2& patch)
	{
	}

	void ObjectSerializer::Action(BezierSurfaceC2& surface)
	{
		auto& scene = MG1::Scene::Get();
		auto s = MG1::BezierSurfaceC2();

		s.name = surface.GetName();
		s.SetId(surface.Id());

		s.uWrapped = surface.m_cylinder;
		s.vWrapped = false;
		s.size = MG1::Uint2{ static_cast<uint32_t>(surface.m_patchesU), static_cast<uint32_t>(surface.m_patchesV) };

		for (auto& patch : surface.m_patches)
		{
			auto p = MG1::BezierPatchC2();
			p.name = patch->GetName();
			p.SetId(patch->Id());

			p.samples = MG1::Uint2{ static_cast<uint32_t>(patch->SamplesU()), static_cast<uint32_t>(patch->SamplesV()) };

			for (auto& cp : patch->m_controlPoints)
			{
				p.controlPoints.push_back(MG1::PointRef(cp->Id()));
			}

			s.patches.push_back(p);
		}

		scene.surfacesC2.push_back(s);
	}
}
