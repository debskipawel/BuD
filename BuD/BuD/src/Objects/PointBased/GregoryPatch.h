#pragma once

#include <Objects/Abstract/PointBasedObject.h>

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
		std::vector<Vector3> Meshify(const std::vector<Vector3>& controlPoints);

		std::vector<Point*> m_secondLineControlPoints;

		const int m_samplesU = 20, m_samplesV = 20;
		Vector3 m_color = { 1.0f, 1.0f, 1.0f };

		friend class GuiDrawer;
		friend class ObjectSerializer;
	};
}
