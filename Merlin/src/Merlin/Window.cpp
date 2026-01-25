#include "mepch.h"

#include <Merlin/Window.h>
#include <Merlin/Logger.h>
#include <Merlin/Events/AppEvents.h>
#include <Merlin/Events/InputEvents.h>

namespace Merlin {
	Window::Window(const WindowProps& props) { Init(props); }
	Window::~Window() { Shutdown(); }

	void Window::Init(const WindowProps& props) {
		m_Data.Title = props.title;
		m_Data.Width = props.width;
		m_Data.Height = props.height;

		if (!glfwInit()) {
			logger.error("Failed to initialize GLFW");
			std::exit(EXIT_FAILURE);
		}
		logger.info("Initialized GLFW");

		m_Window = glfwCreateWindow(props.width, props.height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		glfwMakeContextCurrent(m_Window);
		SetVSync(false);

		// load GLAD opengl function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			logger.error("Failed to initialize GLAD");
			std::exit(EXIT_FAILURE);
		}

		logger.info("OpenGL Version: ", glGetString(GL_VERSION));
		logger.info("GLSL Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width; 
			data.Height = height;
			
			WindowResizeEvent event(width, height);
			data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
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
		
		m_Data.VSync = enabled;
	}

	bool Window::IsVSync() const {
		return m_Data.VSync;
	}
}