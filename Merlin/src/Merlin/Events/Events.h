#pragma once

#include "Event.h"
#include "EventBus.h"

namespace Merlin {
	// Application Events
	class MERLIN_API WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowResize)

	private:
		unsigned int m_Width, m_Height;
	};

	class MERLIN_API WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowClose)
	};

	class MERLIN_API AppTickEvent : public Event {
	public:
		AppTickEvent() {}
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppTick)
	};

	class MERLIN_API AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppUpdate)
	};

	class MERLIN_API AppRenderEvent: public Event {
	public:
		AppRenderEvent() {}
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppRender)
	};

	class MERLIN_API LogEvent : public Event {
	public:
		LogEvent(const std::string& message) : m_Message(message) {}
		std::string GetMessage() const { return m_Message; }
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(Log)
	private:
		std::string m_Message;
	};

	// Input Events
	class MERLIN_API KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return m_KeyCode; }
	protected:
		KeyEvent(int keycode) : m_KeyCode(keycode) {}
		int m_KeyCode;
	};

	class MERLIN_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << ")";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	class MERLIN_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
		EVENT_CLASS_TYPE(KeyReleased)
	};

	class MERLIN_API MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
		EVENT_CLASS_TYPE(MouseMoved)
	private:
		float m_MouseX, m_MouseY;
	};

	class MERLIN_API MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
		EVENT_CLASS_TYPE(MouseScrolled)
	private:
		float m_XOffset, m_YOffset;
	};

	class MERLIN_API MouseButtonEvent : public Event {
	public:
		inline int GetMouseButton() const { return m_Button; }
	protected:
		MouseButtonEvent(int button) : m_Button(button) {}
		int m_Button;
	};

	class MERLIN_API MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MERLIN_API MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	// Event type aliases
	namespace Events {
		using WindowClose = WindowCloseEvent;
		using WindowResize = WindowResizeEvent;
		using AppTick = AppTickEvent;
		using AppUpdate = AppUpdateEvent;
		using AppRender = AppRenderEvent;
		using KeyPressed = KeyPressedEvent;
		using KeyReleased = KeyReleasedEvent;
		using MouseMoved = MouseMovedEvent;
		using MouseScrolled = MouseScrolledEvent;
		using MouseButtonPressed = MouseButtonPressedEvent;
		using MouseButtonReleased = MouseButtonReleasedEvent;
		using Log = LogEvent;

		template<typename T>
		inline SubscriptionHandle On(std::function<void(T&)> callback) {
			return EventBus::Get().On<T>(callback);
		}

		inline SubscriptionHandle OnInput(std::function<void(Event&)> callback) {
			return EventBus::Get().OnCategory(EventCategoryInput, callback);
		}
		inline SubscriptionHandle OnKeyboard(std::function<void(Event&)> callback) {
			return EventBus::Get().OnCategory(EventCategoryKeyboard, callback);
		}
		inline SubscriptionHandle OnMouse(std::function<void(Event&)> callback) {
			return EventBus::Get().OnCategory(EventCategoryMouse, callback);
		}
		inline SubscriptionHandle OnApplication(std::function<void(Event&)> callback) {
			return EventBus::Get().OnCategory(EventCategoryApplication, callback);
		}

		inline void Emit(Event& event) {
			EventBus::Get().Emit(event);
		}
	}
}
