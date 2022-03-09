#pragma once

#include "Core/ClientApp.h"
#include "Core/Window.h"
#include "Core/Renderer.h"
#include "Core/MouseCode.h"
#include "Core/KeyboardKeys.h"

#include "Event/KeyboardEvents.h"
#include "Event/MouseEvents.h"
#include "Event/WindowEvents.h"

#ifdef _WIN32

#include "Win32/Win32Window.h"
#include "DirectX11/DX11Renderer.h"

#endif
