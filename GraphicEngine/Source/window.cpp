#include "../Include/window.h"

namespace Lve {

	Window::Window(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name }
	{
		initWindow();
	}

	Window::~Window()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void Window::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);//indique de ne pas utiliser opengl
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);//empeche la fenetre de se resizer au demarrage
	
		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	}

}