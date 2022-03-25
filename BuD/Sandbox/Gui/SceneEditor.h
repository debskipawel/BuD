#pragma once

#include <BuD.h>

using namespace DirectX::SimpleMath;

class SceneEditor
{
public:
	SceneEditor();

	void RenderGui();
protected:

	BuD::SceneObject* m_selectedEntity = nullptr;

	bool m_show = true;
};
