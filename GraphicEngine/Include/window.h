#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>


namespace Lve{

class Window
{
	public:
		Window(int w, int h, std::string name);
		~Window();

		//We don't want to copy a window - we want a single window
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;


		bool shoudlClose() { return glfwWindowShouldClose(window); }

		
	private:
		
		void initWindow();


		const int width;
		const int height;

		std::string windowName;
		GLFWwindow* window;




};

}//End Namespace 

