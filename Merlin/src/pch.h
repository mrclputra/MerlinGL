#ifndef MERLINGL_PCH_H
#define MERLINGL_PCH_H

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <entt/entt.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>
#include <future>
#include <thread>
#include <atomic>
#include <queue>
#include <set>
#include <spdlog/sinks/ringbuffer_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <portable-file-dialogs.h>
#include <fstream>
#include <sstream>
#include <string>

#endif  // MERLINGL_PCH_H