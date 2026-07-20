#ifndef MERLINGL_CORE_PCH_H
#define MERLINGL_CORE_PCH_H

// precompiled header for Core and Engine files; main library
// this is never included by a public header btw; as they should stay self-contained since consuming code will compile them without this pch

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <entt/entt.hpp>
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
#include <fstream>
#include <sstream>
#include <string>

#endif  // MERLINGL_CORE_PCH_H