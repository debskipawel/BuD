#include "Scene.h"

#include <algorithm>
#include <iterator>

#include <Objects/Independent/Point.h>
#include <Objects/Independent/Torus.h>

#include <Objects/PointBased/Curves/BezierCurveC0.h>
#include <Objects/PointBased/Curves/BezierCurveC2.h>
#include <Objects/PointBased/Curves/InterpolatedCurveC2.h>

#include <Objects/PointBased/Surfaces/BezierPatchC0.h>
#include <Objects/PointBased/Surfaces/BezierPatchC0GPU.h>
#include <Objects/PointBased/Surfaces/BezierSurfaceC0.h>

#include <Objects/PointBased/Surfaces/BezierPatchC2.h>
#include <Objects/PointBased/Surfaces/BezierSurfaceC2.h>

#include <Visitors/ObjectSerializer.h>
#include <Serializer.h>

namespace BuD
{
	std::shared_ptr<Point> Scene::CreatePoint(const DX11Device& device, const Vector3& position)
	{
		auto point = std::make_shared<Point>(*this, device, position);
		AddSceneObject(point);

		return point;
	}

	std::shared_ptr<SceneObject> Scene::CreateTorus(const DX11Device& device, const Vector3& position, float largeRadius, float smallRadius)
	{
		auto torus = std::make_shared<Torus>(*this, device, position, largeRadius, smallRadius);
		AddSceneObject(torus);

		return torus;
	}

	std::shared_ptr<SceneObject> Scene::CreateBezierCurveC0(const DX11Device& device, const std::vector<Point*>& controlPoints)
	{
		auto curve = std::make_shared<BezierCurveC0>(*this, device, controlPoints);
		AddSceneObject(curve);

		return curve;
	}

	std::shared_ptr<SceneObject> Scene::CreateBezierCurveC2(const DX11Device& device, const std::vector<Point*>& controlPoints)
	{
		auto curve = std::make_shared<BezierCurveC2>(*this, device, controlPoints);
		AddSceneObject(curve);

		return curve;
	}

	std::shared_ptr<SceneObject> Scene::CreateInterpolatedCurveC2(const DX11Device& device, const std::vector<Point*>& controlPoints)
	{
		auto curve = std::make_shared<InterpolatedCurveC2>(*this, device, controlPoints);
		AddSceneObject(curve);

		return curve;
	}

	std::shared_ptr<SceneObject> Scene::CreateBezierPatchC0(const DX11Device& device, const std::vector<Point*>& controlPoints, int samplesU, int samplesV, BezierSurfaceC0* owner)
	{
		auto patch = std::make_shared<BezierPatchC0>(*this, device, controlPoints, samplesU, samplesV, owner);
		AddSceneObject(patch);

		return patch;
	}

	std::shared_ptr<SceneObject> Scene::CreateBezierPatchC0GPU(const DX11Device& device, const std::vector<Point*>& controlPoints, int samplesU, int samplesV, BezierSurfaceC0* owner)
	{
		auto patch = std::make_shared<BezierPatchC0GPU>(*this, device, controlPoints, samplesU, samplesV, owner);
		AddSceneObject(patch);

		return patch;
	}

	std::shared_ptr<SceneObject> Scene::CreateBezierSurfaceC0(const DX11Device& device, const Vector3& position, float patchWidth, float patchLength, int patchesU, int patchesV, int sampleU, int sampleV, bool asCylinder)
	{
		auto surface = std::make_shared<BezierSurfaceC0>(*this, device, position, patchWidth, patchLength, patchesU, patchesV, sampleU, sampleV, asCylinder);
		AddSceneObject(surface);

		return surface;
	}

	std::shared_ptr<SceneObject> Scene::CreateBezierSurfaceC0(const std::vector<BezierPatch*>& patches)
	{
		auto surface = std::make_shared<BezierSurfaceC0>(*this, patches);
		AddSceneObject(surface);

		return surface;
	}

	std::shared_ptr<SceneObject> Scene::CreateBezierPatchC2(const DX11Device& device, const std::vector<Point*>& controlPoints, int samplesU, int samplesV, BezierSurfaceC2* owner)
	{
		auto patch = std::make_shared<BezierPatchC2>(*this, device, controlPoints, samplesU, samplesV, owner);
		AddSceneObject(patch);

		return patch;
	}

