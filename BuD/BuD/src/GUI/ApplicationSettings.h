#pragma once

#include <Core/InteractionMode.h>

namespace BuD
{
	struct ApplicationSettings
	{
		InteractionMode m_appMode = InteractionMode::NONE;
		bool m_inAction = false;
		bool m_inPreviewMode = false;
		bool m_cameraMoving = false;
	};
}
