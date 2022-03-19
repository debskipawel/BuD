#pragma once

#include "Core/ClientApp.h"
#include "Core/Window.h"
#include "Core/Renderer.h"
#include "Core/MouseCode.h"
#include "Core/KeyboardKeys.h"

#include "Camera/CameraFactory.h"

#include "DirectX11/Buffers/DX11ConstantBuffer.h"
#include "DirectX11/Buffers/DX11VertexBuffer.h"
#include "DirectX11/Buffers/DX11IndexBuffer.h"
#include "DirectX11/Shaders/Loader/DX11ShaderLoader.h"

#include "Event/KeyboardEvents.h"
#include "Event/MouseEvents.h"
#include "Event/WindowEvents.h"

#include "Geometry/Torus.h"
#include "Geometry/Point.h"

#include "Scene/Mesh.h"

#include "imgui.h"

#ifdef _WIN32

#include "Win32/Win32Window.h"
#include "DirectX11/DX11Renderer.h"

#endif
