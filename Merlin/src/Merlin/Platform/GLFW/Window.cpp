#include "mepch.h"

#include "Window.h"
#include <Merlin/Logger.h>
#include <Merlin/Events/AppEvents.h>
#include <Merlin/Events/InputEvents.h>

namespace Merlin {
	Window::Window(const WindowProps& props) { Init(props); }
	Window::~Window() { Shutdown(); }

	void Window::Init(const WindowProps& props) {
		m_Title = props.title;
		m_Width = props.width;
		m_Height = props.height;

		if (!glfwInit()) {
			logger.error("Failed to initialize GLFW");
			std::exit(EXIT_FAILURE);
		}
		logger.info("Initialized GLFW");

		m_Window = glfwCreateWindow(props.width, props.height, m_Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, this);
		SetVSync(true);

		// load GLAD opengl function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			logger.error("Failed to initialize GLAD");
			std::exit(EXIT_FAILURE);
		}

		logger.info("OpenGL Version: ", glGetString(GL_VERSION));
		logger.info("GLSL Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			Window& win = *(Window*)glfwGetWindowUserPointer(window);
			win.m_Width = width;
			win.m_Height = height;

			WindowResizeEvent event(width, height);
			win.m_EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			Window& win = *(Window*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			win.m_EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			Window& win = *(Window*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					win.m_EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					win.m_EventCallback(event);
					break;
				}

				// enable this if we want repeating event calls
				//case GLFW_REPEAT: {
				//	KeyPressedEvent event(key, 1);
				//	win.m_EventCallback(event);
				//	break;
				//}
			}
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			Window& win = *(Window*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(button);
					win.m_EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(button);
					win.m_EventCallback(event);
					break;
				}
			}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			Window& win = *(Window*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			win.m_EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			Window& win = *(Window*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			win.m_EventCallback(event);
			});
	}

	void Window::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void Window::Shutdown() { 
		glfwDestroyWindow(m_Window);
	}

	void Window::SetVSync(bool enabled) {
		if (enabled) 
			glfwSwapInterval(1);
		else 
			glfwSwapInterval(0);
		
		m_VSync = enabled;
	}

	bool Window::IsVSync() const {
		return m_VSync;
	}
}