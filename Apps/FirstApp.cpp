#include "FirstApp.h"


namespace Lve {

	void FirstApp::run()
	{
		while (!Window.shoudlClose())
		{
			glfwPollEvents();
		}
	}
}