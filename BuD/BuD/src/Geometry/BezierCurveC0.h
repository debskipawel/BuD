#pragma once

#include "SceneObject.h"

namespace BuD
{
	class BezierCurveC0 : public SceneObject
	{
	public:
		BezierCurveC0(const DX11Device& device, std::vector<SceneObject*> controlPoints);

		virtual void DrawGui() override;

		virtual void MoveTo(const Vector3& position) override {}
		virtual void MoveBy(const Vector3& difference) override {}
		virtual void ScaleTo(const Vector3& scale) override {}
		virtual void RotateTo(const Quaternion& quaternion) override {}
		virtual void RotateTo(const Vector3& rotation) override {}

		virtual GeometryType GetType() override { return GeometryType::BEZIER_C0; }

		virtual void AddControlPoint(SceneObject* obj);
		virtual void RemoveControlPoint(SceneObject* obj);

		virtual void Select() override
		{
			SceneObject::Select();

			m_color = { 0.7f, 0.5f, 0.0f };
		}

		virtual void Unselect()
		{
			SceneObject::Unselect();

			m_color = { 1.0f, 1.0f, 1.0f };
		}

	protected:
		Vector3 m_color;

		std::vector<SceneObject*> m_controlPoints;

		static std::shared_ptr<DX11ConstantBuffer> VSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> PSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> GSConstantBuffer(const DX11Device& device);
		static std::shared_ptr<DX11ConstantBuffer> s_vsConstantBuffer;
		static std::shared_ptr<DX11ConstantBuffer> s_gsConstantBuffer;
		static std::shared_ptr<DX11ConstantBuffer> s_psConstantBuffer;
	};
}
