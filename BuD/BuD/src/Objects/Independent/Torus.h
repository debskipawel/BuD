#pragma once

#include <Objects/Abstract/SceneObject.h>
#include <MathObjects/Parameterized2DObjectCPU.h>

#include <DirectX11/DX11Device.h>

namespace BuD
{
	class Torus : public SceneObject, public Parameterized2DObjectCPU
	{
	public:
		Torus(const DX11Device& device, const Vector3& position, float largeRadius, float smallRadius);

		virtual void Accept(AbstractVisitor& visitor) override;
		
		virtual void OnUpdate() override;
		virtual void OnDelete() override {}
		virtual void OnSelect() override;
		virtual void OnUnselect() override;

		virtual ObjectType GetType() override { return ObjectType::TORUS; }

		virtual Vector3 GetPoint(Vector2 parameter) override;
		
		virtual Vector2 MinDomain() override { return { 0.0f, 0.0f }; }
		virtual Vector2 MaxDomain() override { return { DirectX::XM_2PI, DirectX::XM_2PI }; }
		
		virtual bool ParameterizationWrapped() override { return true; }

	protected:
		Vector3 m_color = { 1.0f, 1.0f, 1.0f };

		float m_smallRadius, m_largeRadius;

		friend class GuiDrawer;
	};
}
