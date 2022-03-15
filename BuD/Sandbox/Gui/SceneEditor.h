#pragma once

#include <BuD.h>

using namespace DirectX::SimpleMath;

class SceneEditor
{
public:
	SceneEditor(std::shared_ptr<BuD::Parameterized2DEntity> selected);

	void RenderGui();
protected:

	std::shared_ptr<BuD::Parameterized2DEntity> m_selectedEntity;

	bool m_show = true;
};
