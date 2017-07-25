#include "txbase/stdafx.h"
#include "txbase/opengl/application.h"
#include <thread>
#include <chrono>

namespace TX {
	namespace UI {
		Application::Application() {
			config.title = "Application";
		}
		void Application::Run() {
			std::printf("GLFW: \t%s\n", glfwGetVersionString());
			glfwSetErrorCallback(GLFWError);
			if (!glfwInit())
				throw std::runtime_error("glfwInit failed");

			glfwWindowHint(GLFW_VISIBLE, 0);
			glfwWindowHint(GLFW_FOCUSED, 0);
			glfwWindowHint(GLFW_RESIZABLE, config.resizable);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		#if __APPLE__
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif

			window = glfwCreateWindow(
				config.windowSize.x,
				config.windowSize.y,
				config.title.c_str(),
				config.fullscreen ? glfwGetPrimaryMonitor() : NULL,
				NULL);
			if (!window) {
				glfwTerminate();
				throw std::runtime_error("glfwCreateWindow failed");
			}
			glfwSetWindowUserPointer(window, this);
			glfwMakeContextCurrent(window);
			glfwSwapInterval(1);

			glfwSetKeyCallback(window, GLFWKey);
// 			glfwSetCharModsCallback(window, GLFWCharMods); // 3.1
			glfwSetCharCallback(window, GLFWChar); // 3.1
			glfwSetCursorPosCallback(window, GLFWCursorPos);
			glfwSetMouseButtonCallback(window, GLFWMouseButton);
			glfwSetScrollCallback(window, GLFWMouseScroll);
			glfwSetFramebufferSizeCallback(window, GLFWFramebufferSize);
			glfwSetWindowSizeCallback(window, GLFWWindowSize);
			glfwSetWindowFocusCallback(window, GLFWWindowFocus);
			glfwSetWindowPosCallback(window, GLFWWindowPos);
			glfwSetWindowIconifyCallback(window, GLFWWindowIconify);


			if (gl3wInit()) {
				std::cerr << "gl3wInit failed." << std::endl;
				throw std::runtime_error("");
			}

			std::cerr << "OpenGL: \t" << GetVersion() << std::endl;

			Start();
			glfwShowWindow(window);
			//glfwFocusWindow(window);	// TODO version 3.2+

			// Rendering loop
			bool busy = true;
			while (!glfwWindowShouldClose(window)) {
				FrameStart();
				busy = Render();
				if (busy) {
					glfwPollEvents();
				}
				else {
					glfwWaitEvents();
				}
				glfwSwapBuffers(window);
				FrameEnd();
			}

			OnExit();
			glfwDestroyWindow(window);
			glfwTerminate();
		}
		Application::~Application() {}

		MouseButtonState	Application::Get(MouseButton button) { return MouseButtonState(glfwGetMouseButton(window, static_cast<int>(button))); }
		bool				Application::Get(KeyCode code) { return glfwGetKey(window, static_cast<int>(code)) == GLFW_PRESS; }
		const char *		Application::GetVersion() { return (const char *)glGetString(GL_VERSION); }
		GLFWwindow *		Application::GetWindow() { return window; }

		float				Application::GetTime() { return (float)glfwGetTime(); }
		float				Application::GetDeltaTime() { return deltaTime; };
		float				Application::GetFrameRate() { return fps; }
		void				Application::GetCursorPos(float *x, float *y) { double dx, dy; glfwGetCursorPos(window, &dx, &dy); *x = float(dx); *y = float(dy); }
// 		void				Application::Refresh() { glfwPostEmptyEvent(); }	// GLFW 3.1
		bool				Application::IsWindowVisible() { return glfwGetWindowAttrib(window, GLFW_VISIBLE); }
		void				Application::Exit() { glfwSetWindowShouldClose(window, GL_TRUE); }

		Application * Application::This(GLFWwindow *window) { return (Application *)(glfwGetWindowUserPointer(window)); }
		void Application::GLFWKey(GLFWwindow *window, int key, int scancode, int action, int mods) {
			This(window)->OnKey(KeyCode(key), KeyState(action), mods);
		}

		// TODO replace with GLFWCharMods in 3.1
		void Application::GLFWChar(GLFWwindow *window, uint codepoint) {
			This(window)->OnText(codepoint, 0);
		}
// 		void Application::GLFWCharMods(GLFWwindow *window, uint codepoint, int mods) {
// 			This(window)->OnText(codepoint, mods);
// 		}
		void Application::GLFWCursorPos(GLFWwindow *window, double x, double y) {
			This(window)->OnMouseMove(float(x), float(y));
		}
		void Application::GLFWMouseButton(GLFWwindow *window, int button, int state, int mods) {
			This(window)->OnMouseButton(MouseButton(button), MouseButtonState(state), mods);
		}
		void Application::GLFWMouseScroll(GLFWwindow *window, double xoff, double yoff) { This(window)->OnMouseScroll(float(xoff), float(yoff)); }
		void Application::GLFWFramebufferSize(GLFWwindow *window, int w, int h) {
			auto *app = This(window);
			Vec2i newSize(w, h);
			if (app->config.windowSize != newSize) {
				app->config.windowSize = newSize;
				app->OnResize();
				glViewport(0, 0, w, h);
			}
		}
		void Application::GLFWWindowSize(GLFWwindow *window, int w, int h) { This(window)->OnWindowResize(w, h); }
		void Application::GLFWWindowFocus(GLFWwindow *window, int focused) { This(window)->OnWindowFocusChanged(focused); }
		void Application::GLFWWindowPos(GLFWwindow *window, int x, int y) { This(window)->OnWindowPos(x, y); }
		void Application::GLFWWindowIconify(GLFWwindow *window, int iconified) { This(window)->OnWindowMinimize(); }
		void Application::GLFWError(int error, const char* desc) {
			std::cerr << "GLFWError " << error << ": " << desc << std::endl;
		}

		void Application::FrameStart() {
			float now = GetTime();
			deltaTime = now - frameStart;
			fps = Math::Lerp(0.95f, 1.f / deltaTime, fps);
			frameStart = now;
			//glfwSetWindowTitle(window, (config.title + '[' + std::to_string(int(GetFrameRate())) + ']').c_str());
		}

		void Application::FrameEnd() {
			float now = GetTime();
			frameEnd = now;

			// limit fps
			float deadLine = frameStart + 1.f / config.fps;
			while (deadLine > now) {
				float waitTime = (deadLine - now) * 0.6f;
				if (waitTime > 0.005f) {
					std::this_thread::sleep_for(std::chrono::milliseconds(int(waitTime * 1000)));
				}
				now = GetTime();
			}
		}
	}
}
