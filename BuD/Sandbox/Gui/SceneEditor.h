#pragma once

#include <BuD.h>

using namespace DirectX::SimpleMath;

class SceneEditor
{
public:
	SceneEditor(std::vector<std::shared_ptr<BuD::Parameterized2DEntity>> entities, std::shared_ptr<BuD::Parameterized2DEntity> selected = nullptr);

	void RenderGui();
protected:

	std::vector<BuD::Parameterized2DEntity*> m_entities;
	BuD::Parameterized2DEntity* m_selectedEntity;

	bool m_show = true;
};
