#pragma once

#include "Abstract/SceneObject.h"
#include "Independent/Point.h"

#include <map>

namespace BuD
{
	class Scene
	{
	public:
		Scene() = default;

		virtual std::shared_ptr<Point> CreatePoint(const DX11Device& device, const Vector3& position);
		virtual std::shared_ptr<Point> CreateUnmovablePoint(const DX11Device& device, const Vector3& position);

		virtual std::shared_ptr<SceneObject> CreateTorus(const DX11Device& device, const Vector3& position, float largeRadius, float smallRadius);
		virtual std::shared_ptr<SceneObject> CreateBezierCurveC0(const DX11Device& device, const std::vector<Point*>& controlPoints);
		virtual std::shared_ptr<SceneObject> CreateBezierCurveC2(const DX11Device& device, const std::vector<Point*>& controlPoints);
		virtual std::shared_ptr<SceneObject> CreateInterpolatedCurveC2(const DX11Device& device, const std::vector<Point*>& controlPoints);

		virtual std::shared_ptr<SceneObject> GetSceneObject(uint32_t id);

		virtual std::map<uint32_t, std::shared_ptr<SceneObject>> GetAllSceneObjects();
		virtual std::map<uint32_t, std::shared_ptr<SceneObject>> GetSceneObjectsFiltered(ObjectType filter);
		
		virtual std::map<uint32_t, std::shared_ptr<SceneObject>> GetAllSelected();

		virtual void AddSceneObject(std::shared_ptr<SceneObject> object);
		virtual void RemoveSceneObject(std::shared_ptr<SceneObject> object);
		virtual void RemoveSceneObject(uint32_t id);

	protected:
		std::map<uint32_t, std::shared_ptr<SceneObject>> m_objects;
	};
}