	std::shared_ptr<SceneObject> Scene::CreateBezierSurfaceC2(const DX11Device& device, const Vector3& position, float patchWidth, float patchLength, int patchesU, int patchesV, int sampleU, int sampleV, bool asCylinder)
	{
		auto surface = std::make_shared<BezierSurfaceC2>(*this, device, position, patchWidth, patchLength, patchesU, patchesV, sampleU, sampleV, asCylinder);
		AddSceneObject(surface);

		return surface;
	}

	std::shared_ptr<SceneObject> Scene::CreateBezierSurfaceC2(const std::vector<BezierPatch*>& patches)
	{
		auto surface = std::make_shared<BezierSurfaceC2>(*this, patches);
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
	
	void Scene::SaveToFile(std::filesystem::path path)
	{
		ObjectSerializer serializer;

		MG1::Scene::Get().Clear();

		for (auto& [id, obj] : m_objects)
		{
			serializer.Visit(*obj);
		}

		MG1::SceneSerializer sceneSerializer;

		sceneSerializer.SaveScene(path);
	}

	Scene Scene::ReadFromFile(const DX11Device& device, std::filesystem::path path)
	{
		MG1::SceneSerializer sceneSerializer;

		sceneSerializer.LoadScene(path);

		auto& deserializedScene = MG1::Scene::Get();
		std::map<uint32_t, Point*> points;

		Scene scene;

		for (auto& point : deserializedScene.points)
		{
			auto pos = point.position;
			auto p = scene.CreatePoint(device, Vector3{ pos.x, pos.y, pos.z });

			points.insert(std::make_pair(point.GetId(), p.get()));
		}

		for (auto& torus : deserializedScene.tori)
		{
			auto pos = torus.position;
			auto rot = torus.rotation;
			auto sc = torus.scale;
			auto t = scene.CreateTorus(device, Vector3{ pos.x, pos.y, pos.z }, torus.largeRadius, torus.smallRadius);

			t->RotateTo(Vector3{ rot.x, rot.y, rot.z });
			t->ScaleTo(Vector3{ sc.x, sc.y, sc.z });

			//TODO: samples & name
		}

		for (auto& bezierC0 : deserializedScene.bezierC0)
		{
			std::vector<Point*> controlPoints;

			std::transform(bezierC0.controlPoints.begin(), bezierC0.controlPoints.end(), std::back_inserter(controlPoints),
				[&points](MG1::PointRef ref) { return points[ref.GetId()]; }
			);

			auto b = scene.CreateBezierCurveC0(device, controlPoints);
		}

		for (auto& bezierC2 : deserializedScene.bezierC2)
		{
			std::vector<Point*> controlPoints;

			std::transform(bezierC2.controlPoints.begin(), bezierC2.controlPoints.end(), std::back_inserter(controlPoints),
				[&points](MG1::PointRef ref) { return points[ref.GetId()]; }
			);

			auto b = scene.CreateBezierCurveC2(device, controlPoints);
		}

		for (auto& interpolatedC2 : deserializedScene.interpolatedC2)
		{
			std::vector<Point*> controlPoints;

			std::transform(interpolatedC2.controlPoints.begin(), interpolatedC2.controlPoints.end(), std::back_inserter(controlPoints),
				[&points](MG1::PointRef ref) { return points[ref.GetId()]; }
			);

			auto b = scene.CreateInterpolatedCurveC2(device, controlPoints);
		}

		for (auto& surface : deserializedScene.surfacesC0)
		{
			std::vector<BezierPatch*> patches;

			for (auto& patch : surface.patches)
			{
				std::vector<Point*> controlPoints;

				std::transform(patch.controlPoints.begin(), patch.controlPoints.end(), std::back_inserter(controlPoints),
					[&points](MG1::PointRef ref) { return points[ref.GetId()]; }
				);

				auto p = scene.CreateBezierPatchC0(device, controlPoints, patch.samples.x, patch.samples.y, nullptr);
				patches.push_back(reinterpret_cast<BezierPatch*>(p.get()));
			}

			scene.CreateBezierSurfaceC0(patches);
		}

		for (auto& surface : deserializedScene.surfacesC2)
		{
			std::vector<BezierPatch*> patches;

			for (auto& patch : surface.patches)
			{
				std::vector<Point*> controlPoints;

				std::transform(patch.controlPoints.begin(), patch.controlPoints.end(), std::back_inserter(controlPoints),
					[&points](MG1::PointRef ref) { return points[ref.GetId()]; }
				);

				auto p = scene.CreateBezierPatchC2(device, controlPoints, patch.samples.x, patch.samples.y, nullptr);
				patches.push_back(reinterpret_cast<BezierPatch*>(p.get()));
			}

			scene.CreateBezierSurfaceC2(patches);
		}

		return scene;
	}
}
