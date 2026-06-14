#pragma once

// standard library
#include <iostream>
#include <memory>
#include <algorithm>
#include <functional>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <mutex>

#ifdef _WIN32
	#include <Windows.h>
#endif

// TODO: external builds should not include the libraries below
//	except maybe GLFW, cuz i refuse to write my own keycodes :)

// external
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>