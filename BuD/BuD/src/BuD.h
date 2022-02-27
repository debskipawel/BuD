#pragma once

#include "Core/ClientApp.h"
#include "Core/Window.h"
#include "Core/Renderer.h"
#include "Core/MouseCode.h"
#include "Core/KeyboardKeys.h"

#include "Event/KeyboardEvents.h"
#include "Event/MouseEvents.h"
#include "Event/WindowEvents.h"

#include "OpenGL/GLFWWindowFactory.h"

#include "gmtl/gmtl.h"

#ifdef _WIN32

#include "Win32/Win32Window.h"
#include "Win32/DirectX11/DX11Renderer.h"

#endif
