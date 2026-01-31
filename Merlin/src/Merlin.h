#pragma once 

// Merlin Engine Public API
// for use by external Merlin applications

// Core application
#include <Merlin/Application.h>
#include <Merlin/ApplicationBuilder.h>
#include <Merlin/Layer.h>

// other
#include <Merlin/Platform/GLFW/GLFWInput.h>
#include <Merlin/Logger.h>
#include <Merlin/Events/Events.h>

// assets
#include <Merlin/Assets/Asset.h>
#include <Merlin/Assets/AssetManager.h>
#include <Merlin/Assets/MeshAsset.h>
#include <Merlin/Assets/ShaderAsset.h>

// rendering
#include <Merlin/Rendering/Framebuffer.h>

// ECS
#include <Merlin/ECS/Entity.h>
#include <Merlin/ECS/Component.h>
#include <Merlin/ECS/System.h>
#include <Merlin/ECS/Registry.h>

// Components
#include <Merlin/ECS/Components/Transform.h>
#include <Merlin/ECS/Components/MeshRenderer.h>

// Systems
#include <Merlin/ECS/Systems/RenderSystem.h>

// ImGui
#include <imgui.h>
#include <imgui_internal.h>