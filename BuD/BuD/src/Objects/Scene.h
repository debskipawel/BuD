#pragma once

#include "Abstract/SceneObject.h"
#include "Independent/Point.h"

#include <map>
#include <filesystem>

namespace BuD
{
	class BezierSurfaceC0;
	class BezierSurfaceC2;
	class BezierPatch;

	class Scene
	{
	public:
		Scene() = default;

		virtual std::shared_ptr<Point> CreatePoint(const DX11Device& device, const Vector3& position);

		virtual std::shared_ptr<SceneObject> CreateTorus(const DX11Device& device, const Vector3& position, float largeRadius, float smallRadius);
		virtual std::shared_ptr<SceneObject> CreateBezierCurveC0(const DX11Device& device, const std::vector<Point*>& controlPoints);
		virtual std::shared_ptr<SceneObject> CreateBezierCurveC2(const DX11Device& device, const std::vector<Point*>& controlPoints);
		virtual std::shared_ptr<SceneObject> CreateInterpolatedCurveC2(const DX11Device& device, const std::vector<Point*>& controlPoints);

		virtual std::shared_ptr<SceneObject> CreateBezierPatchC0(const DX11Device& device, const std::vector<Point*>& controlPoints, int samplesU, int samplesV, BezierSurfaceC0* owner = nullptr);
		virtual std::shared_ptr<SceneObject> CreateBezierPatchC0GPU(const DX11Device& device, const std::vector<Point*>& controlPoints, int samplesU, int samplesV, BezierSurfaceC0* owner = nullptr);
		virtual std::shared_ptr<SceneObject> CreateBezierSurfaceC0(const DX11Device& device, const Vector3& position, float patchWidth, float patchLength, int patchesU, int patchesV, int sampleU, int sampleV, bool asCylinder);
		virtual std::shared_ptr<SceneObject> CreateBezierSurfaceC0(const std::vector<BezierPatch*>& patches, int patchesU, int patchesV, bool asCylinder);

		virtual std::shared_ptr<SceneObject> CreateBezierPatchC2(const DX11Device& device, const std::vector<Point*>& controlPoints, int samplesU, int samplesV, BezierSurfaceC2* owner = nullptr);
		virtual std::shared_ptr<SceneObject> CreateBezierSurfaceC2(const DX11Device& device, const Vector3& position, float patchWidth, float patchLength, int patchesU, int patchesV, int sampleU, int sampleV, bool asCylinder);
		virtual std::shared_ptr<SceneObject> CreateBezierSurfaceC2(const std::vector<BezierPatch*>& patches, int patchesU, int patchesV, bool asCylinder);

		virtual std::shared_ptr<SceneObject> GetSceneObject(uint32_t id);

		virtual std::map<uint32_t, std::shared_ptr<SceneObject>> GetAllSceneObjects();
		virtual std::map<uint32_t, std::shared_ptr<SceneObject>> GetSceneObjectsFiltered(ObjectFlags filter);
		
		virtual std::map<uint32_t, std::shared_ptr<SceneObject>> GetAllSelected();

		virtual void AddSceneObject(std::shared_ptr<SceneObject> object);
		virtual void RemoveSceneObject(std::shared_ptr<SceneObject> object);
		virtual void RemoveSceneObject(uint32_t id);

		virtual void SaveToFile(std::filesystem::path path);

		static Scene ReadFromFile(const DX11Device& device, std::filesystem::path path);

	protected:
		std::map<uint32_t, std::shared_ptr<SceneObject>> m_objects;
	};
}
