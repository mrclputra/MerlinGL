#pragma once

// Merlin Engine Public API
// for use by external Merlin applications

// Core
#include <Merlin/Core/Application.h>
#include <Merlin/Core/ApplicationBuilder.h>
#include <Merlin/Core/Layer.h>
#include <Merlin/Core/Logger.h>
#include <Merlin/Platform/GLFW/GLFWInput.h>

// Events
#include <Merlin/Events/Events.h>

// Assets
#include <Merlin/Assets/Asset.h>
#include <Merlin/Assets/AssetManager.h>
#include <Merlin/Assets/MeshAsset.h>
#include <Merlin/Assets/ShaderAsset.h>

// Rendering
#include <Merlin/Rendering/Framebuffer.h>

// ECS
#include <Merlin/ECS/Entity.h>
#include <Merlin/ECS/Component.h>
#include <Merlin/ECS/System.h>
#include <Merlin/ECS/Registry.h>

#include <Merlin/ECS/Components/Transform.h>
#include <Merlin/ECS/Components/MeshRenderer.h>
#include <Merlin/ECS/Systems/RenderSystem.h>

// Gui
#include <Merlin/Gui/Widget.h>
#include <Merlin/Gui/Console.h>
#include <Merlin/Gui/Profiler.h>

// imgui symbols are exported with the library
// so this should be usable no problem
#include <imgui.h>
#include <imgui_internal.h>