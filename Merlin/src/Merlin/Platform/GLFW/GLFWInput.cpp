#include "mepch.h"
#include "GLFWInput.h"

namespace Merlin {
	Input* Input::s_Instance = new GLFWInput();

	void Input::Init(void* nativeWindow) {
		s_Instance->m_NativeWindow = nativeWindow;
	}

	bool GLFWInput::IsKeyPressedImpl(int keycode) {
		auto window = static_cast<GLFWwindow*>(m_NativeWindow);
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool GLFWInput::IsMouseButtonPressedImpl(int button) {
		auto window = static_cast<GLFWwindow*>(m_NativeWindow);
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> GLFWInput::GetMousePositionImpl() {
		auto window = static_cast<GLFWwindow*>(m_NativeWindow);
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	float GLFWInput::GetMouseXImpl() {
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float GLFWInput::GetMouseYImpl() {
		auto [x, y] = GetMousePositionImpl();
		return y;
	}
}