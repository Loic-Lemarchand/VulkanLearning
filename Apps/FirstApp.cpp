#include "FirstApp.h"


namespace Lve {

	void FirstApp::run()
	{
		while (!lveWindow.shoudlClose())
		{
			glfwPollEvents();
		}
	}
}