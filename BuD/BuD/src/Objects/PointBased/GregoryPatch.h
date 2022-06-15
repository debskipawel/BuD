#pragma once

#include <Objects/Abstract/PointBasedObject.h>

#include <array>

namespace BuD
{
	class GregoryPatch : public PointBasedObject
	{
	public:
		GregoryPatch(Scene& scene, const DX11Device& device, const std::vector<Point*>& cycleFirstLine, const std::vector<Point*>& cycleSecondLine);

		virtual void OnUpdate() override;
		virtual void Accept(AbstractVisitor& visitor) override;

		virtual void OnSelect() override;
		virtual void OnUnselect() override;

	protected:
		void TogglePolygonDisplay(bool enable);

		static std::array<float, 4> DeCasteljau(float t);
		static std::vector<unsigned short> Indices(float samplesU, float samplesV);

		std::vector<Vector3> Meshify(const std::vector<Vector3>& controlPoints);

		std::vector<Point*> m_secondLineControlPoints;
		std::vector<std::shared_ptr<Mesh>> m_polygonMeshes;

		int m_samplesU = 8, m_samplesV = 8;
		bool m_drawPolygon;
		Vector3 m_color = { 1.0f, 1.0f, 1.0f };

		friend class GuiDrawer;
		friend class ObjectSerializer;
	};
}
