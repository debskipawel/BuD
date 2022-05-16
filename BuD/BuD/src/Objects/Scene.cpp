#include "Scene.h"

#include <algorithm>
#include <iterator>

#include <Objects/Independent/Point.h>
#include <Objects/Independent/UnmovablePoint.h>
#include <Objects/Independent/Torus.h>

#include <Objects/PointBased/Curves/BezierCurveC0.h>
#include <Objects/PointBased/Curves/BezierCurveC2.h>
#include <Objects/PointBased/Curves/InterpolatedCurveC2.h>

#include <Objects/PointBased/Surfaces/BezierPatchC0.h>
#include <Objects/PointBased/Surfaces/BezierSurfaceC0.h>

namespace BuD
{
	std::shared_ptr<Point> Scene::CreatePoint(const DX11Device& device, const Vector3& position)
	{
		auto point = std::make_shared<Point>(device, position);
		AddSceneObject(point);

		return point;
	}

	std::shared_ptr<Point> Scene::CreateUnmovablePoint(const DX11Device& device, const Vector3& position)
	{
		auto point = std::make_shared<UnmovablePoint>(device, position);
		AddSceneObject(point);

		return point;
	}

	std::shared_ptr<SceneObject> Scene::CreateTorus(const DX11Device& device, const Vector3& position, float largeRadius, float smallRadius)
	{
		auto torus = std::make_shared<Torus>(device, position, largeRadius, smallRadius);
		AddSceneObject(torus);

		return torus;
	}

	std::shared_ptr<SceneObject> Scene::CreateBezierCurveC0(const DX11Device& device, const std::vector<Point*>& controlPoints)
	{
		auto curve = std::make_shared<BezierCurveC0>(device, controlPoints);
		AddSceneObject(curve);

		return curve;
	}

	std::shared_ptr<SceneObject> Scene::CreateBezierCurveC2(const DX11Device& device, const std::vector<Point*>& controlPoints)
	{
		auto curve = std::make_shared<BezierCurveC2>(device, controlPoints);
		AddSceneObject(curve);

		return curve;
	}

	std::shared_ptr<SceneObject> Scene::CreateInterpolatedCurveC2(const DX11Device& device, const std::vector<Point*>& controlPoints)
	{
		auto curve = std::make_shared<InterpolatedCurveC2>(device, controlPoints);
		AddSceneObject(curve);

		return curve;
	}

	std::shared_ptr<SceneObject> Scene::CreateBezierPatchC0(const DX11Device& device, const std::vector<Point*>& controlPoints, int samplesU, int samplesV)
	{
		auto patch = std::make_shared<BezierPatchC0>(device, controlPoints, samplesU, samplesV);
		AddSceneObject(patch);

		return patch;
	}

	std::shared_ptr<SceneObject> Scene::CreateBezierSurfaceC0(const DX11Device& device, const Vector3& position, float patchWidth, float patchLength, int patchesU, int patchesV, int sampleU, int sampleV, bool asCylinder)
	{
		auto surface = std::make_shared<BezierSurfaceC0>(device, *this, position, patchWidth, patchLength, patchesU, patchesV, sampleU, sampleV, asCylinder);
		AddSceneObject(surface);

		return surface;
	}

	std::shared_ptr<SceneObject> Scene::GetSceneObject(uint32_t id)
	{
		auto result = m_objects.find(id);

		if (result == m_objects.end())
		{
			return nullptr;
		}

		return result->second;
	}

	std::map<uint32_t, std::shared_ptr<SceneObject>> Scene::GetAllSceneObjects()
	{
		return m_objects;
	}

	std::map<uint32_t, std::shared_ptr<SceneObject>> Scene::GetSceneObjectsFiltered(ObjectFlags filter)
	{
		std::map<uint32_t, std::shared_ptr<SceneObject>> result;

		std::copy_if(m_objects.begin(), m_objects.end(), std::inserter(result, result.end()),
			[filter](std::pair<uint32_t, std::shared_ptr<SceneObject>> record) { return (record.second->GetFlags() & filter) == filter; }
		);

		return result;
	}

	std::map<uint32_t, std::shared_ptr<SceneObject>> Scene::GetAllSelected()
	{
		std::map<uint32_t, std::shared_ptr<SceneObject>> result;

		std::copy_if(m_objects.begin(), m_objects.end(), std::inserter(result, result.end()),
			[](std::pair<uint32_t, std::shared_ptr<SceneObject>> record) { return record.second->Selected(); }
		);

		return result;
	}

	void Scene::AddSceneObject(std::shared_ptr<SceneObject> object)
	{
		auto id = object->Id();

		if (m_objects.find(id) != m_objects.end())
		{
			return;
		}

		m_objects.insert(std::make_pair(id, object));
	}
	
	void Scene::RemoveSceneObject(std::shared_ptr<SceneObject> object)
	{
		RemoveSceneObject(object->Id());
	}
	
	void Scene::RemoveSceneObject(uint32_t id)
	{
		auto result = m_objects.find(id);

		if (result == m_objects.end())
		{
			return;
		}

		result->second->OnDelete();
		m_objects.erase(id);
	}
}
