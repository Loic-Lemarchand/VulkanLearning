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
		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };  }
		bool wasWindowwResized() { return frameBufferResized; }
		void resetWindowResizedFlag() { frameBufferResized = false; }
		GLFWwindow* getGLFWwindow() const { return window; }


		void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
		
	private:
		static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
		void initWindow();


		int width;
		int height;
		bool frameBufferResized = false;

		std::string windowName;
		GLFWwindow* window;




};

}//End Namespace 

////This file's code is from Brendan Galea's vulkan tutorial
// 
// (2020, 13 december). Device Setup & Pipeline cont. - Vulkan Game Engine Tutorial 03. https://www.youtube.com/watch?v=LYKlEIzGmW4
//     
// tutorial03 - Google Drive. (s. d.). https://drive.google.com/drive/folders/1Hs-3v_AFVbASmymY4I2UB-JWvW3-hTAV
// 
/////

